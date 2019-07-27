#!/bin/sh

# inc/config.h.in needs to be "younger" than configure.in
#   Otherwise autoheader will attempted to be run, which we do not want
#   autotools considers config.h.in to be a derivative of configure.in
aclocal && \
automake --add-missing --foreign --copy -i && \
# autoheader && \
touch config.h.in && \
autoconf

rm -rf autom4te.cache/

#autoreconf --verbose  --install
#rm -rf autom4te.cache


