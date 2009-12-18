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
