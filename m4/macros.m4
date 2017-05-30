# AX_PROG_WXPROPGRID_NAME
# -----------------------
AC_DEFUN([AX_PROG_WXPROPGRID_NAME],
[
   AC_ARG_WITH(wxpropgrid-name,
      AC_HELP_STRING(
         [--with-wxpropgrid-name@<:@=NAME@:>@],
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



# AX_PROG_WITH_WXPROPGRID_DIR
# -----------------------
AC_DEFUN([AX_PROG_WITH_WXPROPGRID_DIR],
[
   AC_ARG_WITH(wxpropgrid-dir,
      AC_HELP_STRING(
         [--with-wxpropgrid-dir@<:@=DIR@:>@],
         [specify wxpropgrid installation directory. Default: /usr/local]
      ),
      [WXPROPGRID_DIR=${withval}],
      [WXPROPGRID_DIR=""]
   )

   if test "x$WXPROPGRID_DIR" = "x" ; then
     WXPROPGRID_INCLUDE_DIR=""
     WXPROPGRID_LIBDIR=""
   else
     WXPROPGRID_INCLUDE_DIR="-I$WXPROPGRID_DIR/include"
     WXPROPGRID_LIBDIR="-L$WXPROPGRID_DIR/lib"
   fi

   AC_SUBST(WXPROPGRID_DIR)
   AC_SUBST(WXPROPGRID_INCLUDE_DIR)
   AC_SUBST(WXPROPGRID_LIBDIR)
])



# AX_PROG_WITH_PARAVER
# --------------------
AC_DEFUN([AX_PROG_WITH_PARAVER],
[
   AC_ARG_WITH(paraver,
      AC_HELP_STRING(
         [--with-paraver@<:@=DIR@:>@],
         [specify paraver-kernel and paraver-api libraries base install directory. Default: /usr/local]
      ),
      [PARAVER_DIR=${withval}],
      [PARAVER_DIR=${prefix}]
   )
   AC_SUBST(PARAVER_DIR)
])



# AX_PROG_WITH_PARAVER_LIBDIR
# ---------------------------
AC_DEFUN([AX_PROG_WITH_PARAVER_LIBDIR],
[
   AC_ARG_WITH(paraver-libdir,
      AC_HELP_STRING(
         [--with-paraver-libdir@<:@=DIR@:>@],
         [specify paraver-kernel and paraver-api libraries install directory. Default: /usr/local/lib]
      ),
      [PARAVER_LIBDIR=${withval}],
      [PARAVER_LIBDIR=${PARAVER_DIR}/lib]
   )
   if test "${PARAVER_LIBDIR##*/}" = "lib64" ; then
     PARAVER_RPATH_LIB="lib64"
   else
     PARAVER_RPATH_LIB="lib"
   fi

   AC_SUBST(PARAVER_LIBDIR)
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
    PCFPARSER_CFLAGS="-DOLD_PCFPARSER -I${PARAVER_DIR}/include/pcfparser/old"
  else
    AC_DEFINE([SET_OLD_PCFPARSER], 0, [Old pcfparser enabled by user.])
    PCFPARSER_CFLAGS="-I${PARAVER_DIR}/include/pcfparser"
  fi
])



# AX_PROG_ENABLE_OPENMP
# -----------------------
AC_DEFUN([AX_PROG_ENABLE_OPENMP],
[
  AC_ARG_ENABLE(openmp,
    AC_HELP_STRING(
      [--enable-openmp],
      [Enable OpenMP parallel version. (Disabled by default)]
    ),
    [enable_openmp="${enableval}"],
    [enable_openmp="no"]
  )
  if test "${enable_openmp}" = "yes" ; then
    AC_DEFINE([PARALLEL_ENABLED], 1, [Parallel version enabled by user.])
    CXXFLAGS="$CXXFLAGS -fopenmp"
    LDFLAGS="$LDFLAGS -fopenmp"
  fi
])



# AX_PROG_WITH_EXTRAE
# -------------------
AC_DEFUN([AX_PROG_WITH_EXTRAE],
[
  AC_REQUIRE([AX_PROG_ENABLE_OPENMP])

  AC_ARG_WITH(extrae,
    AC_HELP_STRING(
      [--with-extrae@<:@=DIR@:>@],
      [Specify extrae library base install directory. Default: /usr/local]
    ),
    [if test "x${withval}" = "xyes" ; then 
       EXTRAE_DIR="/usr/local"
     else
       EXTRAE_DIR=${withval}
     fi],
    [EXTRAE_DIR=""]
  )
  if test "x${EXTRAE_DIR}" != "x" ; then
    AC_MSG_NOTICE([Trying to detect Extrae installation in ${EXTRAE_DIR}])
    if test -d "${EXTRAE_DIR}/lib" && test -f "${EXTRAE_DIR}/lib/libseqtrace.so" ; then
      AC_MSG_NOTICE([Extrae installation found])
      CPPFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
      CXXFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
      CFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
      LIBS_EXTRAE="-L$EXTRAE_DIR/lib"
      if test "${enable_openmp_parallel}" = "yes" ; then
        LDFLAGS_EXTRAE="-L$EXTRAE_DIR/lib -lomptrace"
      else
        LDFLAGS_EXTRAE="-L$EXTRAE_DIR/lib -lseqtrace"
      fi
    else
      AC_MSG_WARN([No Extrae installation found in ${EXTRAE_DIR}])
    fi
  fi
])



# AX_PROG_ENABLE_PARALLEL
# -----------------------
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


