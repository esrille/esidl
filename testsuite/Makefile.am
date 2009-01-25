TESTS_ENVIRONMENT = PATH=..:$(PATH) esidl -I$(srcdir) -npapi

TESTS = 1.idl 2.idl 3.idl 4.idl 5.idl 6.idl 7.idl 8.idl 9.idl \
	10.idl 11.idl 12.idl        14.idl 15.idl 16.idl 17.idl 18.idl 19.idl \
	20.idl 21.idl 22.idl 23.idl 24.idl 25.idl 26.idl \
	dom.idl html.idl

EXTRA_DIST = $(TESTS) Interface.idl

clean-local:
	-rm -rf *.h *.ent
