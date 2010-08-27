
bin_PROGRAMS = wxparaver

wxparaver_SOURCES = connection.cpp \
                    connection.h \
                    copypaste.cpp \
                    copypaste.h \
                    cutfilterdialog.cpp \
                    cutfilterdialog.h \
                    derivedtimelinedialog.cpp \
                    derivedtimelinedialog.h \
                    ghistogram.cpp \
                    ghistogram.h \
                    gtimeline.cpp \
                    gtimeline.h \
                    histogramdialog.cpp \
                    histogramdialog.h \
                    paravermain.cpp \
                    paravermain.h \
                    pg_extraprop.cpp \
                    pg_extraprop.h \
                    pg_util.cpp \
                    pg_util.h \
                    popupmenu.cpp \
                    popupmenu.h \
                    preferencesdialog.cpp \
                    preferencesdialog.h \
                    rowsselectiondialog.cpp \
                    rowsselectiondialog.h \
                    saveconfigurationdialog.cpp \
                    saveconfigurationdialog.h \
                    windows_tree.cpp \
                    windows_tree.h \
                    wxparaverapp.cpp \
                    wxparaverapp.h

noinst_HEADERS = \
              arrow_down.xpm\
              arrow_up.xpm\
              caution.xpm\
              cut_trace.xpm\
              delete.xpm\
              derived_add.xpm\
              derived_controlled_add.xpm\
              derived_controlled_clear_by.xpm\
              derived_controlled_maximum.xpm\
              derived_different.xpm\
              derived_divide.xpm\
              derived_maximum.xpm\
              derived_minimum.xpm\
              derived_product.xpm\
              derived_substract.xpm\
              hide_cols.xpm\
              histo_color.xpm\
              histo_horvert.xpm\
              histo_zoom.xpm\
              inclusive.xpm\
              new_derived_window.xpm\
              new_histogram.xpm\
              new_window.xpm\
              open3d.xpm\
              opencontrol.xpm\
              opendata.xpm\
              openfiltered.xpm\
              semantic_color.xpm\
              table.xpm\
              timeline.xpm

wxparaver_LDADD = @LIBS@ -l@WXPROPGRID_NAME@ -lparaver-kernel -lparaver-api
wxparaver_CXXFLAGS = @CXXFLAGS@

