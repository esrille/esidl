SUFFIXES = .h .idl

w3c_generated_files = \
	css.h \
	cssomview.h \
	dom.h \
	events.h \
	html5.h \
	ls.h \
	ranges.h \
	smil.h \
	stylesheets.h \
	traversal.h \
	views.h

w3c_idl_files = \
	dom.idl \
	ranges.idl \
	cssomview.idl \
	views.idl \
	css.idl \
	events.idl \
	html5.idl \
	traversal.idl \
	ls.idl \
	smil.idl \
	stylesheets.idl
	
noinst_HEADERS = \
	$(w3c_generated_files)

EXTRA_DIST = \
	$(w3c_idl_files) \
	COPYRIGHT.html

$(w3c_generated_files) : $(w3c_idl_files)
	../../../esidl -I $(srcdir) -namespace es -template -string std::string -indent google -object Object $^

clean-local:
	rm -rf *.h

dist-hook:
	rm -rf $(distdir)/*.h

