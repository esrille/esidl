lib_LTLIBRARIES = libnpessample.la

AM_CPPFLAGS = \
	-fPIC -fshort-wchar \
	-I $(srcdir) \
	-I ../include \
	-I ../include/w3c \
	-I $(srcdir)/../include \
	-I $(srcdir)/../../include \
	-g \  
	-DXP_UNIX

AM_LDFLAGS = -L.. -ldl -lesnpapi

cpp_source_files = \
	np_entry.cpp \
	npn_gate.cpp \
	npp_gate.cpp \
	npp_sample.cpp \
	sample.cpp

header_files = \
	sample.h

libnpessample_la_SOURCES = $(cpp_source_files) $(header_files)

EXTRA_DIST = \
	sample.html

