AR ?= ar
RANLIB ?= ranlib

ifndef VERBOSE
  define make-slib
    @echo "      [AR] $@"
    @$(AR) rc $@ $?
    @echo "  [RANLIB] $@"
    @$(RANLIB) $@
  endef
else
  define make-slib
    $(AR) rvc $@ $?
    $(RANLIB) $@
  endef
endif

define c_compiler
  $(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<
endef
ifndef VERBOSE
  define compile-c
    @echo "      [CC] $<"
    @$(c_compiler)
  endef
else
  define compile-c
    $(c_compiler)
  endef
endif

define depend-impl
  -rm -f $@; \
  	$(CC) -M -MT "$*.o" $(CPPFLAGS) $< >$@.$$$$ && \
	(sed -e 's@\($*\)\.o[ :]*@\1.o $@ : @g' \
		$@.$$$$ >$@); rm -f $@.$$$$
endef
ifndef VERBOSE
  define depend
    @echo "  [DEPEND] $<"
    @$(depend-impl)
  endef
else
  define depend
    $(depend-impl)
  endef
endif

ifndef VERBOSE
  define mkdist-tarball
    @echo "     [TAR] $@"
    @tar czf $@ $^
  endef
else
  define mkdist-tarball
    tar czvf $@ $^
  endef
endif
