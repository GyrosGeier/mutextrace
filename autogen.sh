#! /bin/sh -ex
aclocal
autoheader
libtoolize
automake -a
autoconf
