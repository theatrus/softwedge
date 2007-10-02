
all: softwedge

softwedge: softwedge.c
	$(CC) -O2 -o softwedge softwedge.c -lX11 -lXtst
