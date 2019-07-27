
# Do not create dependencies while compiling
AUTOMAKE_OPTIONS = no-dependencies

EXTRA_DIST = License README.md

AM_CFLAGS = -Wall -Wextra -Iinclude/

saciolibdir = $(libdir)/
sacioincdir = $(includedir)/sacio

saciolib_LIBRARIES = libsacio_bsd.a
sacioinc_HEADERS   = include/sacio/sacio.h include/sacio/timespec.h

libsacio_bsd_a_SOURCES = src/sacio.c src/geodesic.c src/timespec.c \
												 src/time64.c src/strip.c src/compat.c \
												 src/header_map.c src/defs.h src/sacio.h src/time64.h \
												 src/timespec.h src/geodesic.h src/strip.h \
												 src/time64_limits.h

# TESTS

TESTS = t/iotest t/compat

check_PROGRAMS = t/iotest t/compat

t_iotest_SOURCES = t/iotest.c
t_compat_SOURCES = t/compat.c

t_iotest_CFLAGS = -Iinclude
t_compat_CFLAGS = -Iinclude
t_iotest_LDADD = libsacio_bsd.a
t_compat_LDADD = libsacio_bsd.a

CLEANFILES = t/test*.tmp


doc:
	doxygen docs/Doxyfile


distclean-local:
	-rm -rf autom4te.cache

src/header_map.c: src/header_map.txt
	gperf -t -T --output-file=$@ -N sac_keyword_to_header $<
