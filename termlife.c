#include <stdio.h>
#include <unistd.h> /* usleep */
#include <time.h> /* time */
#include <stdlib.h> /* rand, srand */

#define X 40
#define Y 20
#define DELAY 200
#define ONEIN 5
#define LIVE 'O'
#define DEAD ' '

void output(void);
void step(void);
int neighbors(int x, int y, char old[Y][X]);

char field[Y][X];

void main()
{
	/* init */

	/* smooth console output */
	setvbuf(stdout, NULL, _IOFBF, BUFSIZ);

	srand(time(NULL));

	for(int i = 0; i < Y; i++)
		for(int j = 0; j < X; j++)
			//field[i][j] = (rand()%ONEIN==0)?LIVE:DEAD; 
			field[i][j] = DEAD;
	/*
	for(int i = Y/4; i < Y-Y/4; i++)
		for(int j = X/4; j < X-X/4; j++)
			field[i][j] = (rand()%ONEIN==0)?LIVE:DEAD; 
			*/
	//field[i][j] = DEAD;

	/* glider */
	   field[2][2] = LIVE;
	   field[2][3] = LIVE;
	   field[2][4] = LIVE;
	   field[3][4] = LIVE;
	   field[4][2] = LIVE;

	/* main loop */
	while(1) {
		output();
		step();
		usleep(DELAY*1000);
#ifdef DEBUG
		printf("Enter for next");
		getchar();
#endif
	}
}

void output()
{
	/* clear screen
	 * taken from clear.c in ubase
	 */
	printf("\x1b[2J\x1b[H");

	printf("┌"); for(int i = 0; i < X; i++) printf("─");              printf ("┐\n");
	for(int i = 0; i < Y; i++) {
		printf("│"); for(int j = 0; j < X; j++) printf("%c",field[i][j]); printf ("│\n");
	}
	printf("└"); for(int i = 0; i < X; i++) printf("─");              printf ("┘");
	fflush(stdout);
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
#ifdef DEBUG
			neigh && printf("neigh(%d,%d)=%d\n",i,j,neigh);
			field[i][j]=neigh+'0';
#else
			if(old[i][j]==DEAD) { /* dead cell */
				if(neigh==3)
					field[i][j] = LIVE;
			} else { /* live cell */
				if(neigh != 2 && neigh != 3)
					field[i][j] = DEAD;
			}
#endif
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
