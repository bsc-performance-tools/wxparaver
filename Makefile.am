
SUBDIRS = \
          include \
          src \
          icons


install-data-hook:
	cp -f ./docs/dimemas-wrapper.sh $(DESTDIR)$(bindir)
	chmod a+x $(DESTDIR)$(bindir)/dimemas-wrapper.sh

	cp -f ./docs/prvstats-wrapper.sh $(DESTDIR)$(bindir)
	chmod a+x $(DESTDIR)$(bindir)/prvstats-wrapper.sh

	if test "x${MINGW_CPPFLAGS}" != "x" ; then \
		cp -f ./docs/wxparaver.bat $(DESTDIR)$(prefix) ; \
	fi


if ENABLE_DEPENDENCIES_AWARE_INSTALL
	cp -f ./docs/wxparaver $(DESTDIR)$(bindir)/wxparaver
  
	test "x${BOOST_LDFLAGS}" != "x" && \
		( cp  $(DESTDIR)$(bindir)/wxparaver  $(DESTDIR)$(bindir)/wxparaver.tmp && \
		  $(SED) "s|@inst_BOOST_LIBDIR@|${BOOST_LDFLAGS}:|g" < $(DESTDIR)$(bindir)/wxparaver.tmp > $(DESTDIR)$(bindir)/wxparaver ) || \
		( cp  $(DESTDIR)$(bindir)/wxparaver $(DESTDIR)$(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_BOOST_LIBDIR@||g" < $(DESTDIR)$(bindir)/wxparaver.tmp > $(DESTDIR)$(bindir)/wxparaver )
  
	test "x${WX_LIBS}" != "x" && \
		( cp  $(DESTDIR)$(bindir)/wxparaver  $(DESTDIR)$(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_WXWIDGETS_LIBDIR@|${WX_LIBS}:|g" < $(DESTDIR)$(bindir)/wxparaver.tmp > $(DESTDIR)$(bindir)/wxparaver ) || \
		( cp  $(DESTDIR)$(bindir)/wxparaver $(DESTDIR)$(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_WXWIDGETS_LIBDIR@||g" < $(DESTDIR)$(bindir)/wxparaver.tmp > $(DESTDIR)$(bindir)/wxparaver )
  
	test "x${OPENSSL_LDFLAGS}" != "x" && \
		( cp  $(DESTDIR)$(bindir)/wxparaver $(DESTDIR)$(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_LIBSSL_LIBDIR@|${OPENSSL_LDFLAGS}:|g"  < $(DESTDIR)$(bindir)/wxparaver.tmp > $(DESTDIR)$(bindir)/wxparaver ) || \
		( cp  $(DESTDIR)$(bindir)/wxparaver $(DESTDIR)$(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_LIBSSL_LIBDIR@||g" < $(DESTDIR)$(bindir)/wxparaver.tmp > $(DESTDIR)$(bindir)/wxparaver )

	cp  $(DESTDIR)$(bindir)/wxparaver $(DESTDIR)$(bindir)/wxparaver.tmp && \
	$(AWK) \
			'{  \
					getline current < "$(DESTDIR)$(bindir)/wxparaver.tmp" ; \
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
				} '	$(DESTDIR)$(bindir)/wxparaver.tmp > $(DESTDIR)$(bindir)/wxparaver
		
	( cp  $(DESTDIR)$(bindir)/wxparaver $(DESTDIR)$(bindir)/wxparaver.tmp && \
			$(SED) "s|@inst_LOGIN_NODE_DETECTION@|if [ ! \"$$\(hostname \| grep login\)x\" = \"x\" ]; then export OMP_NUM_THREADS=4 ; ulimit -Ss 128000 ; fi|g" < $(DESTDIR)$(bindir)/wxparaver.tmp > $(DESTDIR)$(bindir)/wxparaver )
	
	test -f $(DESTDIR)$(bindir)/wxparaver.tmp && rm -fv $(DESTDIR)$(bindir)/wxparaver.tmp
  
else
	$(SED) "s|@inst_BOOST_LIBDIR@||g ; \
          s|@inst_WXWIDGETS_LIBDIR@||g ; \
          s|@inst_LIBSSL_LIBDIR@||g ; \
					s|@inst_LOGIN_NODE_DETECTION@||g" < ./docs/wxparaver > $(DESTDIR)$(bindir)/wxparaver
endif
	
	
	chmod a+x $(DESTDIR)$(bindir)/wxparaver
	
	$(mkdir_p) $(DESTDIR)$(prefix)/share/
	$(mkdir_p) $(DESTDIR)$(prefix)/share/wxparaver-signals
	$(install_sh_PROGRAM) -m 775 ./docs/paraload.sig      $(DESTDIR)$(prefix)/share/wxparaver-signals
	$(install_sh_PROGRAM) -m 775 ./docs/paraload-v1.0.sig $(DESTDIR)$(prefix)/share/wxparaver-signals

	$(mkdir_p) $(DESTDIR)$(prefix)/share/doc
	$(mkdir_p) $(DESTDIR)$(prefix)/share/doc/wxparaver_help_contents/
	cp -fpR ./docs/wxparaver_help_contents/install/* $(DESTDIR)$(prefix)/share/doc/wxparaver_help_contents
	
		
	if test -f ./docs/workspaces.xml ; then \
		cp -f ./docs/workspaces.xml $(DESTDIR)$(prefix)/share/ ; \
	fi


uninstall-hook:
	test -f "$(DESTDIR)$(bindir)/dimemas-wrapper.sh"       && rm  -fv $(DESTDIR)$(bindir)/dimemas-wrapper.sh
	test -f "$(DESTDIR)$(bindir)/prvstats-wrapper.sh"      && rm  -fv $(DESTDIR)$(bindir)/prvstats-wrapper.sh
	test -f "$(DESTDIR)$(bindir)/wxparaver"                && rm  -fv $(DESTDIR)$(bindir)/wxparaver

	test -f "$(DESTDIR)$(prefix)/share/workspaces.xml"     && rm  -fv $(DESTDIR)$(prefix)/share/workspaces.xml
	test -d "$(DESTDIR)$(prefix)/share/wxparaver-signals"  && rm -rfv $(DESTDIR)$(prefix)/share/wxparaver-signals
	test -d "$(DESTDIR)$(prefix)/share/doc/wxparaver_help" && rm -rfv $(DESTDIR)$(prefix)/share/doc/wxparaver_help


dist-hook:
	$(mkdir_p) $(distdir)/docs
	cp -f ./docs/dimemas-wrapper.sh   $(distdir)/docs
	cp -f ./docs/prvstats-wrapper.sh  $(distdir)/docs
	cp -f ./docs/wxparaver            $(distdir)/docs
	cp -f ./docs/wxparaver.bat        $(distdir)/docs
  
	cp -f ./docs/paraload.sig         $(distdir)/docs
	cp -f ./docs/paraload-v1.0.sig    $(distdir)/docs

	if test -f ./docs/workspaces.xml ; then \
		cp -f ./docs/workspaces.xml  $(distdir)/docs ; \
	fi

	$(mkdir_p) $(distdir)/docs/wxparaver_help_contents
	cp -fpR ./docs/wxparaver_help_contents/install  $(distdir)/docs/wxparaver_help_contents

installcheck-local:
	test -f $(DESTDIR)$(bindir)/wxparaver && \
	echo "  [wxparaver][bin] Installed " || ( echo "  [wxparaver][bin] Check failed!!!" && exit 1 )

