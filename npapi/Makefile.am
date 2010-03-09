SUBDIRS = include . sample

lib_LIBRARIES = libesnpapi.a

AM_CPPFLAGS = \
	-fPIC -fshort-wchar \
	-I include \
	-I $(srcdir)/include \
	-I $(srcdir)/../include \
	-I ../cplusplus \
	-I $(srcdir)/../cplusplus

libesnpapi_a_SOURCES = \
	include/esnpapi.h \
	include/proxyImpl.h \
	src/bridge.cpp \
	src/invoke.cpp

