
bin_PROGRAMS = wxparaver

wxparaver_SOURCES = copypaste.cpp \
                    copypaste.h \
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
                    saveconfigurationdialog.cpp \
                    saveconfigurationdialog.h \
                    windows_tree.cpp \
                    windows_tree.h \
                    wxparaverapp.cpp \
                    wxparaverapp.h

noinst_HEADERS = \
              caution.xpm\
              delete.xpm\
              hide_cols.xpm\
              histo_color.xpm\
              histo_horvert.xpm\
              histo_zoom.xpm\
              new_derived_window.xpm\
              new_histogram.xpm\
              new_window.xpm\
              open3d.xpm\
              opencontrol.xpm\
              opendata.xpm\
              openfiltered.xpm\
              table.xpm\
              timeline.xpm

wxparaver_LDADD = @LIBS@ -lwxcode_gtk2u_propgrid-2.8 -lparaver-kernel -lparaver-api
wxparaver_CXXFLAGS = @CXXFLAGS@

