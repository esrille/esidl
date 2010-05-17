if NACL
esidl=esidl
else
esidl=../esidl
endif

SUFFIXES = .idl

vpath %.idl = $(srcdir)/../dom

w3c_generated_files = \
	org/w3c/dom/Node.h

w3c_idl_files = \
	2dcontext.idl \
	console.idl \
	cssom.idl \
	cssomview.idl \
	dom2css.idl \
	dom2ranges.idl \
	dom2traversal.idl \
	dom2views.idl \
	dom3core.idl \
	dom3events.idl \
	dom3ls.idl \
	dom3validation.idl \
	dom3xpath.idl \
	eventsource.idl \
	elementtraversal.idl \
	file.idl \
	geolocation.idl \
	html5.idl \
	indexeddb.idl \
	microdata.idl \
	postmsg.idl \
	progress-events.idl \
	selectors.idl \
	smil.idl \
	svg.idl \
	webdatabase.idl \
	webgl.idl \
	webnotifications.idl \
	webstorage.idl \
	websocket.idl \
	workers.idl \
	xhr2.idl
	
nodist_noinst_HEADERS = \
	$(w3c_generated_files)

$(w3c_generated_files) : $(w3c_idl_files)
	$(esidl) -cplusplus -string std::string -object Object -fvirtualbase $^

clean-local:
	rm -rf org com *.h

EXTRA_DIST = \
	any.h \
	nullable.h \
	sequence.h \
	org/w3c/dom.h \
	org/w3c/dom/ObjectArray.h \
	org/w3c/dom/ObjectMap.h

