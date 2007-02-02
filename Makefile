SOURCES = main.c runner.c data.c assert.c

HEADERS = bogotest.h internal.h

OBJECTS = $(SOURCES:.c=.o)

CPPFLAGS += `pkg-config --cflags glib-2.0 gobject-2.0`

PREFIX ?= /usr/local

.PHONY: all clean check install

all: libbogotest.a

clean:
	rm -f libbogotest.a $(OBJECTS)

check: libbogotest.a
	(cd test && ./runtests)

install:
	install -d $(DESTDIR)$(PREFIX)/include
	install -d $(DESTDIR)$(PREFIX)/lib
	install bogotest.h $(DESTDIR)$(PREFIX)/include
	install libbogotest.a $(DESTDIR)$(PREFIX)/lib

libbogotest.a: $(OBJECTS)
	ar rc $@ $^
	ranlib $@

%.o: %.c $(HEADERS)
