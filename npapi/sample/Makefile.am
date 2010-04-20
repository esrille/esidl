AM_CPPFLAGS = \
	-fshort-wchar -std=gnu++0x \
	-I $(srcdir) \
	-I ../include \
	-I ../include/w3c \
	-I $(srcdir)/../include \
	-I $(srcdir)/../../include \
	-I ../../cplusplus \
	-I $(srcdir)/../../cplusplus \
	-DXP_UNIX \
	-fno-implicit-inline-templates -fno-default-inline

if !NACL
AM_CPPFLAGS += -fPIC
lib_LTLIBRARIES = libnpessample.la libnpespaint.la libnpesxhr.la libnpesmd5sum.la
AM_LDFLAGS = -L.. -ldl -lesnpapi
else
bin_PROGRAMS = npessample npespaint npesxhr npesmd5sum
AM_LDFLAGS = -L.. -lesnpapi -lgoogle_nacl_npruntime -lgoogle_nacl_imc -lpthread -lsrpc
npessample_CPPFLAGS = $(AM_CPPFLAGS)
npespaint_CPPFLAGS = $(AM_CPPFLAGS)
npesxhr_CPPFLAGS = $(AM_CPPFLAGS)
npesmd5sum_CPPFLAGS = $(AM_CPPFLAGS)
endif

cpp_source_files = \
	np_entry.cpp \
	npp_gate.cpp

if !NACL
cpp_source_files += \
	npn_gate.cpp
endif

if !NACL
libnpessample_la_SOURCES = $(cpp_source_files) $(header_files) npp_sample.cpp sample.cpp sample.h
libnpespaint_la_SOURCES = $(cpp_source_files) $(header_files) npp_paint.cpp paint.cc paint.h
libnpesxhr_la_SOURCES = $(cpp_source_files) $(header_files) npp_xhr.cpp xhr.cc xhr.h
libnpesmd5sum_la_SOURCES = $(cpp_source_files) $(header_files) npp_md5sum.cpp md5sum.cc md5sum.h
else
npessample_SOURCES = $(cpp_source_files) $(header_files) npp_sample.cpp sample.cpp sample.h
npespaint_SOURCES = $(cpp_source_files) $(header_files) npp_paint.cpp paint.cc paint.h
npesxhr_SOURCES = $(cpp_source_files) $(header_files) npp_xhr.cpp xhr.cc xhr.h
npesmd5sum_SOURCES = $(cpp_source_files) $(header_files) npp_md5sum.cpp md5sum.cc md5sum.h
endif

EXTRA_DIST = \
	md5sum.html \
	md5sum.nacl.html \
	paint.html \
	paint.nacl.html \
	eraser.png pen.png tools.png \
	sample.html \
	sample.nacl.html \
	red.css green.css blue.css \
	xhr.html \
	xhr.nacl.html \
	xhr.txt


