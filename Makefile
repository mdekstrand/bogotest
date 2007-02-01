SOURCES = main.c runner.c data.c assert.c

HEADERS = bogotest.h internal.h

OBJECTS = $(SOURCES:.c=.o)

CPPFLAGS += `pkg-config --cflags glib-2.0`

.PHONY: all clean check

all: libbogotest.a

clean:
	rm -f libbogotest.a $(OBJECTS)

check: libbogotest.a
	(cd test && ./runtests)

libbogotest.a: $(OBJECTS)
	ar rc $@ $^
	ranlib $@

%.o: %.c $(HEADERS)
