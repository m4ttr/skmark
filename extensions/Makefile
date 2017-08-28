KDIR=/usr/src/linux-headers-$(shell uname -r)
CFLAGS=-I$(KDIR)/include/uapi -I$(KDIR)/include -I../include -DPIC -fPIC -Wno-cpp
LDFLAGS=-lxtables -shared

.PHONY: lib
lib: libxt_SKMARK.so 

%.so: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm libxt_SKMARK.so 

.PHONY: install
install: lib
	install -d $(DESTDIR)$(shell pkg-config --variable=xtlibdir xtables)
	install -m 0755 libxt_SKMARK.so $(DESTDIR)$(shell pkg-config --variable=xtlibdir xtables)

