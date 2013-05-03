TESTS = \
    mutex_working \
    two_threads_one_mutex

check_PROGRAMS = \
    mutextrace \
    $(TESTS)

mutextrace_SOURCES = ../mutextrace.c
mutextrace_CPPFLAGS = $(AM_CPPFLAGS) -DPRELOADLIB=\"$(top_builddir)/.libs/mutextrace.so\"

LDFLAGS = -no-install
LDADD = @LIBPTHREAD@
