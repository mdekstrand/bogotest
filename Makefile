VERSION = 0.1

SOURCES = main.c runner.c data.c assert.c

HEADERS = bogotest.h internal.h

OBJECTS = $(SOURCES:.c=.o)

DEPFILES = $(SOURCES:%.c=%.dep)

MANPAGES = bogotest.7

MANPAGE_SOURCES = $(MANPAGES:%.7=%.7.txt)
DOC_TEMPFILES = $(MANPAGES:%.7=%.7.xml)

CPPFLAGS += `pkg-config --cflags glib-2.0 gobject-2.0`

DISTFILES = Makefile lib.mk $(SOURCES) $(HEADERS) \
	    test/runtests test/test.c test/tests.dat \
	    $(MANPAGES) $(MANPAGE_SOURCES)
TARBALL = bogotest-$(VERSION).tgz

PREFIX ?= /usr/local
MANDIR ?= $(PREFIX)/share/man
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include

include lib.mk

.PHONY: all clean check install doc veryclean dist _gcheck

all: _gcheck libbogotest.a
	@echo "Build successful."

ifndef VERBOSE
  define do-clean
    @echo "   [CLEAN] $(1)"
    @rm -f $(1)
  endef
else
  define do-clean
    rm -f $(1)
  endef
endif

clean:
	$(call do-clean,libbogotest.a)
	$(call do-clean,$(OBJECTS))
	$(call do-clean,$(DEPFILES))
	$(call do-clean,$(TARBALL))
	$(call do-clean,$(DOC_TEMPFILES))

veryclean: clean
	$(call do-clean,$(MANPAGES))

check: libbogotest.a
	(cd test && ./runtests)

install: libbogotest.a bogotest.h $(MANPAGES)
	install -d $(DESTDIR)$(INCLUDEDIR)
	install -d $(DESTDIR)$(LIBDIR)
	install bogotest.h $(DESTDIR)$(INCLUDEDIR)
	install libbogotest.a $(DESTDIR)$(LIBDIR)
	install -d $(DESTDIR)$(MANDIR)/man7
	install bogotest.7 $(DESTDIR)$(MANDIR)/man7

doc: $(MANPAGES)

dist: $(TARBALL)
	@echo "Successfully built distribution tarball."

$(TARBALL): $(DISTFILES)
	$(mkdist-tarball)

_gcheck:
	@pkg-config --atleast-version=2.4.0 glib-2.0 || \
		(echo "Error: glib2 >= 2.4.0 required"; false)
	@pkg-config --atleast-version=2.4.0 gobject-2.0 || \
		(echo "Error: gobject >= 2.4.0 required"; false)

ifndef VERBOSE
  define build-manpage
    @echo "[ASCIIDOC] $<"
    @a2x -d manpage -f manpage $<
  endef
else
  define build-manpage
    a2x -d manpage -f manpage $<
  endef
endif
%.7: %.7.txt
	$(build-manpage)

libbogotest.a: $(OBJECTS)
	$(make-slib)

%.o: %.c
	$(compile-c)

%.dep: %.c
	$(depend)

ifneq "$(findstring clean,$(filter clean veryclean,$(MAKECMDGOALS)))" "clean"
  -include $(DEPFILES)
endif
