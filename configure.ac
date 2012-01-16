#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(wxparaver, 4.3.0, tools@bsc.es)
AC_CONFIG_SRCDIR([wxparaverapp.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Initialize automake
AM_INIT_AUTOMAKE

# Checks for libraries.
AM_OPTIONS_WXCONFIG
reqwx=2.8.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
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
  LIBS="$LIBS -L$EXTRAE_LIBS/lib"
  LDFLAGS="$LDFLAGS -lseqtrace"
fi


CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS $BOOST_CPPFLAGS -I./ -I../common-files/ -I$PARAVER_DIR/include"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY $BOOST_CPPFLAGS -I./ -I../common-files/ -I$PARAVER_DIR/include"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS -L$PARAVER_DIR/lib/paraver-kernel"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS $BOOST_SERIALIZATION_LIB -Wl,-rpath -Wl,../lib/paraver-kernel"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
