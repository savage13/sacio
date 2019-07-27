#!/bin/bash
set -ev
ls -lrt
configure && make && make check
