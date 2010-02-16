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
   AC_SUBST(WXPROPGRID_NAME)
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

