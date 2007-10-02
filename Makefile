
all: softwedge

softwedge: softwedge.c
	$(CC) -Wall -O2 -o softwedge softwedge.c -lX11 -lXtst
