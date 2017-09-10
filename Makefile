CFLAGS   = -std=c99 -Wall -pedantic -O2
DESTDIR=/home/al/bin

all: xlife termlife

xlife: xlife.c
	cc $(CFLAGS) $^ -lX11 -o$@

termlife: termlife.c
	cc $(CFLAGS) $^ -o$@

install: all
	mkdir -p $(DESTDIR)
	cp xlife $(DESTDIR)/xlife
	cp termlife $(DESTDIR)/termlife

clean:
	rm xlife termlife
