.POSIX:
VERSION = 0.1.0a

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

CC = cc -std=c99

BXGAMMA_CFLAGS = $(CFLAGS) -DVERSION='"$(VERSION)"' $(CPPFLAGS)
BXGAMMA_LIBS = -lX11 -lXxf86vm -lsdie $(LDLIBS)

BIN = bxgamma
SRC = bxgamma.c
MAN1 = bxgamma.1
DISTFILES = COPYING README Makefile $(SRC) $(MAN1)
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
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -f $(BIN) $(DESTDIR)$(BINDIR)
	cp -f $(MAN1) $(DESTDIR)$(MANDIR)/man1

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(BIN)
	rm -f $(DESTDIR)$(MANDIR)/man1/$(MAN1)

.SUFFIXES: .c
.c:
	$(CC) $(BXGAMMA_CFLAGS) $(LDFLAGS) -o $@ $< $(BXGAMMA_LIBS)
