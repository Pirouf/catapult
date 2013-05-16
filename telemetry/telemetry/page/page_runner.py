# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
import codecs
import glob
import logging
import os
import time
import traceback
import urlparse
import random

from telemetry.core import util
from telemetry.core import wpr_modes
from telemetry.core import exceptions
from telemetry.page import page_measurement_results
from telemetry.page import page_filter as page_filter_module
from telemetry.page import page_test

class PageState(object):
  def __init__(self):
    self.did_login = False

class _RunState(object):
  def __init__(self):
    self.first_browser = True
    self.browser = None
    self.tab = None
    self.is_tracing = False

  def Close(self):
    self.is_tracing = False

    if self.tab:
      self.tab.Disconnect()
      self.tab = None

    if self.browser:
      self.browser.Close()
      self.browser = None

def _ShuffleAndFilterPageSet(page_set, options):
  if options.pageset_shuffle_order_file and not options.pageset_shuffle:
    raise Exception('--pageset-shuffle-order-file requires --pageset-shuffle.')

  if options.pageset_shuffle_order_file:
    return page_set.ReorderPageSet(options.pageset_shuffle_order_file)

  page_filter = page_filter_module.PageFilter(options)
  pages = [page for page in page_set.pages[:]
           if not page.disabled and page_filter.IsSelected(page)]

  if options.pageset_shuffle:
    random.Random().shuffle(pages)
  return [page
      for _ in xrange(int(options.pageset_repeat))
      for page in pages
      for _ in xrange(int(options.page_repeat))]

