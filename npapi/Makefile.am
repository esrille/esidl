SUBDIRS = . sample

lib_LIBRARIES = libesnpapi.a

AM_CPPFLAGS = \
	-fshort-wchar \
	-I include \
	-I $(srcdir)/include \
	-I $(srcdir)/../include \
	-I ../cplusplus \
	-I $(srcdir)/../cplusplus \
	-DXP_UNIX \
	-fno-implicit-inline-templates -fno-default-inline

if !NACL
AM_CPPFLAGS += -fPIC
endif

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
	src/proxy.cpp \
	src/stub.cpp \
	src/metadata.cpp \
	src/metadataFile.cpp \
	src/metadataGeolocation.cpp \
	src/metadataHtml.cpp \
	src/metadataHtmlA_G.cpp \
	src/metadataHtmlH_N.cpp \
	src/metadataHtmlO_U.cpp \
	src/metadataHtmlV_Z.cpp \
	src/metadataSvg.cpp \
	src/metadataSvgA_E.cpp \
	src/metadataSvgF_G.cpp \
	src/metadataSvgH_N.cpp \
	src/metadataSvgO_U.cpp \
	src/metadataSvgV_Z.cpp \
	src/metadataWebDatabase.cpp \
	src/metadataWebGL.cpp \
	src/metadataWorkers.cpp

