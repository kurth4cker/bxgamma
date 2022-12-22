.POSIX:

NAME = bxgamma
VERSION = 0.1.0a

CC = cc

GAMMA_CPPFLAGS = -DPACKAGE_STRING='"$(NAME) $(VERSION)"' $(CPPFLAGS)
GAMMA_CFLAGS = -std=c99 $(CFLAGS)
GAMMA_LIBS = -lX11 -lXxf86vm $(LDLIBS)

PROG = $(NAME)
SRC = bxgamma.c

all: $(PROG)

clean:
	rm -f $(PROG)

.SUFFIXES: .c
.c:
	$(CC) $(GAMMA_CFLAGS) $(GAMMA_CPPFLAGS) $(LDFLAGS) -o $@ $< $(GAMMA_LIBS)
