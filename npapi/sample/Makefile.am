AM_CPPFLAGS = \
	-fPIC -fshort-wchar \
	-I $(srcdir) \
	-I ../include \
	-I ../include/w3c \
	-I $(srcdir)/../include \
	-I $(srcdir)/../../include \
	-I ../../cplusplus \
	-I $(srcdir)/../../cplusplus \
	-g \
	-DXP_UNIX

if !NACL
lib_LTLIBRARIES = libnpessample.la
AM_LDFLAGS = -L.. -ldl -lesnpapi
else
bin_PROGRAMS = npessample
AM_LDFLAGS = -L.. -lesnpapi -lgoogle_nacl_npruntime -lgoogle_nacl_imc -lpthread -lsrpc
npessample_CPPFLAGS = $(AM_CPPFLAGS)
endif

cpp_source_files = \
	np_entry.cpp \
	npp_gate.cpp \
	npp_sample.cpp \
	sample.cpp

if !NACL
cpp_source_files += \
	npn_gate.cpp
endif

header_files = \
	sample.h

if !NACL
libnpessample_la_SOURCES = $(cpp_source_files) $(header_files)
else
npessample_SOURCES = $(cpp_source_files) $(header_files)
endif

EXTRA_DIST = \
	sample.html \
	sample.nacl.html


