
AC_INIT([sacio], [1.0.2], [savage13@gmail.com])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([-Wall -Werror foreign no-dependencies subdir-objects ])
AM_MAINTAINER_MODE
AC_CONFIG_HEADERS([config.h])
AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB

AC_DEFINE([USE_GEOGRAPHICLIB], [1], [Use the geographiclib library for geodetic calcs])

AC_CHECK_FUNC(fmemopen,   [ AC_DEFINE( [HAVE_FUNC_FMEMOPEN],       [1], [ System Libraries define fmemopen ]) ],
                         [ AC_DEFINE( [MISSING_FUNC_FMEMOPEN],    [1], [ System Libraries missing fmemopen ]) ]  )

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

