# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([mutextrace], [0.1.4], [Simon.Richter@hogyros.de])
AM_INIT_AUTOMAKE([1.10 foreign parallel-tests])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile tests/Makefile])

AM_MAINTAINER_MODE

AC_DISABLE_STATIC

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create], [AC_SUBST([LIBPTHREAD], [-lpthread])], [AC_MSG_ERROR([*** libpthread missing])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
