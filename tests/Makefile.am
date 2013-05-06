TESTS = \
    mutex_working \
    two_threads_one_mutex

check_PROGRAMS = \
    mutextrace \
    $(TESTS)

check_LIBRARIES = libcommon.a

libcommon_a_SOURCES = common.c common.h

mutextrace_SOURCES = ../mutextrace.c
mutextrace_CPPFLAGS = $(AM_CPPFLAGS) -DPRELOADLIB=\"$(top_builddir)/.libs/mutextrace.so\"

LDFLAGS = -no-install
LDADD = libcommon.a @LIBPTHREAD@
