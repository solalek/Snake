#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctime>
#include <stdio.h>
#include "stuff.h"
#include <sys/select.h>



int dir = Directions::DOWN;


void set_cursor(int x = 0, int y = 0)
{
	std::cout << "\033[" << y << ";" << x << "H";
}

// Disable canonical mode and echo for the terminal
void enableRawMode(struct termios& orig_termios) {
	struct termios raw;
	tcgetattr(STDIN_FILENO, &orig_termios);
	raw = orig_termios;
	raw.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Restore terminal settings
void disableRawMode(struct termios& orig_termios) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Check if a key was pressed
bool kbhit() {
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

char getch() {
	char buf = 0;
	read(STDIN_FILENO, &buf, 1);
	return buf;
}
