SUFFIXES = .h .idl

%.h : %.idl
	PATH=../..:$(PATH) ../../esidl -I $(srcdir) -namespace es -template -string std::string $<

object.h : object.idl
	PATH=../..:$(PATH) ../../esidl -I $(srcdir) -string std::string $<
	
noinst_HEADERS = \
	any.h \
	esnpapi.h \
	object.h \
	proxyImpl.h \
	npapi/npapi.h \
	npapi/npruntime.h \
	npapi/nptypes.h \
	npapi/npupp.h \
	w3c/css.h \
	w3c/cssomview.h \
	w3c/dom.h \
	w3c/events.h \
	w3c/eventsource.h \
	w3c/html5.h \
	w3c/ls.h \
	w3c/ranges.h \
	w3c/smil.h \
	w3c/stylesheets.h \
	w3c/traversal.h \
	w3c/validation.h \
	w3c/views.h

EXTRA_DIST = \
	object.idl \
	w3c/css.idl \
	w3c/cssomview.idl \
	w3c/dom.idl \
	w3c/events.idl \
	w3c/eventsource.idl \
	w3c/html5.idl \
	w3c/ls.idl \
	w3c/ranges.idl \
	w3c/smil.idl \
	w3c/stylesheets.idl \
	w3c/traversal.idl \
	w3c/validation.idl \
	w3c/views.idl \
	w3c/COPYRIGHT.html

clean-local:
	rm -rf object.h w3c

dist-hook:
	rm -rf $(distdir)/object.h $(distdir)/w3c/*.h

