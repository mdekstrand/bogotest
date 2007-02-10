SOURCES = main.c runner.c data.c assert.c

HEADERS = bogotest.h internal.h

OBJECTS := $(SOURCES:.c=.o)

DEPFILES := $(SOURCES:%.c=%.dep)

CPPFLAGS += `pkg-config --cflags glib-2.0 gobject-2.0`

PREFIX ?= /usr/local

include lib.mk

.PHONY: all clean check install _gcheck

all: _gcheck libbogotest.a

clean:
	rm -f libbogotest.a $(OBJECTS) $(DEPFILES)

check: libbogotest.a
	(cd test && ./runtests)

install:
	install -d $(DESTDIR)$(PREFIX)/include
	install -d $(DESTDIR)$(PREFIX)/lib
	install bogotest.h $(DESTDIR)$(PREFIX)/include
	install libbogotest.a $(DESTDIR)$(PREFIX)/lib

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
