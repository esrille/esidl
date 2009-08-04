SUBDIRS = . testsuite npapi

BUILT_SOURCES = lexer.cc parser.cc parser.h

bin_PROGRAMS = esidl

esidl_SOURCES = \
	include/cplusplus.h\
	include/esidl.h \
	include/expr.h \
	include/formatter.h \
	include/forward.h \
	include/info.h \
	include/reflect.h \
	include/utf.h \
	src/cxx.cpp \
	src/driver.cpp \
	src/esidl.cpp \
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
	LICENSE

clean-local:
	-rm -f $(BUILT_SOURCES) parser.output $(nobase_nodist_include_HEADERS)

dist-hook:
	-rm -f $(distdir)/lexer.cc $(distdir)/parser.cc $(distdir)/parser.h

