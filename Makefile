.POSIX:

NAME = bxgamma
VERSION = 0.1.0

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

CC = cc

BXGAMMA_CPPFLAGS = -DPACKAGE_STRING='"$(NAME) $(VERSION)"' $(CPPFLAGS)
BXGAMMA_CFLAGS = $(CFLAGS)
BXGAMMA_LIBS = -lX11 -lXxf86vm $(LDLIBS)

PROG = $(NAME)
SRC = bxgamma.c

all: $(PROG)

clean:
	rm -f $(PROG)

dist:
	mkdir -p $(NAME)-$(VERSION)
	cp -f COPYING README Makefile $(SRC) $(NAME)-$(VERSION)
	tar -cf $(NAME)-$(VERSION).tar $(NAME)-$(VERSION)
	rm -rf $(NAME)-$(VERSION)

install: $(PROG)
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -f $(PROG) $(DESTDIR)$(BINDIR)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(PROG)

.SUFFIXES: .c
.c:
	$(CC) $(BXGAMMA_CFLAGS) $(BXGAMMA_CPPFLAGS) $(LDFLAGS) -o $@ $< $(BXGAMMA_LIBS)
