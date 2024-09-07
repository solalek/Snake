#ifndef STUFF_H
#define STUFF_H

const int mapX = 21, mapY = 10;

char map[mapY][mapX] = {
	"-------------------\n",
	"|                 |\n",
	"|                 |\n",
	"|                 |\n",
	"|                 |\n",
	"|                 |\n",
	"|                 |\n",
	"|                 |\n",
	"|                 |\n",
	"-------------------\n",
};

enum Directions {
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

int dir = Directions::DOWN;

void set_cursor(int x = 0, int y = 0);

// Disable canonical mode and echo for the terminal
void enableRawMode(struct termios& orig_termios);

// Restore terminal settings
void disableRawMode(struct termios& orig_termios);

// Check if a key was pressed
bool kbhit();

char getch();

#endif
