#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h> /* usleep */
#include <time.h> /* time */
#include <stdlib.h> /* rand, srand */

#define X 70
#define Y 70
#define PIX 10 /* side of a pixel */
#define DELAY 100
#define ONEIN 5
#define LIVE 'O'
#define DEAD ' '

void output(void);
void step(void);
int neighbors(int x, int y, char old[Y][X]);

char field[Y][X];
Display *d;
Window w;
XEvent e;
int s;

int main()
{
	/* Xlib init */
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

	/* life init */
	srand(time(NULL));

	for(int i = 0; i < Y; i++)
		for(int j = 0; j < X; j++)
			field[i][j] = (rand()%ONEIN==0)?LIVE:DEAD; 
	/*
	for(int i = Y/4; i < Y-Y/4; i++)
		for(int j = X/4; j < X-X/4; j++)
			field[i][j] = (rand()%ONEIN==0)?LIVE:DEAD; 
			*/
	//field[i][j] = DEAD;

	/* glider */
	/*
	   field[2][2] = LIVE;
	   field[2][3] = LIVE;
	   field[2][4] = LIVE;
	   field[3][4] = LIVE;
	   field[4][2] = LIVE;
	   */

	/* main loop */
	while(1) {

		if(XPending(d)) {
			XNextEvent(d,&e);
			if(e.type == ClientMessage) {
				break;
			}
		}

		output();
		step();
		usleep(DELAY*1000);
	}
	XDestroyWindow(d,w);
	XCloseDisplay(d);

	return 0;
}

void output()
{
	XSetForeground(d,DefaultGC(d,s),WhitePixel(d,s));
	XFillRectangle(d,w,DefaultGC(d,s),0,0,X*PIX,Y*PIX);
	XSetForeground(d,DefaultGC(d,s),BlackPixel(d,s));
	for(int i = 0; i < Y; i++) {
		for(int j = 0; j < X; j++) {
			if(field[i][j]==LIVE)
				XFillRectangle(d,w,DefaultGC(d,s),i*PIX,j*PIX,PIX,PIX);
		}
	}
	XFlush(d);
}

void step()
{
	char old[Y][X];
	for(int i = 0; i < Y; i++)
		for(int j = 0; j < X; j++)
			old[i][j]=field[i][j];

	for(int i = 0; i < Y; i++) {
		for(int j = 0; j < X; j++) {
			int neigh = neighbors(i,j,old);
			if(old[i][j]==DEAD) { /* dead cell */
				if(neigh==3)
					field[i][j] = LIVE;
			} else { /* live cell */
				if(neigh != 2 && neigh != 3)
					field[i][j] = DEAD;
			}
		}
	}
}

/* FIYME maybe make old global */
int neighbors(int x, int y, char old[Y][X])
{
	int ret = old[x][y]==LIVE?-1:0;

	for(int i = -1; i < 2; i++)
		for(int j = -1; j < 2; j++)
			if(old[(x+i+Y)%Y][(y+j+X)%X]==LIVE)
				ret++;

	return ret;
}
