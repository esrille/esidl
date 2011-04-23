if NACL
esidl=esidl
else
esidl=../esidl
endif

SUFFIXES = .java .idl

vpath %.idl = $(srcdir)/../dom

w3c_generated_files = \
	org/w3c/dom/Node.java

w3c_idl_files = \
	2dcontext.idl \
	arrays.idl \
	cssom.idl \
	cssomview.idl \
	dom2css.idl \
	dom2ranges.idl \
	dom2traversal.idl \
	dom3events.idl \
	dom3ls.idl \
	dom3validation.idl \
	dom3xpath.idl \
	domcore.idl \
	eventsource.idl \
	file.idl \
	html5.idl \
	microdata.idl \
	postmsg.idl \
	progress-events.idl \
	selectors.idl \
	typedarray.idl \
	url.idl \
	webgl.idl \
	xhr2.idl
	
nodist_noinst_HEADERS = \
	$(w3c_generated_files)

$(w3c_generated_files) : $(w3c_idl_files)
	$(esidl) -java $^

noinst_DATA = build.xml java-binding.zip

build.xml : build.xml.src
	sed -e 's|$$(srcdir)|$(srcdir)|' $^ > $@

java-binding.zip : $(w3c_generated_files)
	find . -name "*.java" -print | zip -q $@ -@

clean-local:
	rm -rf org $(noinst_DATA)

EXTRA_DIST = \
	build.xml.src \
	org/w3c/dom/ObjectArray.java

