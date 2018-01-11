#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define PIX 10 /* side of a pixel */

Display *d;
Window w;
XEvent e;
int s;

void cleanup()
{
	XDestroyWindow(d,w);
	XCloseDisplay(d);
}

void init()
{
	d = XOpenDisplay(NULL);
	if(!d) {
		printf("Couldn't open display\n");
		exit(1);
	}
	s = DefaultScreen(d);
	w = XCreateSimpleWindow(d, RootWindow(d,s), 0, 0, X*PIX, Y*PIX, 0, 0, WhitePixel(d,s));

	Atom delWindow = XInternAtom(d, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(d,w,&delWindow, 1);

	/* make the window floating and fixed-size */
	XSizeHints *sizeh = NULL;
	sizeh = XAllocSizeHints();
	sizeh->flags = PSize | PMaxSize | PMinSize;
	sizeh->height =	sizeh->min_width  = sizeh->max_width = X*PIX;
	sizeh->width  =	sizeh->min_height = sizeh->max_height = Y*PIX;
	XSetWMProperties(d, w, NULL, NULL, NULL, 0, sizeh, NULL, NULL);
	XFree(sizeh);

	XSelectInput(d,w,ExposureMask);
	XMapWindow(d,w);
}

void output(char field[Y][X])
{
	XSetForeground(d,DefaultGC(d,s),WhitePixel(d,s));
	XFillRectangle(d,w,DefaultGC(d,s),0,0,X*PIX,Y*PIX);
	XSetForeground(d,DefaultGC(d,s),BlackPixel(d,s));
	for(int i = 0; i < Y; i++) {
		for(int j = 0; j < X; j++) {
			if(field[i][j]==ALIVE)
				XFillRectangle(d,w,DefaultGC(d,s),i*PIX,j*PIX,PIX,PIX);
		}
	}
	XFlush(d);
}

int process_input()
{
	if(XPending(d)) {
		XNextEvent(d,&e);
		if(e.type == ClientMessage) {
			return MSG_EXIT;
		}
	}
	return 0;
}
