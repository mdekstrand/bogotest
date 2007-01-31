SOURCES = main.c runner.c data.c assert.c

HEADERS = bogotest.h internal.h

OBJECTS = $(SOURCES:.c=.o)

CPPFLAGS += `pkg-config --cflags glib-2.0`

.PHONY: all clean

all: libbogotest.a

clean:
	rm -f libbogotest.a $(OBJECTS)

libbogotest.a: $(OBJECTS)
	ar rc $@ $^
	ranlib $@

%.o: %.c $(HEADERS)
