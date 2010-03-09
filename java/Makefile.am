SUFFIXES = .java .idl

vpath %.idl = $(srcdir)/../dom

w3c_generated_files = \
	org/w3c/dom/Node.java

w3c_idl_files = \
	arrays.idl \
	cssomview.idl \
	dom2css.idl \
	dom2events.idl \
	dom2ranges.idl \
	dom2stylesheets.idl \
	dom2traversal.idl \
	dom2views.idl \
	dom3core.idl \
	dom3ls.idl \
	dom3validation.idl \
	dom3xpath.idl \
	eventsource.idl \
	file.idl \
	geolocation.idl \
	html5.idl \
	indexeddb.idl \
	progress-events.idl \
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
	../esidl -java $^

noinst_DATA = build.xml java-binding.zip

build.xml : build.xml.src
	sed -e 's|$$(srcdir)|$(srcdir)|' $^ > $@

java-binding.zip : $(w3c_generated_files)
	find . -name "*.java" -print | zip -q $@ -@

clean-local:
	rm -rf org $(noinst_DATA)

EXTRA_DIST = build.xml.src

