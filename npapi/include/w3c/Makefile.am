SUFFIXES = .h .idl

w3c_generated_files = \
	css.h \
	cssomview.h \
	dom.h \
	events.h \
	geolocation.h \
	html5.h \
	ls.h \
	ranges.h \
	smil.h \
	stylesheets.h \
	svg.h \
	traversal.h \
	views.h

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

EXTRA_DIST = \
	$(w3c_idl_files) \
	COPYRIGHT.html

$(w3c_generated_files) : $(w3c_idl_files)
	../../../esidl -I$(srcdir) -namespace es -template -string std::string -indent google -object Object -fvirtualbase $^

clean-local:
	rm -rf *.h

