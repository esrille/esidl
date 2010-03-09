SUBDIRS = .

SUFFIXES = .h .idl

noinst_HEADERS = \
	esnpapi.h \
	proxyImpl.h \
	npapi/npapi.h \
	npapi/npruntime.h \
	npapi/nptypes.h \
	npapi/npupp.h

nodist_noinst_HEADERS = \
	object.h

EXTRA_DIST = \
	object.idl

object.h : object.idl
	../../esidl -I$(srcdir) -string std::string -indent google -object Object $<

clean-local:
	rm -rf object.h

