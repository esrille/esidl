SUBDIRS = . testsuite java cplusplus npapi

BUILT_SOURCES = lexer.cc parser.cc parser.h

bin_PROGRAMS = esidl

esidl_SOURCES = \
	include/cplusplus.h\
	include/cplusplusCall.h \
	include/cplusplusTemplate.h \
	include/esidl.h \
	include/cxx.h \
	include/expr.h \
	include/formatter.h \
	include/forward.h \
	include/info.h \
	include/java.h \
	include/reflect.h \
	include/utf.h \
	src/cplusplus.cpp \
	src/cxx.cpp \
	src/driver.cpp \
	src/esidl.cpp \
	src/java.cpp \
	src/lexer.ll \
	src/parser.yy \
	src/print.cpp \
	src/skeleton.cpp \
	src/template.cpp

esidl_CXXFLAGS = -D YYERROR_VERBOSE

AM_YFLAGS = -d -v -t

AM_CPPFLAGS = \
	-iquote $(srcdir)/include \
	-iquote $(srcdir)/src

EXTRA_DIST = \
	LICENSE \
	dom/COPYRIGHT.html \
	dom/cssomview.idl \
	dom/dom2css.idl \
	dom/dom2events.idl \
	dom/dom2ranges.idl \
	dom/dom2stylesheets.idl \
	dom/dom2traversal.idl \
	dom/dom2views.idl \
	dom/dom3core.idl \
	dom/dom3ls.idl \
	dom/dom3validation.idl \
	dom/dom3xpath.idl \
	dom/eventsource.idl \
	dom/file.idl \
	dom/html5.idl \
	dom/idl_scraper.pl \
	dom/NOTICE \
	dom/webgl.idl \
	dom/webstorage.idl \
	dom/websocket.idl \
	dom/wget_webidl_dom \
	dom/workers.idl \
	dom/xhr2.idl

clean-local:
	-rm -f $(BUILT_SOURCES) parser.output $(nobase_nodist_include_HEADERS)

dist-hook:
	-rm -f $(distdir)/lexer.cc $(distdir)/parser.cc $(distdir)/parser.h

