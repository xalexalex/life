#include "output.h"
#define _DEFAULT_SOURCE /* for usleep */
#include <unistd.h> /* usleep */
#include <time.h> /* time */
#include <stdlib.h> /* rand, srand */

void step(void);
int neighbors(int x, int y, char old[Y][X]);

char field[Y][X];

int main()
{
	init();

	srand(time(NULL));

	for(int i = 0; i < Y; i++)
		for(int j = 0; j < X; j++)
			field[i][j] = (rand()%ONEIN==0)?LIVE:DEAD; 

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
		if(process_input() == MSG_EXIT)
			break;
		output(field);
		step();
		usleep(DELAY*1000);
	}
	cleanup();

	return 0;
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
