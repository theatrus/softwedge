
# Really crappy makefile to make softwedge

DESTDIR=/usr/local/bin

all: libsoftwedge.a softwedge

libsoftwedge.a: sw/softwedge.c sw/softwedge.h
	$(CC) -Wall -Isw -O2 -c sw/softwedge.c -o sw/softwedge.o
	ar cr sw/libsoftwedge.a sw/softwedge.o

softwedge: sw/main.c sw/softwedge.h
	$(CC) -Wall -Isw -O2 -Lsw -o softwedge sw/main.c -lsoftwedge -lX11 -lXtst

install:
	install softwedge $(DESTDIR)

clean:
	rm -f softwedge
