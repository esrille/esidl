SUFFIXES = .idl

vpath %.idl = $(srcdir)/../dom

esidl=../esidl

lib_LIBRARIES = libesv8api.a

LDADD = libesv8api.a

AM_CPPFLAGS = \
	-std=gnu++0x -Wall \
	-I $(srcdir)/include \
	-I $(srcdir)/../cplusplus \
	-DXP_UNIX -DHAVE_CONFIG_H

AM_LDFLAGS = \
	-lpthread -lv8

nodist_libesv8api_a_SOURCES = $(generated_sources) $(generated_headers)

libesv8api_a_SOURCES = \
	include/esv8api.h \
	include/object.h \
	src/bridge.cpp \
	src/Any.cpp

bin_PROGRAMS = \
	bridge.test

bin_PROGRAMS : libesv8api.a

generated_test_headers = \
	org/w3c/dom/test/Event.h \
	org/w3c/dom/test/EventInit.h \
	org/w3c/dom/test/CustomEventInit.h \
	org/w3c/dom/test/EventListener.h \
	org/w3c/dom/test/EventTarget.h

$(generated_test_headers) : org/w3c/dom/test/Event.h

generated_test_sources = \
	org/w3c/dom/test/Event.cpp \
	org/w3c/dom/test/EventInit.cpp \
	org/w3c/dom/test/CustomEventInit.cpp \
	org/w3c/dom/test/EventListener.cpp \
	org/w3c/dom/test/EventTarget.cpp

$(generated_test_sources) : org/w3c/dom/test/Event.cpp

BUILT_SOURCES = \
	$(generated_test_headers) $(generated_test_sources)

bridge_test_SOURCES = \
	test/bridge.test.cpp \
	test/EventImp.h \
	test/EventListenerImp.h \
	test/EventTargetImp.h \
	test/test.idl

bridge_test_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I $(srcdir)/test

nodist_bridge_test_SOURCES = \
	$(generated_test_headers) $(generated_test_sources)

clean-local:
	rm -rf org *.h

EXTRA_DIST = config.h

org/w3c/dom/test/Event.h : test/test.idl
	$(esidl) -messenger -string std::u16string -object Object -fno-multipleinheritance $^

org/w3c/dom/test/Event.cpp : test/test.idl
	$(esidl) -messenger-src -string std::u16string -object Object -fno-multipleinheritance $^
