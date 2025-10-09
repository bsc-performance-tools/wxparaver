#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(wxparaver, 4.12.0, tools@bsc.es)
AC_CONFIG_SRCDIR([src/wxparaverapp.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AX_PROG_WITH_DEBUG_LEVEL
AX_PROG_ENABLE_WINDOWS_DEBUG_CONSOLE

# Init for macro AX_PROG_WITH_DEBUG
AM_CFLAGS="$CPPFLAGS_DEBUG_LEVEL $AM_CFLAGS"
AM_CXXFLAGS="$CPPFLAGS_DEBUG_LEVEL $AM_CXXFLAGS"

: ${CFLAGS=""}
: ${CXXFLAGS=""}

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
AX_BOOST_SYSTEM
AX_CHECK_OPENSSL
AX_PROG_ENABLE_DEPENDENCIES_AWARE_INSTALL
AX_PROG_WITH_PARAVER
AX_PROG_ENABLE_MINGW

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


AM_CPPFLAGS="-fPIC -I./ -I../include/ -I../icons $PCFPARSER_CFLAGS -I$PARAVER_DIR/include $AM_CPPFLAGS $CPPFLAGS_EXTRAE $WX_CPPFLAGS $BOOST_CPPFLAGS $OPENSSL_INCLUDES -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG $OPENMP_CXXFLAGS"
AM_CXXFLAGS="-fPIC -I./ -I../include/ -I../icons $PCFPARSER_CFLAGS -I$PARAVER_DIR/include $AM_CXXFLAGS $CXXFLAGS_EXTRAE $WX_CXXFLAGS_ONLY $BOOST_CPPFLAGS $OPENSSL_INCLUDES -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG $OPENMP_CXXFLAGS"
AM_CFLAGS="$AM_CFLAGS $CFLAGS_EXTRAE $PCFPARSER_CFLAGS $WX_CFLAGS_ONLY $OPENSSL_INCLUDES -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG $OPENMP_CXXFLAGS"
AM_LDFLAGS="$AM_LDFLAGS $OPENSSL_LDFLAGS -L$PARAVER_LIBDIR $BOOST_LDFLAGS -Wl,-rpath -Wl,../$PARAVER_RPATH_LIB $LDFLAGS_EXTRAE $OPENMP_LDFLAGS"
LIBS="$LIBS $WX_LIBS $BOOST_SERIALIZATION_LIB $BOOST_DATE_TIME_LIB $BOOST_SYSTEM_LIB $OPENSSL_LIBS $MINGW_LIBS"

AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

AC_CONFIG_FILES([Makefile src/Makefile include/Makefile icons/Makefile])
AC_OUTPUT
