TESTS = $(check_PROGRAMS)

check_PROGRAMS = \
    mutex_working \
    two_threads_one_mutex

mutex_working_SOURCES = mutex_working.c
mutex_working_LDADD = ../mutextrace.la @LIBPTHREAD@

two_threads_one_mutex_SOURCES = two_threads_one_mutex.c
two_threads_one_mutex_LDADD = ../mutextrace.la @LIBPTHREAD@

LDFLAGS = -no-install
