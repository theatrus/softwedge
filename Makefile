
# Really crappy makefile to make softwedge

all: libsoftwedge.a softwedge

libsoftwedge.a: sw/softwedge.c sw/softwedge.h
	$(CC) -Wall -Isw -O2 -c sw/softwedge.c -o sw/softwedge.o
	ar cr sw/libsoftwedge.a sw/softwedge.o

softwedge: sw/main.c sw/softwedge.h
	$(CC) -Wall -Isw -O2 -Lsw -o softwedge sw/main.c -lX11 -lXtst -lsoftwedge

install:
	install softwedge /usr/local/bin/

clean:
	rm -f softwedge