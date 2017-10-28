#include "output.h"
#include <stdio.h>


void cleanup()
{
}

void init()
{
	/* smooth console output */
	setvbuf(stdout, NULL, _IOFBF, BUFSIZ);
}

void output(char field[Y][X])
{
	/* clear screen
	 * taken from clear.c in ubase
	 */
	printf("\x1b[2J\x1b[H");

	/* top */
	printf("┌");
	for(int i = 0; i < X; i++)
		printf("─");
	printf ("┐\n");

	/* rows */
	for(int i = 0; i < Y; i++) {
		printf("│");
		for(int j = 0; j < X; j++)
			printf("%c",field[i][j]);
		printf ("│\n");
	}

	/* bottom */
	printf("└");
	for(int i = 0; i < X; i++)
		printf("─");
	printf ("┘");

	fflush(stdout);
}

int process_input()
{
	return 0;
}
