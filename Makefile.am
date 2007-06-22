BUILT_SOURCES = lexer.cc parser.cc parser.h

bin_PROGRAMS = esidl reflect

esidl_SOURCES = src/lexer.ll src/parser.yy src/esidl.cpp src/guid.c \
                include/esidl.h \
                src/header.cpp include/header.h \
		src/ird.cpp include/binaryStream.h

esidl_CXXFLAGS = -D YYERROR_VERBOSE

reflect_SOURCES = src/reflect.cpp

AM_YFLAGS = -d -v -t

AM_CPPFLAGS = \
	-I$(srcdir)/include \
	-I$(srcdir)/src \
	-I- \
	-I$(srcdir)/../include

clean-local:
	-rm -f $(BUILT_SOURCES) parser.output

dist-hook:
	-rm -f $(distdir)/lexer.cc $(distdir)/parser.cc $(distdir)/parser.h
