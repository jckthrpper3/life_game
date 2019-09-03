#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define Xsize 20
#define Ysize 10

int getMenu(void);
void gotoXY(int, int);
void displayfield(char**);
int getch(void);
void generatefield(char**);

int main(int argc, char *argv[]) {
	int i, j, X=0, Y=0;
	char **field = (char **) malloc(sizeof(char*)*Xsize), c = '0';
	for (i=0; i < Xsize; i++)
		field[i] = (char *) malloc(sizeof(char)*Ysize);

	for (i=0; i < Xsize; i++)
		for(j=0;j < Ysize; j++)
			field[i][j] = '*';
	printf("\033[H\033[J\t\t\t\t\t\033[1;31mL\033[1;32mI\033[1;33mF\033[1;34mE\033[m\n");
	printf("Use HJKL to move, F to place a fish, S for a shrimp, R for rock and C to clear selected field. You can quit with Q\n");

	printf("\e[?25l");
	displayfield(field);
	gotoXY(4+X, 4+Y);
	printf("\033[1;31m%c\033[m", field[X][Y]);
	gotoXY(4+X, 4+Y);
	while(!((c == 'Q') || (c == 'q'))) {
		c = getch();
		gotoXY(4+X, 4+Y);
		printf("%c", field[X][Y]);
		gotoXY(4+X, 4+Y);
		switch(c) {
			case 'H':
			case 'h':
				if (X > 0)
					X--;
				break;
			case 'J':
			case 'j':
				if (Y < Ysize-1)
					Y++;
				break;
			case 'K':
			case 'k':
				if (Y > 0)
					Y--;
				break;
			case 'L':
			case 'l':
				if (X < Xsize - 1)
					X++;
				break;
			case 'F':
			case 'f':
				field[X][Y] = 'F';
				break;
			case 'S':
			case 's':
				field[X][Y] = 'S';
				break;
			case 'R':
			case 'r':
				field[X][Y] = 'R';
				break;
			case 'C':
			case 'c':
			case ' ':
				field[X][Y] = '*';
				break;
		}
		gotoXY(4+X, 4+Y);
		printf("\033[1;31m%c\033[m", field[X][Y]);
		gotoXY(4+X, 4+Y);
	}
	gotoXY(4+X, 4+Y);
	printf("%c", field[X][Y]);
	
	gotoXY(0,3);
	c = 0;
	printf("Use S to step and R to run\n");
	while (!((c == 'Q') || (c == 'q'))) {
		c = getch();
		switch (c) {
			case 'S':
			case 's':
				generatefield(field);
				displayfield(field);
				break;
			case 'R':
			case 'r':
				gotoXY(3, 15);
				printf("Sorry, this option is under development:(");
		}
	}
	for (i=0; i < Xsize; i++)
		free(field[i]);
	free(field);
	printf("\033[H\033[J");
	printf("\e[?25h");
	return 0;
}
int getMenu(void) {
	return 0;
}
void gotoXY(int x, int y) {
	printf("%c[%d;%df",0x1B,y,x);
	return;
}
void displayfield(char **field) {
	for(int i=0; i < Xsize; i++)
		for(int j=0; j < Ysize; j++) {
			gotoXY(4+i, 4+j);
			printf("%c", field[i][j]);
		}
	return;
}
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
void generatefield(char **field) {
	int X, Y, count, loI, hiI, loJ, hiJ, i, j;
	for (X=0; X < Xsize; X++)
		for(Y=0; Y < Ysize; Y++) {
			count = -1;
			loI = loJ = -1;
			hiI = hiJ =  1;
			switch (field[X][Y]) {
				case 'F':
					if (X == 0)
						loI = 0;
					if (X == Xsize-1)
						hiI=0;
					if (Y == 0)
						loJ = 0;
					if (Y == Ysize-1)
						hiJ = 0;
					for (i=loI; i <= hiI; i++)
						for (j=loJ; j <= hiJ; j++)
							if((field[X+i][Y+j] == 'F') || (field[X+i][Y+j] == 'f'))
								count++;
					if ((count > 3) || (count < 2))
						field[X][Y] = 'f';
					break;
				case 'S':
					if (X == 0)
						loI = 0;
					if (X == Xsize-1)
						hiI=0;
					if (Y == 0)
						loJ = 0;
					if (Y == Ysize-1)
						hiJ = 0;
					for (i=loI; i <= hiI; i++)
						for (j=loJ; j <= hiJ; j++)
							if((field[X+i][Y+j] == 'S') || (field[X+i][Y+j] == 's'))
								count++;
					if ((count > 3) || (count < 2))
						field[X][Y] = 's';
					break;
				case '*':
					count = 0;

					if (X == 0)
						loI = 0;
					if (X == Xsize-1)
						hiI=0;
					if (Y == 0)
						loJ = 0;
					if (Y == Ysize-1)
						hiJ = 0;
					for (i=loI; i <= hiI; i++)
						for (j=loJ; j <= hiJ; j++)
							if((field[X+i][Y+j] == 'F') || (field[X+i][Y+j] == 'f'))
								count++;
					if (count == 3)
						field[X][Y] = '0';
					break;
			} // end of switch
		}
	for (X=0; X < Xsize; X++)
		for (Y=0; Y < Ysize; Y++) {
			switch(field[X][Y]) {
				case 'f':
				case 's':
					field[X][Y] = '*';
					break;
				case '0':
					field[X][Y] = 'F';
					break;
			}
		}
	return;
}
