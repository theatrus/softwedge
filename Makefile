
all: softwedge

softwedge: sw/softwedge.c sw/softwedge.h
	$(CC) -Wall -Isw -O2 -o softwedge sw/softwedge.c -lX11 -lXtst

install:
	install softwedge /usr/local/bin/

clean:
	rm -f softwedge