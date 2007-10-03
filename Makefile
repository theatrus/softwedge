
all: softwedge

softwedge: sw/softwedge.c sw/softwedge.h
	$(CC) -Wall -Isw -O2 -o softwedge sw/softwedge.c -lX11 -lXtst
