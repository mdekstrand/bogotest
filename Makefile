VERSION = 0.1

SOURCES = main.c runner.c data.c assert.c

HEADERS = bogotest.h internal.h

OBJECTS = $(SOURCES:.c=.o)

DEPFILES = $(SOURCES:%.c=%.dep)

CPPFLAGS += `pkg-config --cflags glib-2.0 gobject-2.0`

DISTFILES = Makefile lib.mk $(SOURCES) $(HEADERS) \
	    test/runtests test/test.c test/tests.dat
TARBALL = bogotest-$(VERSION).tgz

PREFIX ?= /usr/local

include lib.mk

.PHONY: all clean check install dist _gcheck

all: _gcheck libbogotest.a
	@echo "Build successful."

define do-clean
  rm -f libbogotest.a $(OBJECTS) $(DEPFILES) $(TARBALL)
endef
ifndef VERBOSE
clean:
	@echo "   [CLEAN]"
	@$(do-clean)
else
clean:
	$(do-clean)
endif

check: libbogotest.a
	(cd test && ./runtests)

install:
	install -d $(DESTDIR)$(PREFIX)/include
	install -d $(DESTDIR)$(PREFIX)/lib
	install bogotest.h $(DESTDIR)$(PREFIX)/include
	install libbogotest.a $(DESTDIR)$(PREFIX)/lib

dist: $(TARBALL)
	@echo "Successfully built distribution tarball."

$(TARBALL): $(DISTFILES)
	$(mkdist-tarball)

_gcheck:
	@pkg-config --atleast-version=2.4.0 glib-2.0 || \
		(echo "Error: glib2 >= 2.4.0 required"; false)
	@pkg-config --atleast-version=2.4.0 gobject-2.0 || \
		(echo "Error: gobject >= 2.4.0 required"; false)

libbogotest.a: $(OBJECTS)
	$(make-slib)

%.o: %.c
	$(compile-c)

%.dep: %.c
	$(depend)

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(DEPFILES)
endif
