#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(wxparaver, 4.6.1, tools@bsc.es)
AC_CONFIG_SRCDIR([wxparaverapp.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_SED

# Initialize automake
AM_INIT_AUTOMAKE

# Checks for libraries.
AM_OPTIONS_WXCONFIG
ifdef([WX_CONFIG_CHECK],
      [WX_CONFIG_CHECK([3.0.0],[wxWin=1],,[std,aui,propgrid,richtext])],
      [AM_PATH_WXCONFIG([2.8.0],[wxWin=1])]
)
if test "$wxWin" != 1; then
  	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi


AX_BOOST_BASE(1.36)
AX_BOOST_SERIALIZATION
AX_PROG_WXPROPGRID_NAME
AX_PROG_WITH_PARAVER
 
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([long, long long, char, int, short int])
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(short int)
AC_CHECK_TYPES([int64_t, uint64_t, int8_t, uint8_t, int32_t, uint32_t, int16_t, uint16_t])

# Checks for library functions.
AC_CHECK_FUNCS([floor rint])

# Optional features
AX_PROG_ENABLE_TRACING
AX_PROG_WITH_EXTRAE
AX_PROG_ENABLE_PARALLEL

AX_PROG_ENABLE_OLD_PCFPARSER
#AM_CONDITIONAL(MAKE_OLD_PCFPARSER, test "${enable_old_pcfparser}" = "yes" )

CPPFLAGS="-I./ -I../common-files/ $PCFPARSER_CFLAGS -I$PARAVER_DIR/include $CPPFLAGS $WX_CPPFLAGS $BOOST_CPPFLAGS"
CXXFLAGS="-I./ -I../common-files/ $PCFPARSER_CFLAGS -I$PARAVER_DIR/include $CXXFLAGS $WX_CXXFLAGS_ONLY $BOOST_CPPFLAGS"
CFLAGS="$CFLAGS $PCFPARSER_CFLAGS $WX_CFLAGS_ONLY"
if test -d "$PARAVER_DIR/lib/"; then
  LIBS="$LIBS $WX_LIBS -L$PARAVER_DIR/lib/paraver-kernel"
  LDFLAGS="$LDFLAGS $BOOST_LDFLAGS $BOOST_SERIALIZATION_LIB -Wl,-rpath -Wl,../lib/paraver-kernel"
else if test -d "$PARAVER_DIR/lib64/"; then 
  LIBS="$LIBS $WX_LIBS -L$PARAVER_DIR/lib64/paraver-kernel"
  LDFLAGS="$LDFLAGS $BOOST_LDFLAGS $BOOST_SERIALIZATION_LIB -Wl,-rpath -Wl,../lib64/paraver-kernel"
  fi
fi
#LDFLAGS="$LDFLAGS $BOOST_LDFLAGS $BOOST_SERIALIZATION_LIB -Wl,-rpath=../lib/ptools_common_files -Wl,-rpath=../lib/paraver-kernel -Wl,-rpath=../lib/wxparaver"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
