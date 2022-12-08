.POSIX:

NAME = xgamma
VERSION = 1.0.8a

CC = cc

CPPFLAGS = -DPACKAGE_STRING='"$(NAME) $(VERSION)"'
LDLIBS = -lX11 -lXxf86vm

PROG = $(NAME)
SRC = xgamma.c

all: $(PROG)

clean:
	rm -f $(PROG)

.SUFFIXES: .c
.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)
