#!/bin/bash
set -ev
ls -lrt
./configure && make -d && make check