class PageRunner(object):
  """Runs a given test against a given test."""
  def __init__(self, page_set):
    self.page_set = page_set
    self.has_called_will_run_page_set = False

  def __enter__(self):
    return self

  def __exit__(self, *args):
    self.Close()

  def Run(self, options, possible_browser, test, out_results):
    # Reorder page set based on options.
    pages = _ShuffleAndFilterPageSet(self.page_set, options)

    # Check if we can run against WPR.
    pages_without_archives = []
    for page in pages:
      parsed_url = urlparse.urlparse(page.url)
      if parsed_url.scheme == 'file':
        continue
      if not page.archive_path:
        if options.allow_live_sites:
          logging.warning("""
  No page set archive provided for the page %s. Running against live sites!
  Results won't be repeatable or comparable.
""", page.url)
        else:
          logging.warning("""
  No page set archive provided for the page %s. Not running the page. To run
  against live sites, pass the flag --allow-live-sites.
""", page.url)
          out_results.AddFailure(page, 'Page set archive not defined', '')
          pages_without_archives.append(page)
      elif options.wpr_mode != wpr_modes.WPR_RECORD:
        # The page has an archive, and we're not recording.
        if not os.path.isfile(page.archive_path):
          if options.allow_live_sites:
            logging.warning("""
  The page set archive %s for page %s does not exist, running against live
  sites! Results won't be repeatable or comparable.

  To fix this, either add svn-internal to your .gclient using
  http://goto/read-src-internal, or create a new archive using record_wpr.
  """, os.path.relpath(page.archive_path), page.url)
          else:
            logging.warning("""
  The page set archive %s for page %s does not exist. Not running the page.

  To fix this, either add svn-internal to your .gclient using
  http://goto/read-src-internal, or create a new archive using record_wpr.
  To run against live sites, pass the flag --allow-live-sites.
  """, os.path.relpath(page.archive_path), page.url)
            out_results.AddFailure(page, 'Page set archive doesn\'t exist', '')
            pages_without_archives.append(page)

    pages = [page for page in pages if page not in pages_without_archives]

    # Verify credentials path.
    credentials_path = None
    if self.page_set.credentials_path:
      credentials_path = os.path.join(os.path.dirname(self.page_set.file_path),
                                      self.page_set.credentials_path)
      if not os.path.exists(credentials_path):
        credentials_path = None

    # Set up user agent.
    if self.page_set.user_agent_type:
      options.browser_user_agent_type = self.page_set.user_agent_type

    for page in pages:
      test.CustomizeBrowserOptionsForPage(page, possible_browser.options)

    self._ValidateOrCreateEmptyDirectory('--trace-dir', options.trace_dir)
    self._ValidateOrCreateEmptyDirectory('--profiler-dir', options.profiler_dir)

    state = _RunState()
    last_archive_path = None
    results_for_current_run = out_results

    try:
      for page in pages:
        if options.wpr_mode != wpr_modes.WPR_RECORD:
          if page.archive_path and os.path.isfile(page.archive_path):
            possible_browser.options.wpr_mode = wpr_modes.WPR_REPLAY
          else:
            possible_browser.options.wpr_mode = wpr_modes.WPR_OFF
        if last_archive_path != page.archive_path:
          state.Close()
          state = _RunState()
          last_archive_path = page.archive_path
        if (test.discard_first_result and
            not self.has_called_will_run_page_set):
          # If discarding results, substitute a dummy object.
          results_for_current_run = (
            page_measurement_results.PageMeasurementResults())
        else:
          results_for_current_run = out_results
        tries = 3
        while tries:
          try:
            if not state.browser:
              self._SetupBrowser(state, test, possible_browser,
                                 credentials_path, page.archive_path)
            if not state.tab:
              if len(state.browser.tabs) == 0:
                state.browser.tabs.New()
              state.tab = state.browser.tabs[0]

            if state.browser.supports_tab_control:
              while len(state.browser.tabs) > 1:
                state.browser.tabs[-1].Close()

            self._WaitForThermalThrottlingIfNeeded(state.browser.platform)

            if options.trace_dir:
              self._SetupTracingTab(state)
            if options.profiler_dir:
              self._SetupProfiling(state, options, page)

            try:
              self._RunPage(options, page, state.tab, test,
                            results_for_current_run)
              self._CheckThermalThrottling(state.browser.platform)
            except exceptions.TabCrashException:
              stdout = ''
              if not options.show_stdout:
                stdout = state.browser.GetStandardOutput()
                stdout = (('\nStandard Output:\n') +
                          ('*' * 80) +
                          '\n\t' + stdout.replace('\n', '\n\t') + '\n' +
                          ('*' * 80))
              logging.warning('Tab crashed: %s%s', page.url, stdout)
              state.Close()

            if options.trace_dir:
              self._EndTracing(state, options, page)
            if options.profiler_dir:
              self._EndProfiling(state)

            if test.needs_browser_restart_after_each_run:
              state.Close()

            break
          except exceptions.BrowserGoneException:
            logging.warning('Lost connection to browser. Retrying.')
            state.Close()
            tries -= 1
            if not tries:
              logging.error('Lost connection to browser 3 times. Failing.')
              raise
      test.DidRunPageSet(state.tab, results_for_current_run)
    finally:
      state.Close()

  def _RunPage(self, options, page, tab, test, results):
    if not test.CanRunForPage(page):
      logging.warning('Skiping test: it cannot run for %s', page.url)
      results.AddSkippedPage(page, 'Test cannot run', '')
      return

    logging.info('Running %s' % page.url)

    page_state = PageState()
    try:
      did_prepare = self._PreparePage(page, tab, page_state, test, results)
    except util.TimeoutException, ex:
      logging.error(str(ex) + ' Timeout occurred during page %s', page.url)
      results.AddFailure(page, ex, traceback.format_exc())
      return
    except exceptions.TabCrashException, ex:
      results.AddFailure(page, ex, traceback.format_exc())
      raise
    except exceptions.BrowserGoneException:
      raise
    except Exception, ex:
      logging.error('Unexpected failure while running %s: %s',
                    page.url, traceback.format_exc())
      self._CleanUpPage(page, tab, page_state)
      raise

    if not did_prepare:
      self._CleanUpPage(page, tab, page_state)
      return

    try:
      test.Run(options, page, tab, results)
    except page_test.Failure, ex:
      logging.info('%s: %s', ex, page.url)
      results.AddFailure(page, ex, traceback.format_exc())
      return
    except util.TimeoutException, ex:
      logging.warning('Timed out while running %s', page.url)
      results.AddFailure(page, ex, traceback.format_exc())
      return
    except exceptions.TabCrashException, ex:
      results.AddFailure(page, ex, traceback.format_exc())
      raise
    except exceptions.BrowserGoneException:
      raise
    except Exception, ex:
      logging.error('Unexpected failure while running %s: %s',
                    page.url, traceback.format_exc())
      raise
    finally:
      self._CleanUpPage(page, tab, page_state)

    results.AddSuccess(page)

  def Close(self):
    pass

  def _GetSequentialFileName(self, base_name):
    """Returns the next sequential file name based on |base_name| and the
    existing files."""
    index = 0
    while True:
      output_name = '%s_%03d' % (base_name, index)
      if not glob.glob(output_name + '.*'):
        break
      index = index + 1
    return output_name

  def _ValidateOrCreateEmptyDirectory(self, name, path):
    if not path:
      return
    if not os.path.exists(path):
      os.mkdir(path)
    if not os.path.isdir(path):
      raise Exception('%s isn\'t a directory: %s' % (name, path))
    elif os.listdir(path):
      raise Exception('%s isn\'t empty: %s' % (name, path))

  def _SetupBrowser(self, state, test, possible_browser, credentials_path,
                    archive_path):
    assert not state.tab
    state.browser = possible_browser.Create()
    state.browser.credentials.credentials_path = credentials_path
    test.SetUpBrowser(state.browser)

    if state.first_browser:
      state.browser.credentials.WarnIfMissingCredentials(self.page_set)
      state.first_browser = False

    state.browser.SetReplayArchivePath(archive_path)

  def _SetupProfiling(self, state, options, page):
    output_file = os.path.join(options.profiler_dir, page.url_as_file_safe_name)
    if options.page_repeat != 1 or options.pageset_repeat != 1:
      output_file = self._GetSequentialFileName(output_file)
    state.browser.StartProfiling(options, output_file)

  def _EndProfiling(self, state):
    state.browser.StopProfiling()

  def _SetupTracingTab(self, state):
    if state.browser.supports_tracing:
      state.is_tracing = True
      state.browser.StartTracing()

  def _EndTracing(self, state, options, page):
    if state.is_tracing:
      assert state.browser
      state.is_tracing = False
      state.browser.StopTracing()
      trace_result = state.browser.GetTraceResultAndReset()
      logging.info('Processing trace...')

      trace_file = os.path.join(options.trace_dir, page.url_as_file_safe_name)
      if options.page_repeat != 1 or options.pageset_repeat != 1:
        trace_file = self._GetSequentialFileName(trace_file)
      trace_file += '.json'

      with codecs.open(trace_file, 'w',
                       encoding='utf-8') as trace_file:
        trace_result.Serialize(trace_file)
      logging.info('Trace saved.')

  def _PreparePage(self, page, tab, page_state, test, results):
    parsed_url = urlparse.urlparse(page.url)
    if parsed_url[0] == 'file':
      serving_dirs, filename = page.serving_dirs_and_file
      tab.browser.SetHTTPServerDirectories(serving_dirs)
      target_side_url = tab.browser.http_server.UrlOf(filename)
    else:
      target_side_url = page.url

    if page.credentials:
      page_state.did_login = tab.browser.credentials.LoginNeeded(
        tab, page.credentials)
      if not page_state.did_login:
        msg = 'Could not login to %s on %s' % (page.credentials,
                                               target_side_url)
        logging.info(msg)
        results.AddFailure(page, msg, "")
        return False

    if not self.has_called_will_run_page_set:
      self.has_called_will_run_page_set = True
      test.WillRunPageSet(tab, results)

    test.WillNavigateToPage(page, tab)
    tab.Navigate(target_side_url, page.script_to_evaluate_on_commit)
    test.DidNavigateToPage(page, tab)

    page.WaitToLoad(tab, 60)
    tab.WaitForDocumentReadyStateToBeInteractiveOrBetter()

    return True

  def _CleanUpPage(self, page, tab, page_state): # pylint: disable=R0201
    if page.credentials and page_state.did_login:
      tab.browser.credentials.LoginNoLongerNeeded(tab, page.credentials)
    util.CloseConnections(tab)

  def _WaitForThermalThrottlingIfNeeded(self, platform):
    if not platform.CanMonitorThermalThrottling():
      return
    thermal_throttling_retry = 0
    while (platform.IsThermallyThrottled() and
           thermal_throttling_retry < 3):
      logging.warning('Thermally throttled, waiting (%d)...',
                      thermal_throttling_retry)
      thermal_throttling_retry += 1
      time.sleep(thermal_throttling_retry * 2)

    if platform.IsThermallyThrottled():
      logging.error('Device is thermally throttled before running '
                    'performance tests, results will vary.')

  def _CheckThermalThrottling(self, platform):
    if not platform.CanMonitorThermalThrottling():
      return
    if platform.HasBeenThermallyThrottled():
      logging.error('Device has been thermally throttled during '
                    'performance tests, results will vary.')

  @staticmethod
  def AddCommandLineOptions(parser):
    page_filter_module.PageFilter.AddCommandLineOptions(parser)
