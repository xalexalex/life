CFLAGS=-std=c99 -Wall -pedantic -O2

all: xlife termlife

xlife: life.o x.o
	cc $(CFLAGS) $^ -lX11 -o$@

termlife: life.o term.o
	cc $(CFLAGS) $^ -o$@

clean:
	rm xlife termlife *.o
