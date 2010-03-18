SUBDIRS = . sample

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
	include/object.h \
	include/proxyImpl.h \
	include/npapi/npapi.h \
	include/npapi/npruntime.h \
	include/npapi/nptypes.h \
	include/npapi/npupp.h \
	src/bridge.cpp \
	src/invoke.cpp \
	src/resolve.cpp

