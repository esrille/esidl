SUBDIRS = . w3c

SUFFIXES = .h .idl

noinst_HEADERS = \
	any.h \
	object.h \
	esnpapi.h \
	proxyImpl.h \
	npapi/npapi.h \
	npapi/npruntime.h \
	npapi/nptypes.h \
	npapi/npupp.h
	
EXTRA_DIST = \
	object.idl

object.h : object.idl
	../../esidl -I $(srcdir) -string std::string -indent google -object Object $<

clean-local:
	rm -rf object.h

dist-hook:
	rm -rf $(distdir)/object.h
