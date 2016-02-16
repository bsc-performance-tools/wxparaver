# AX_PROG_WXPROPGRID_NAME
# -----------
AC_DEFUN([AX_PROG_WXPROPGRID_NAME],
[
   AC_ARG_WITH(wxpropgrid,
      AC_HELP_STRING(
         [--with-wxpropgrid@<:@=NAME@:>@],
         [specify name for wxPropertyGrid library instead of wxcode_gtk2u_propgrid-2.8]
      ),
      [WXPROPGRID_NAME=${withval}],
      [WXPROPGRID_NAME=wxcode_gtk2u_propgrid-2.8]
   )
   if test $wx_config_major_version -lt 3; then
     PROPGRID_LIBS="-l$WXPROPGRID_NAME"
   else
     PROPGRID_LIBS=""
   fi
   AC_SUBST(WXPROPGRID_NAME)
   AC_SUBST(PROPGRID_LIBS)
])


# AX_PROG_WITH_PARAVER
# -----------
AC_DEFUN([AX_PROG_WITH_PARAVER],
[
   AC_ARG_WITH(paraver,
      AC_HELP_STRING(
         [--with-paraver@<:@=DIR@:>@],
         [specify paraver-kernel and paraver-api libraries base install directory. Default: /usr/local]
      ),
      [PARAVER_DIR=${withval}],
      [PARAVER_DIR=/usr/local]
   )
   AC_SUBST(PARAVER_DIR)
])

# AX_PROG_ENABLE_TRACING
# -----------
AC_DEFUN([AX_PROG_ENABLE_TRACING],
[
  AC_ARG_ENABLE(tracing,
    AC_HELP_STRING(
      [--enable-tracing],
      [Enable tracing generation. (Disabled by default)]
    ),
    [enable_tracing="${enableval}"],
    [enable_tracing="no"]
  )
  if test "${enable_tracing}" = "yes" ; then
    AC_DEFINE([TRACING_ENABLED], 1, [Tracing enabled by user.])
    CPPFLAGS="$CPPFLAGS -g"
    CXXFLAGS="$CXXFLAGS -g"
    CFLAGS="$CFLAGS -g"
  fi
])



# AX_PROG_ENABLE_OLD_PCFPARSER
# ----------------------------
AC_DEFUN([AX_PROG_ENABLE_OLD_PCFPARSER],
[
  AC_ARG_ENABLE(old_pcfparser,
    AC_HELP_STRING(
      [--enable-old-pcfparser],
      [Enable compilation with old pcfparser. (Disabled by default)]
    ),
    [enable_old_pcfparser="${enableval}"],
    [enable_old_pcfparser="no"]
  )
  if test "${enable_old_pcfparser}" = "yes" ; then
    AC_DEFINE([SET_OLD_PCFPARSER], 1, [Old pcfparser enabled by user.])
    PCFPARSER_CFLAGS="-DOLD_PCFPARSER -I../../common-files/pcfparser"
  else
    AC_DEFINE([SET_OLD_PCFPARSER], 0, [Old pcfparser enabled by user.])
    PCFPARSER_CFLAGS="-I../../common-files/pcfparser/libtools"
  fi
])



# AX_PROG_WITH_EXTRAE
# -----------
AC_DEFUN([AX_PROG_WITH_EXTRAE],
[
  AC_ARG_WITH(extrae,
    AC_HELP_STRING(
      [--with-extrae@<:@=DIR@:>@],
      [Specify extrae library base install directory. Default: /usr/local]
    ),
    [EXTRAE_DIR=${withval}],
    [EXTRAE_DIR=/usr/local]
  )
  if test "${enable_tracing}" = "yes" ; then
    CPPFLAGS="$CPPFLAGS -I$EXTRAE_DIR/include"
    CXXFLAGS="$CXXFLAGS -I$EXTRAE_DIR/include"
    CFLAGS="$CFLAGS -L$EXTRAE_DIR/include"
    LIBS="$LIBS -L$EXTRAE_DIR/lib"
    LDFLAGS="$LDFLAGS -L$EXTRAE_DIR/lib -lseqtrace"
  fi
])

# AX_PROG_ENABLE_PARALLEL
# -----------
AC_DEFUN([AX_PROG_ENABLE_PARALLEL],
[
  AC_ARG_ENABLE(parallel,
    AC_HELP_STRING(
      [--enable-parallel],
      [Enable parallel version. (Disabled by default)]
    ),
    [enable_parallel="${enableval}"],
    [enable_parallel="no"]
  )
  if test "${enable_parallel}" = "yes" ; then
    AC_DEFINE([PARALLEL_ENABLED], 1, [Parallel version enabled by user.])
    LDFLAGS="$LDFLAGS -fPIC"
  fi
])
