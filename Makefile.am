
bin_PROGRAMS = wxparaver.bin

wxparaver_bin_SOURCES = advancedsaveconfiguration.h \
                    advancedsaveconfiguration.cpp \
                    connection.cpp \
                    connection.h \
                    copypaste.cpp \
                    copypaste.h \
                    cutfilterdialog.cpp \
                    cutfilterdialog.h \
                    derivedtimelinedialog.cpp \
                    derivedtimelinedialog.h \
                    eventsselectiondialog.cpp \
                    eventsselectiondialog.h \
                    filebrowserbutton.cpp \
                    filebrowserbutton.h \
                    filedialogext.cpp \
                    filedialogext.h \
                    finddialog.cpp \
                    finddialog.h \
                    ghistogram.cpp \
                    ghistogram.h \
                    gtimeline.cpp \
                    gtimeline.h \
                    helpcontents.cpp \
                    helpcontents.h \
                    histogramdialog.cpp \
                    histogramdialog.h \
                    histotablebase.cpp \
                    histotablebase.h \
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
                    runscript.cpp \
                    runscript.h \
                    saveconfigurationdialog.cpp \
                    saveconfigurationdialog.h \
                    sequencedriver.cpp \
                    sequencedriver.h \
                    sessionsaver.cpp \
                    sessionsaver.h \
                    windows_tree.cpp \
                    windows_tree.h \
                    wxparaverapp.cpp \
                    wxparaverapp.h

noinst_HEADERS = \
              app_edit.xpm\
              arrow_down.xpm\
              arrow_left.xpm\
              arrow_right.xpm\
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
              file_browser.xpm\
              hide_cols.xpm\
              histo_color.xpm\
              histo_horvert.xpm\
              histo_zoom.xpm\
              inclusive.xpm\
              index.xpm\
              logoBSC.xpm\
              new_derived_window.xpm\
              new_histogram.xpm\
              new_window.xpm\
              open3d.xpm\
              opencontrol.xpm\
              opendata.xpm\
              openfiltered.xpm\
              run_script.xpm\
              semantic_color.xpm\
              table.xpm\
              timeline.xpm\
              window_properties.xpm

wxparaver_bin_LDADD = @LIBS@ @PROPGRID_LIBS@ -lparaver-kernel -lparaver-api
wxparaver_bin_CXXFLAGS = @CXXFLAGS@

 
install-data-hook:
	if test -f $(bindir)/dimemas-wrapper.sh ; then \
		cp -f $(bindir)/dimemas-wrapper.sh $(bindir)/old.dimemas-wrapper.sh ; \
	fi
	cp -f ./docs/dimemas-wrapper.sh $(bindir)
	chmod a+x $(bindir)/dimemas-wrapper.sh
	
	if test -f $(bindir)/stats-wrapper.sh ; then \
		cp -f $(bindir)/stats-wrapper.sh $(bindir)/old.stats-wrapper.sh ; \
	fi
	cp -f ./docs/stats-wrapper.sh $(bindir)
	chmod a+x $(bindir)/stats-wrapper.sh

	if test -f $(bindir)/wxparaver ; then \
		cp -f $(bindir)/wxparaver $(bindir)/old.wxparaver ; \
	fi
	cp -f ./docs/wxparaver          $(bindir)
	$(SED) "s|@inst_PARAVER_HOME@|$(prefix)|g" < ./docs/wxparaver > $(bindir)/wxparaver
	chmod a+x $(bindir)/wxparaver
	cp -f ./docs/wxparaver.mn3   $(bindir)
	cp -f ./docs/wxparaver.mt   $(bindir)
	chmod a+x $(bindir)/wxparaver.mn3
	chmod a+x $(bindir)/wxparaver.mt
	
	$(mkdir_p) $(prefix)/share/
	$(mkdir_p) $(prefix)/share/wxparaver-signals
	$(install_sh_PROGRAM) -m 775 ./docs/paraload.sig      $(prefix)/share/wxparaver-signals
	$(install_sh_PROGRAM) -m 775 ./docs/paraload-v1.0.sig $(prefix)/share/wxparaver-signals

	$(mkdir_p) $(prefix)/share/doc
	$(mkdir_p) $(prefix)/share/doc/wxparaver_help
	$(mkdir_p) $(prefix)/share/doc/wxparaver_help/1.quick_reference
	$(mkdir_p) $(prefix)/share/doc/wxparaver_help/1.quick_reference/img
	cp -f ./docs/quick_reference/wxparaver.quickreference.html $(prefix)/share/doc/wxparaver_help/1.quick_reference/index.html
	cp -f ./docs/quick_reference/img/*.png                     $(prefix)/share/doc/wxparaver_help/1.quick_reference/img
	
	if test -f ./docs/workspaces.xml ; then \
		cp -f ./docs/workspaces.xml $(prefix)/share/ ; \
	fi


dist-hook:
	$(mkdir_p) $(distdir)/docs
	cp -f ./docs/dimemas-wrapper.sh $(distdir)/docs
	cp -f ./docs/stats-wrapper.sh   $(distdir)/docs
	cp -f ./docs/wxparaver          $(distdir)/docs
	cp -f ./docs/wxparaver.mn3      $(distdir)/docs
	cp -f ./docs/wxparaver.mt       $(distdir)/docs

	cp -f ./docs/paraload.sig       $(distdir)/docs
	cp -f ./docs/paraload-v1.0.sig  $(distdir)/docs

	if test -f ./docs/workspaces.xml ; then \
		cp -f ./docs/workspaces.xml  $(distdir)/docs ; \
	fi

	$(mkdir_p) $(distdir)/docs/quick_reference
	$(mkdir_p) $(distdir)/docs/quick_reference/img
	cp -f ./docs/quick_reference/wxparaver.quickreference.html $(distdir)/docs/quick_reference
	cp -f ./docs/quick_reference/img/*.png                     $(distdir)/docs/quick_reference/img

installcheck-local:
	test -f $(bindir)/wxparaver && \
	echo "  [wxparaver][bin] Installed " || ( echo "  [wxparaver][bin] Check failed!!!" && exit 1 )

