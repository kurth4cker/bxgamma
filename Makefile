.POSIX:

VERSION = 0.1.0

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

CC = cc -std=c99

GAMMA_CFLAGS = $(CFLAGS) -DVERSION='"$(VERSION)"' $(CPPFLAGS)
GAMMA_LIBS = -lX11 -lXxf86vm -lsdie $(LDLIBS)

BIN = bxgamma
SRC = bxgamma.c

DISTFILES = COPYING README Makefile $(SRC)
DISTDIR = bxgamma-$(VERSION)

all: $(BIN)

clean:
	rm -f $(BIN)

dist:
	mkdir -p $(DISTDIR)
	cp -f $(DISTFILES) $(DISTDIR)
	tar -cf $(DISTDIR).tar $(DISTDIR)
	rm -rf $(DISTDIR)

install: $(BIN)
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -f $(BIN) $(DESTDIR)$(BINDIR)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(BIN)

.SUFFIXES: .c
.c:
	$(CC) $(GAMMA_CFLAGS) $(LDFLAGS) -o $@ $< $(GAMMA_LIBS)
