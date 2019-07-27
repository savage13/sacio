#!/bin/bash
set -ev
touch aclocal.m4
./configure && make && make check
