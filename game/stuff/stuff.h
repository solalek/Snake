#ifndef STUFF_H
#define STUFF_H

extern int dir;

enum Directions {
	UP,
	DOWN,
	RIGHT,
	LEFT,
};


void set_cursor(int x, int y);

// Disable canonical mode and echo for the terminal
void enableRawMode(struct termios& orig_termios);

// Restore terminal settings
void disableRawMode(struct termios& orig_termios);

// Check if a key was pressed
bool kbhit();

char getch();

#endif
