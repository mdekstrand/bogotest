SOURCES = main.c runner.c data.c assert.c

HEADERS = bogotest.h internal.h

OBJECTS = $(SOURCES:.c=.o)

CFLAGS += `pkg-config --cflags glib-2.0`

libbogotest.a: $(OBJECTS)
	ar rc $@ $^
	ranlib $@

%.o: %.c $(HEADERS)
