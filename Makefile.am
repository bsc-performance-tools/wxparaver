
SUBDIRS = \
          include \
          src \
          icons

 
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


uninstall-hook:
	test -f "$(bindir)/dimemas-wrapper.sh"       && rm  -fv $(bindir)/dimemas-wrapper.sh
	test -f "$(bindir)/old.dimemas-wrapper.sh"   && rm  -fv $(bindir)/old.dimemas-wrapper.sh
	test -f "$(bindir)/stats-wrapper.sh"         && rm  -fv $(bindir)/stats-wrapper.sh
	test -f "$(bindir)/old.stats-wrapper.sh"     && rm  -fv $(bindir)/old.stats-wrapper.sh
	test -f "$(bindir)/wxparaver"                && rm  -fv $(bindir)/wxparaver
	test -f "$(bindir)/old.wxparaver"            && rm  -fv $(bindir)/old.wxparaver
	test -f "$(bindir)/wxparaver.mn3"            && rm  -fv $(bindir)/wxparaver.mn3
	test -f "$(bindir)/wxparaver.mt"             && rm  -fv $(bindir)/wxparaver.mt

	test -f "$(prefix)/share/workspaces.xml"     && rm  -fv $(prefix)/share/workspaces.xml
	test -d "$(prefix)/share/wxparaver-signals"  && rm -rfv $(prefix)/share/wxparaver-signals
	test -d "$(prefix)/share/doc/wxparaver_help" && rm -rfv $(prefix)/share/doc/wxparaver_help


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

