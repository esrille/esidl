if NACL
esidl=esidl
else
esidl=../../esidl
endif

SUFFIXES = .idl

vpath %.idl = $(srcdir)

generated_headers = \
	com/rsa/MD5.h \
	org/w3c/dom/Test.h

generated_sources = \
	com/rsa/MD5.cpp \
	org/w3c/dom/Test.cpp

BUILT_SOURCES = $(generated_headers) $(generated_sources)

nodist_noinst_HEADERS = $(generated_headers)

com/rsa/MD5.h : md5.idl
	$(esidl) -messenger -string std::string -object Object -fno-multipleinheritance $^

com/rsa/MD5.cpp : md5.idl
	$(esidl) -messenger-src -string std::string -object Object -fno-multipleinheritance $^

org/w3c/dom/Test.h : test.idl
	$(esidl) -messenger -string std::string -object Object -fno-multipleinheritance $^

org/w3c/dom/Test.cpp : test.idl
	$(esidl) -messenger-src -string std::string -object Object -fno-multipleinheritance $^

AM_CPPFLAGS = \
	-fshort-wchar -std=gnu++0x -Wall \
	-I $(builddir) \
	-I $(builddir)/.. \
	-I $(srcdir) \
	-I ../include \
	-I $(srcdir)/.. \
	-I $(srcdir)/../include \
	-I $(srcdir)/../../include \
	-I $(srcdir)/../../cplusplus \
	-DXP_UNIX -DHAVE_CONFIG_H \
	-fno-implicit-inline-templates -fno-default-inline

if !NACL
AM_CPPFLAGS += -fPIC
lib_LTLIBRARIES = libnpespaint.la libnpesxhr.la libnpesmd5sum.la libnpestest.la 
AM_LDFLAGS = -L.. -ldl -lesnpapi
else
bin_PROGRAMS = npespaint npestest npesxhr npesmd5sum
AM_LDFLAGS = -L.. -lesnpapi -lgoogle_nacl_npruntime -lgoogle_nacl_imc -lpthread -lsrpc
npestest_CPPFLAGS = $(AM_CPPFLAGS)
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
nodist_libnpestest_la_SOURCES = org/w3c/dom/Test.h org/w3c/dom/Test.cpp
libnpestest_la_SOURCES = $(cpp_source_files) npp_test.cpp test.cpp test.h
libnpespaint_la_SOURCES = $(cpp_source_files) npp_paint.cpp paint.cc paint.h
libnpesxhr_la_SOURCES = $(cpp_source_files) npp_xhr.cpp xhr.cc xhr.h
nodist_libnpesmd5sum_la_SOURCES = com/rsa/MD5.h com/rsa/MD5.cpp
libnpesmd5sum_la_SOURCES = $(cpp_source_files) npp_md5sum.cpp md5sum.cc md5sum.h
else
nodist_npestest_SOURCES = org/w3c/dom/Test.h org/w3c/dom/Test.cpp
npespaint_SOURCES = $(cpp_source_files) npp_paint.cpp paint.cc paint.h
npestest_SOURCES = $(cpp_source_files) npp_test.cpp test.cpp test.h
npesxhr_SOURCES = $(cpp_source_files) npp_xhr.cpp xhr.cc xhr.h
nodist_npesmd5sum_SOURCES = com/rsa/MD5.h com/rsa/MD5.cpp
npesmd5sum_SOURCES = $(cpp_source_files) npp_md5sum.cpp md5sum.cc md5sum.h
endif

EXTRA_DIST = \
	md5.idl \
	test.idl 

dist_html_DATA = \
	md5sum.html \
	md5sum.nacl.html \
	paint.html \
	paint.nacl.html \
	eraser.png pen.png tools.png \
	test.html \
	test.nacl.html \
	red.css green.css blue.css \
	xhr.html \
	xhr.nacl.html \
	xhr.txt

if NACL
nodist_html_DATA = $(bin_PROGRAMS)
endif

clean-local:
	-rm -rf com org


