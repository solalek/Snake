#include <iostream>
#include <ctime>
#include <termios.h>
#include "stuff/stuff.h"
#include "game.h"
#include "snake/snake.h"
#include "apple/apple.h"

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

void printMap(Snake &snake, Apple &apple);


void Game::Start() {
		Snake snake;
		Apple apple;
		int time = clock();
		printMap(snake, apple);
		struct termios orig_termios;
		enableRawMode(orig_termios);

		while (true) {
			if (kbhit()) {
				char ch = getch();
				switch (ch) {
					case 'w':
						dir = Directions::UP;
						break;
					case 'a':
						dir = Directions::LEFT;
						break;
					case 's':
						dir = Directions::DOWN;
						break;
					case 'd':
						dir = Directions::RIGHT;
						break;
				}
			}
			if ((clock() - time) / (speed) >= 1) {
				time = clock();
				snake.move(apple);
				printMap(snake, apple);
			}
		}
		disableRawMode(orig_termios);
}

int Game::Main() {
    int choice;
    std::cout << "1. Start\n" << "2. Options\n" << "3. Exit\n" << "Enter: ";
    std::cin >> choice;
    return choice;
}

int Game::Options() {
    int choice;
    std::cout << "1. Set the speed\n" << "2. Back\n" << "Enter: ";
    std::cin >> choice;
    return choice;
}

int Game::Speed() {
    system("clear");
    bool isRunning = true;
    int choice = SpeedButtons::BACKSPD;
    while (isRunning)
    {
        if (speed == CLOCKS_PER_SEC / 2) {
            std::cout << "Now speed is fast\n";
        }
        else if (speed == CLOCKS_PER_SEC) {
            std::cout << "Now speed is middle\n";
        }
        else  if (speed == CLOCKS_PER_SEC * 1.5) {
            std::cout << "Now speed is slow\n";
        }
        std::cout << "1. Fast\n" << "2. Middle\n" << "3. Slow\n" << "4. Back\n" << "Enter: ";
        std::cin >> choice;
        switch (choice) {
        case SpeedButtons::FAST:
            speed = CLOCKS_PER_SEC / 2;
            system("clear");
            continue;
        case SpeedButtons::MIDDLE:
            speed = CLOCKS_PER_SEC;
            system("clear");
            continue;
        case SpeedButtons::SLOW:
            speed = CLOCKS_PER_SEC * 1.5;
            system("clear");
            continue;
        case SpeedButtons::BACKSPD:
            isRunning = false;
            break;
        }
    }
    return choice;
}

void Game::Run() {
    bool isRunning = true;
    while (isRunning) {
        int choice = Main();
        switch (choice) {
        case MainButtons::START:
            Start();
            break;
        case MainButtons::EXIT:
            return;
        case MainButtons::OPTIONS:
            system("clear");
            choice = Options();
            switch (choice) {
            case OptionsButtons::SPEED:
                Speed();
                system("clear");
                continue;
            case OptionsButtons::BACKOPT:
                system("clear");
                continue;
            }
        }
    }
}

void printMap(Snake &snake, Apple &apple) {
	system("clear");  // Use "clear" command for Linux
	Snake::Body* snakeBody = snake.head;
	map[apple.y][apple.x] = '@';
	while (snakeBody != nullptr) {
		map[snakeBody->y][snakeBody->x] = '0';
		snakeBody = snakeBody->next;
	}
	std::cout << "Size: " << snake.GetSize() << std::endl;
	for (auto& el : map) {
		std::cout << el;
	}
	snakeBody = snake.head;
	while (snakeBody != nullptr) {
		map[snakeBody->y][snakeBody->x] = ' ';
		snakeBody = snakeBody->next;
	}
	map[apple.y][apple.x] = ' ';
}
