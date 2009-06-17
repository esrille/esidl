SUBDIRS = . testsuite

BUILT_SOURCES = lexer.cc parser.cc parser.h

bin_PROGRAMS = esidl esidl2 entdump

esidl_SOURCES = src/driver.cpp

esidl2_SOURCES = es/ent.h \
	src/lexer.ll src/parser.yy src/esidl.cpp include/esidl.h \
	include/utf.h include/formatter.h \
	src/print.cpp src/cxx.cpp \
	src/ent.cpp src/skeleton.cpp \
	include/expr.h include/string.h include/cplusplus.h

esidl2-cxx.$(OBJEXT) : Makefile

esidl2_CXXFLAGS = -D YYERROR_VERBOSE

entdump_SOURCES = src/entdump.cpp

AM_YFLAGS = -d -v -t

AM_CPPFLAGS = \
	-I$(srcdir)/include \
	-I$(srcdir)/src \
	-I- \
	-I$(srcdir)

clean-local:
	-rm -f $(BUILT_SOURCES) parser.output

dist-hook:
	-rm -f $(distdir)/lexer.cc $(distdir)/parser.cc $(distdir)/parser.h

