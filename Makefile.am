
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
	
	
if ENABLE_DEPENDENCIES_AWARE_INSTALL
	cp -f ./docs/wxparaver $(bindir)/wxparaver
  
	test "x${BOOST_LDFLAGS}" != "x" && \
		( cp  $(bindir)/wxparaver  $(bindir)/wxparaver.tmp && \
		  $(SED) "s|@inst_BOOST_LIBDIR@|${BOOST_LDFLAGS}:|g" < $(bindir)/wxparaver.tmp > $(bindir)/wxparaver ) || \
		( cp  $(bindir)/wxparaver $(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_BOOST_LIBDIR@||g" < $(bindir)/wxparaver.tmp > $(bindir)/wxparaver )
  
	test "x${WX_LIBS}" != "x" && \
		( cp  $(bindir)/wxparaver  $(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_WXWIDGETS_LIBDIR@|${WX_LIBS}:|g" < $(bindir)/wxparaver.tmp > $(bindir)/wxparaver ) || \
		( cp  $(bindir)/wxparaver $(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_WXWIDGETS_LIBDIR@||g" < $(bindir)/wxparaver.tmp > $(bindir)/wxparaver )
  
	test "x${WXPROPGRID_LIBDIR}" != "x" && \
		( cp  $(bindir)/wxparaver $(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_WXPROPGRID_LIBDIR@|${WXPROPGRID_LIBDIR}:|g"  < $(bindir)/wxparaver.tmp > $(bindir)/wxparaver ) || \
		( cp  $(bindir)/wxparaver $(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_WXPROPGRID_LIBDIR@||g" < $(bindir)/wxparaver.tmp > $(bindir)/wxparaver )

	cp  $(bindir)/wxparaver $(bindir)/wxparaver.tmp && \
	$(AWK) \
			'{  \
					getline current < "$(bindir)/wxparaver.tmp" ; \
					if (substr(current,0,17) != "LD_LIBRARY_PATH=\"") \
						print current ; \
					else \
					{ \
					  tmpline = "LD_LIBRARY_PATH=\"" ; \
						split(substr(current,18), tokens, ":") ; \
						for (i=1; i<=length(tokens); i++) \
						{ \
							if (substr(tokens[i],0,2) == "-L" ) \
							{ \
								split(tokens[i], tokens2, " ") ; \
								tmpline = tmpline substr(tokens2[1],3) ; \
							} \
							else \
							{ \
								tmpline = tmpline tokens[i] ; \
							} \
							if (i < length(tokens)) \
							{ \
								tmpline = tmpline ":" ; \
							} \
						} \
						print tmpline ; \
					} \
				} '	$(bindir)/wxparaver.tmp > $(bindir)/wxparaver
		
	test -f $(bindir)/wxparaver.tmp && rm -fv $(bindir)/wxparaver.tmp
  
else
	$(SED) "s|@inst_BOOST_LIBDIR@||g ; \
          s|@inst_WXWIDGETS_LIBDIR@||g ; \
          s|@inst_WXPROPGRID_LIBDIR@||g " < ./docs/wxparaver > $(bindir)/wxparaver
endif
	
	
	chmod a+x $(bindir)/wxparaver
	
	$(mkdir_p) $(prefix)/share/
	$(mkdir_p) $(prefix)/share/wxparaver-signals
	$(install_sh_PROGRAM) -m 775 ./docs/paraload.sig      $(prefix)/share/wxparaver-signals
	$(install_sh_PROGRAM) -m 775 ./docs/paraload-v1.0.sig $(prefix)/share/wxparaver-signals

	$(mkdir_p) $(prefix)/share/doc
	$(mkdir_p) $(prefix)/share/doc/wxparaver_help_contents/
	cp -fpR ./docs/wxparaver_help_contents/install/* $(prefix)/share/doc/wxparaver_help_contents
	
		
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

	$(mkdir_p) $(distdir)/docs/wxparaver_help_contents
	cp -fpR ./docs/wxparaver_help_contents/install  $(distdir)/docs/wxparaver_help_contents

installcheck-local:
	test -f $(bindir)/wxparaver && \
	echo "  [wxparaver][bin] Installed " || ( echo "  [wxparaver][bin] Check failed!!!" && exit 1 )

