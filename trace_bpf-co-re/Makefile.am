AM_CPPFLAGS = -O2 -g -fstack-protector-strong -fPIE -fPIC -D_FORTIFY_SOURCE=2 \
		-Wformat -Wformat-security -Wformat-overflow -Wno-parentheses \
		-Wno-missing-field-initializers -Wextra -Wall -fno-common

AM_LDFLAGS = -Wl,-z,noexecstack,-z,relro,-z,now -pie

AUTOMAKE_OPTIONS = subdir-objects

SUBDIRS =  include headers src
