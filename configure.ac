#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(wxparaver, 4.8.0, tools@bsc.es)
AC_CONFIG_SRCDIR([src/wxparaverapp.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_SED

# Initialize automake
AM_INIT_AUTOMAKE([tar-ustar])

# Checks for libraries.
AM_OPTIONS_WXCONFIG
ifdef([WX_CONFIG_CHECK],
      [WX_CONFIG_CHECK([3.0.0],[wxWin=1],[wxWin=0],[std,propgrid,aui,richtext])])
if test "$wxWin" != 1; then
  ifdef([WX_CONFIG_CHECK],
        [WX_CONFIG_CHECK([2.8.0],[wxWin=1],[wxWin=0],[])])
fi


if test "$wxWin" != 1; then
  	AC_MSG_WARN([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

test "x$prefix" = xNONE && prefix=$ac_default_prefix

AX_BOOST_BASE(1.36)
AX_BOOST_SERIALIZATION
AX_BOOST_DATE_TIME
AX_PROG_ENABLE_DEPENDENCIES_AWARE_INSTALL
AX_PROG_WITH_WXPROPGRID_DIR
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
AX_PROG_ENABLE_OMPSS
AX_PROG_ENABLE_OPENMP
AX_PROG_WITH_EXTRAE
AX_PROG_ENABLE_OLD_PCFPARSER

CPPFLAGS="-fPIC -I./ -I../include/ -I../icons $PCFPARSER_CFLAGS -I$PARAVER_DIR/include $CPPFLAGS $CPPFLAGS_EXTRAE $WX_CPPFLAGS $WXPROPGRID_INCLUDE_DIR $BOOST_CPPFLAGS -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG -O2 -g"
CXXFLAGS="-fPIC -I./ -I../include/ -I../icons $PCFPARSER_CFLAGS -I$PARAVER_DIR/include $CXXFLAGS $CXXFLAGS_EXTRAE $WX_CXXFLAGS_ONLY $WXPROPGRID_INCLUDE_DIR $BOOST_CPPFLAGS -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG -O2 -g"
CFLAGS="$CFLAGS $CFLAGS_EXTRAE $PCFPARSER_CFLAGS $WX_CFLAGS_ONLY -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG -O2 -g"
LIBS="$LIBS $WX_LIBS $WXPROPGRID_LIBDIR -L$PARAVER_LIBDIR"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS $BOOST_SERIALIZATION_LIB $BOOST_DATE_TIME_LIB -Wl,-rpath -Wl,../$PARAVER_RPATH_LIB $LDFLAGS_EXTRAE"

AC_CONFIG_FILES([Makefile src/Makefile include/Makefile icons/Makefile])
AC_OUTPUT
