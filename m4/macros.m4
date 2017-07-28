# AX_PROG_WXPROPGRID_NAME
# -----------------------
AC_DEFUN([AX_PROG_WXPROPGRID_NAME],
[
   AC_ARG_WITH(wxpropgrid-name,
      AC_HELP_STRING(
         [--with-wxpropgrid-name=NAME],
         [specify NAME for wxPropertyGrid library (default: wxcode_gtk2u_propgrid-2.8, the GTK2 Unicode 2.8 version); if wxWidgets version is 3.0 or greater, this flag is overriden]
      ),
      [if test "${withval}" = "yes" ; then 
          AC_MSG_ERROR([missing NAME parameter for --with-wxpropgrid-name flag])
       else
          WXPROPGRID_NAME=${withval}
       fi],
      [WXPROPGRID_NAME=wxcode_gtk2u_propgrid-2.8]
   )
   
   if test $wx_config_major_version -lt 3; then
      PROPGRID_LIBS="-l$WXPROPGRID_NAME"
   else
      AC_MSG_NOTICE([wxWidgets 3.x detected. --with-wxpropgrid-name overriden.])
      PROPGRID_LIBS=""
   fi
   
   AC_SUBST(WXPROPGRID_NAME)
   AC_SUBST(PROPGRID_LIBS)
])



# AX_PROG_WITH_WXPROPGRID_DIR
# ---------------------------
AC_DEFUN([AX_PROG_WITH_WXPROPGRID_DIR],
[
   AC_ARG_WITH(wxpropgrid-dir,
      AC_HELP_STRING(
         [--with-wxpropgrid-dir@<:@=DIR@:>@],
         [specify wxpropgrid installation DIR (default: none; if no parameter DIR given: /usr/local ); if wxWidgets version is 3.0 or greater, this flag is overriden ]
      ),
      [if test "${withval}" = "yes" ; then 
          WXPROPGRID_DIR="/usr/local"
       else
          WXPROPGRID_DIR=${withval}
       fi],
      [WXPROPGRID_DIR=""]
   )
   
   WXPROPGRID_INCLUDE_DIR=""
   WXPROPGRID_LIBDIR=""
   
   if test $wx_config_major_version -lt 3 ; then
      if test "$WXPROPGRID_DIR" != "yes" ; then
         WXPROPGRID_INCLUDE_DIR="-I$WXPROPGRID_DIR/include"
        
         if test -d "${WXPROPGRID_DIR}/lib" ; then
            WXPROPGRID_LIBDIR="-L${WXPROPGRID_DIR}/lib"
         elif test -d "${WXPROPGRID_DIR}/lib64" ; then
            WXPROPGRID_LIBDIR="-L${WXPROPGRID_DIR}/lib64"
         else
            AC_MSG_ERROR([--with-wxpropgrid-dir unable to find ${WXPROPGRID_DIR}/lib or ${WXPROPGRID_DIR}/lib64])
         fi
      fi
   else
      AC_MSG_NOTICE([wxWidgets 3.x detected. --with-wxpropgrid-dir overriden.])
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
         [specify paraver install directory (default: PREFIX; if no parameter DIR given: /usr/local)]
      ),
      [if test "${withval}" = "yes" ; then 
          PARAVER_DIR="/usr/local"
       else
          PARAVER_DIR=${withval}
       fi],
      [PARAVER_DIR=${prefix}]
   )

   if test "x$PARAVER_PACKAGE_BUILD" = "xyes" ; then
   
      PARAVER_RPATH_LIB="${libdir##*/}/paraver-kernel"
      AC_MSG_NOTICE([paraver-package build: future paraver-kernel in ${PARAVER_DIR}/${PARAVER_RPATH_LIB}])
      
   else
   
      if test -d "${PARAVER_DIR}/lib" && \
         test -f "${PARAVER_DIR}/lib/libparaver-api.so" ; then
         PARAVER_RPATH_LIB="lib"
      elif test -d "${PARAVER_DIR}/lib64" && \
         test -f "${PARAVER_DIR}/lib64/libparaver-api.so" ; then
         PARAVER_RPATH_LIB="lib64"
      elif test -d "${PARAVER_DIR}/lib/paraver-kernel" && \
         test -f "${PARAVER_DIR}/lib/paraver-kernel/libparaver-api.so" ; then
         PARAVER_RPATH_LIB="lib/paraver-kernel"
      elif test -d "${PARAVER_DIR}/lib64/paraver-kernel" && \
         test -f "${PARAVER_DIR}/lib64/paraver-kernel/libparaver-api.so" ; then
         PARAVER_RPATH_LIB="lib64/paraver-kernel"
      else
         AC_MSG_ERROR([cannot find any paraver installation in ${PARAVER_DIR}])
      fi

      AC_MSG_NOTICE([paraver-kernel installation detected in ${PARAVER_DIR}/${PARAVER_RPATH_LIB}])

   fi  

   PARAVER_LIBDIR=${PARAVER_DIR}/${PARAVER_RPATH_LIB}

#   AC_SUBST(PARAVER_RPATH_LIB)
#   AC_SUBST(PARAVER_LIBDIR)
#   AC_SUBST(PARAVER_DIR)
])



