SUFFIXES = .java .idl

vpath %.idl = $(srcdir)/../dom

w3c_generated_files = \
	org/w3c/dom/Node.java

w3c_idl_files = \
	cssomview.idl \
	dom2css.idl \
	dom2events.idl \
	dom2ranges.idl \
	dom2stylesheets.idl \
	dom2traversal.idl \
	dom3core.idl \
	dom3ls.idl \
	dom3validation.idl \
	dom3xpath.idl \
	file.idl \
	html5.idl \
	xhr2.idl
	
nodist_noinst_HEADERS = \
	$(w3c_generated_files)

$(w3c_generated_files) : $(w3c_idl_files)
	../esidl -java $^

noinst_DATA = build.xml

build.xml : build.xml.src
	cp $^ $@

clean-local:
	rm -rf org build.xml

EXTRA_DIST = build.xml.src

