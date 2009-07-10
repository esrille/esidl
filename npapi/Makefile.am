SUBDIRS = include . sample

lib_LIBRARIES = libesnpapi.a

AM_CPPFLAGS = \
	-fPIC -fshort-wchar \
	-I include \
	-I $(srcdir)/include \
	-I $(srcdir)/../include

libesnpapi_a_SOURCES = \
	include/any.h \
	include/esnpapi.h \
	include/proxyImpl.h \
	src/bridge.cpp \
	src/invoke.cpp

if I386
libesnpapi_a_SOURCES += src/variant386.cpp
endif I386

if X86_64
libesnpapi_a_SOURCES += src/variantx64.S
endif X86_64