# AX_PROG_ENABLE_OLD_PCFPARSER
# ----------------------------
AC_DEFUN([AX_PROG_ENABLE_OLD_PCFPARSER],
[
   AC_ARG_ENABLE(old_pcfparser,
      AC_HELP_STRING(
         [--enable-old-pcfparser],
         [enable compilation with old pcfparser (default: disabled)]
      ),
      [enable_old_pcfparser="${enableval}"],
      [enable_old_pcfparser="no"]
   )
   
   if test "${enable_old_pcfparser}" = "yes" ; then
      AC_MSG_NOTICE([Using old pcfparser.])
      AC_DEFINE([OLD_PCFPARSER], 1, [Old pcfparser enabled by user.])
      PCFPARSER_CFLAGS="-I${PARAVER_DIR}/include/pcfparser/old"
   else
      PCFPARSER_CFLAGS="-I${PARAVER_DIR}/include/pcfparser"
   fi
   
   AM_CONDITIONAL(MAKE_OLD_PCFPARSER, test "${enable_old_pcfparser}" = "yes" )
])



# AX_PROG_ENABLE_OMPSS
# --------------------
AC_DEFUN([AX_PROG_ENABLE_OMPSS],
[
   AC_ARG_ENABLE(ompss,
      AC_HELP_STRING(
         [--enable-ompss],
         [enable OmpSs parallel version (default: disabled)]
      ),
      [enable_ompss="${enableval}"],
      [enable_ompss="no"]
   )
   if test "${enable_ompss}" = "yes" ; then
      AC_DEFINE([PARALLEL_ENABLED], 1, [OmpSs parallel version enabled by user.])
      #CXXFLAGS="$CXXFLAGS --instrument"
      #LDFLAGS="$LDFLAGS --instrument -fPIC"
      LDFLAGS="$LDFLAGS -fPIC"
   fi
])



# AX_PROG_ENABLE_OPENMP
# ---------------------
AC_DEFUN([AX_PROG_ENABLE_OPENMP],
[
   AC_ARG_ENABLE(openmp,
      AC_HELP_STRING(
         [--enable-openmp],
         [enable OpenMP parallel version (default: disabled)]
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
         [specify extrae library base install directory. (default: none; if no parameter DIR given: /usr/local) ]
      ),
      [if test "${withval}" = "yes" ; then 
          EXTRAE_DIR="/usr/local"
       else
          EXTRAE_DIR=${withval}
       fi],
      [EXTRAE_DIR=""]
   )
   
   if test "x${EXTRAE_DIR}" != "x" ; then
      AC_MSG_NOTICE([Trying to detect Extrae installation in ${EXTRAE_DIR}])
      if test -d "${EXTRAE_DIR}/lib" && test -f "${EXTRAE_DIR}/lib/libseqtrace.so" ; then
         AC_MSG_NOTICE([Extrae installation found in ${EXTRAE_DIR}/lib])
         CPPFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         CXXFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         CFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         LIBS_EXTRAE="-L$EXTRAE_DIR/lib"
         if test "${enable_openmp_parallel}" = "yes" ; then
            LDFLAGS_EXTRAE="${LIBS_EXTRAE} -lomptrace"
         else
            LDFLAGS_EXTRAE="${LIBS_EXTRAE} -lseqtrace"
         fi
      elif test -d "${EXTRAE_DIR}/lib64" && test -f "${EXTRAE_DIR}/lib64/libseqtrace.so" ; then
         AC_MSG_NOTICE([Extrae installation found in ${EXTRAE_DIR}/lib64])
         CPPFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         CXXFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         CFLAGS_EXTRAE="-g -I$EXTRAE_DIR/include"
         LIBS_EXTRAE="-L$EXTRAE_DIR/lib64"
         if test "${enable_openmp_parallel}" = "yes" ; then
            LDFLAGS_EXTRAE="${LIBS_EXTRAE} -lomptrace"
         else
            LDFLAGS_EXTRAE="${LIBS_EXTRAE} -lseqtrace"
         fi
      else
         AC_MSG_WARN([No Extrae installation found in ${EXTRAE_DIR}])
      fi
   fi
])



# AX_PROG_WITH_OTF2
# -----------------
AC_DEFUN([AX_PROG_WITH_OTF2],
[
   AC_ARG_WITH(otf2,
      AC_HELP_STRING(
         [--with-otf2@<:@=DIR@:>@],
         [specify OTF2 base install directory (default: none; if no DIR parameter given: /usr/local)]
      ),
      [if test "${withval}" = "yes" ; then 
          OTF2_DIR="/usr/local"
       else
          OTF2_DIR=${withval}
       fi],
      [OTF2_DIR=""]
   )
  
   if test "x${OTF2_DIR}" != "x" ; then
      CPPFLAGS_OTF2="-I$OTF2_DIR/include"
      CXXFLAGS_OTF2="-I$OTF2_DIR/include"
      CFLAGS_OTF2="-I$OTF2_DIR/include"
      LIBS_OTF2="-L$OTF2_DIR/lib"
      LDFLAGS_OTF2="-L$OTF2_DIR/lib -lotf2"
   fi     
   AM_CONDITIONAL([WANT_OTF2PRV], [test "x${OTF2_DIR}" != "x"] )

   AC_SUBST(OTF2_DIR)
])

