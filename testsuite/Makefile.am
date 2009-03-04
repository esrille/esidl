TESTS_ENVIRONMENT = PATH=..:$(PATH) esidl -I$(srcdir) -ent -npapi

TESTS =         1.idl  2.idl  3.idl  4.idl  5.idl  6.idl  7.idl         9.idl \
	10.idl 11.idl 12.idl        14.idl 15.idl 16.idl 17.idl 18.idl 19.idl \
	20.idl 21.idl 22.idl 23.idl 24.idl 25.idl 26.idl \
	100.idl 101.idl 102.idl 103.idl 104.idl 105.idl 106.idl 107.idl 108.idl 109.idl \
	110.idl \
	dom.idl html.idl views.idl

EXTRA_DIST = $(TESTS) Interface.idl

clean-local:
	-rm -rf *.h *.ent
