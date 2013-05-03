SUBDIRS = . tests

bin_PROGRAMS = mutextrace

pkglib_LTLIBRARIES = mutextrace.la

mutextrace_SOURCES = mutextrace.c
mutextrace_CPPFLAGS = $(AM_CPPFLAGS) -DPRELOADLIB=\"$(pkglibdir)/mutextrace.so\"

mutextrace_la_SOURCES = \
    mutex_destroy.c \
    mutex_init.c \
    mutex_lock.c \
    mutex_unlock.c \
    real.c real.h \
    store.c store.h \
    thread_create.c
mutextrace_la_LDFLAGS = -module -avoid-version -no-undefined -Wl,-z,defs
mutextrace_la_LIBADD = @LIBPTHREAD@

EXTRA_DIST = COPYING
