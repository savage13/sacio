#!/bin/bash
set -ev
touch configure.ac aclocal.m4 configure Makefile.am Makefile.in
ls -lrt
./configure && make -k && make check
