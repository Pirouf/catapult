# Copyright (c) 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Module containing utilities for apk packages."""

import itertools
import re

from devil import base_error
from devil.android.sdk import aapt


_MANIFEST_ATTRIBUTE_RE = re.compile(
    r'\s*A: ([^\(\)= ]*)(?:\([^\(\)= ]*\))?='
    r'(?:"(.*)" \(Raw: .*\)|\(type.*?\)(.*))$')
_MANIFEST_ELEMENT_RE = re.compile(r'\s*(?:E|N): (\S*) .*$')


def GetPackageName(apk_path):
  """Returns the package name of the apk."""
  return ApkHelper(apk_path).GetPackageName()


# TODO(jbudorick): Deprecate and remove this function once callers have been
# converted to ApkHelper.GetInstrumentationName
def GetInstrumentationName(apk_path):
  """Returns the name of the Instrumentation in the apk."""
  return ApkHelper(apk_path).GetInstrumentationName()


def ToHelper(path_or_helper):
  """Creates an ApkHelper unless one is already given."""
  if isinstance(path_or_helper, basestring):
    return ApkHelper(path_or_helper)
  return path_or_helper


def _ParseManifestFromApk(apk_path):
  aapt_output = aapt.Dump('xmltree', apk_path, 'AndroidManifest.xml')

  parsed_manifest = {}
  node_stack = [parsed_manifest]
  indent = '  '

  for line in aapt_output[1:]:
    if len(line) == 0:
      continue

    indent_depth = 0
    while line[(len(indent) * indent_depth):].startswith(indent):
      indent_depth += 1

    node_stack = node_stack[:indent_depth]
    node = node_stack[-1]

    m = _MANIFEST_ELEMENT_RE.match(line[len(indent) * indent_depth:])
    if m:
      manifest_key = m.group(1)
      if manifest_key in node:
        node[manifest_key] += [{}]
      else:
        node[manifest_key] = [{}]
      node_stack += [node[manifest_key][-1]]
      continue

    m = _MANIFEST_ATTRIBUTE_RE.match(line[len(indent) * indent_depth:])
    if m:
      manifest_key = m.group(1)
      if manifest_key in node:
        raise base_error.BaseError(
            "A single attribute should have one key and one value")
      else:
        node[manifest_key] = m.group(2) or m.group(3)
      continue

  return parsed_manifest


def _ParseNumericKey(obj, key, default=0):
  val = obj.get(key)
  if val is None:
    return default
  return int(val, 0)


class _ExportedActivity(object):
  def __init__(self, name):
    self.name = name
    self.actions = set()
    self.categories = set()
    self.schemes = set()


def _IterateExportedActivities(manifest_info):
  app_node = manifest_info['manifest'][0]['application'][0]
  activities = app_node.get('activity', []) + app_node.get('activity-alias', [])
  for activity_node in activities:
    if not _ParseNumericKey(activity_node, 'android:exported'):
      continue
    activity = _ExportedActivity(activity_node.get('android:name'))
    # Merge all intent-filters into a single set because there is not
    # currently a need to keep them separate.
    for intent_filter in activity_node.get('intent-filter', []):
      for action in intent_filter.get('action', []):
        activity.actions.add(action.get('android:name'))
      for category in intent_filter.get('category', []):
        activity.categories.add(category.get('android:name'))
      for data in intent_filter.get('data', []):
        activity.schemes.add(data.get('android:scheme'))
    yield activity


class ApkHelper(object):

  def __init__(self, path):
    self._apk_path = path
    self._manifest = None

  @property
  def path(self):
    return self._apk_path

  def GetActivityName(self):
    """Returns the name of the first launcher Activity in the apk."""
    manifest_info = self._GetManifest()
    for activity in _IterateExportedActivities(manifest_info):
      if ('android.intent.action.MAIN' in activity.actions and
          'android.intent.category.LAUNCHER' in activity.categories):
        return self._ResolveName(activity.name)
    return None

  def GetViewActivityName(self):
    """Returns name of the first action=View Activity that can handle http."""
    manifest_info = self._GetManifest()
    for activity in _IterateExportedActivities(manifest_info):
      if ('android.intent.action.VIEW' in activity.actions and
          'http' in activity.schemes):
        return self._ResolveName(activity.name)
    return None

  def GetInstrumentationName(
      self, default='android.test.InstrumentationTestRunner'):
    """Returns the name of the Instrumentation in the apk."""
    all_instrumentations = self.GetAllInstrumentations(default=default)
    if len(all_instrumentations) != 1:
      raise base_error.BaseError(
          'There is more than one instrumentation. Expected one.')
    else:
      return self._ResolveName(all_instrumentations[0]['android:name'])

  def GetAllInstrumentations(
      self, default='android.test.InstrumentationTestRunner'):
    """Returns a list of all Instrumentations in the apk."""
    try:
      return self._GetManifest()['manifest'][0]['instrumentation']
    except KeyError:
      return [{'android:name': default}]

  def GetPackageName(self):
    """Returns the package name of the apk."""
    manifest_info = self._GetManifest()
    try:
      return manifest_info['manifest'][0]['package']
    except KeyError:
      raise Exception('Failed to determine package name of %s' % self._apk_path)

  def GetPermissions(self):
    manifest_info = self._GetManifest()
    try:
      return [p['android:name'] for
              p in manifest_info['manifest'][0]['uses-permission']]
    except KeyError:
      return []

  def GetSplitName(self):
    """Returns the name of the split of the apk."""
    manifest_info = self._GetManifest()
    try:
      return manifest_info['manifest'][0]['split']
    except KeyError:
      return None

  def HasIsolatedProcesses(self):
    """Returns whether any services exist that use isolatedProcess=true."""
    manifest_info = self._GetManifest()
    try:
      applications = manifest_info['manifest'][0].get('application', [])
      services = itertools.chain(
          *(application.get('service', []) for application in applications))
      return any(
          _ParseNumericKey(s, 'android:isolatedProcess') for s in services)
    except KeyError:
      return False

  def _GetManifest(self):
    if not self._manifest:
      self._manifest = _ParseManifestFromApk(self._apk_path)
    return self._manifest

  def _ResolveName(self, name):
    name = name.lstrip('.')
    if '.' not in name:
      return '%s.%s' % (self.GetPackageName(), name)
    return name
