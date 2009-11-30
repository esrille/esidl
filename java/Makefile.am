SUFFIXES = .java .idl

vpath %.idl = $(srcdir)/../npapi/include/w3c

w3c_generated_files = \
	org/w3c/dom/Node.java

w3c_idl_files = \
	css.idl \
	cssomview.idl \
	dom.idl \
	events.idl \
	geolocation.idl \
	html5.idl \
	ls.idl \
	ranges.idl \
	smil.idl \
	stylesheets.idl \
	svg.idl \
	traversal.idl \
	views.idl
	
nodist_noinst_HEADERS = \
	$(w3c_generated_files)

$(w3c_generated_files) : $(w3c_idl_files)
	../esidl -I$(srcdir)/../npapi/include/w3c -java $^

clean-local:
	rm -rf org

