#ifndef APPLE_H
#define APPLE_H

class Game;
class Snake;

class Apple {
	friend Game;
	friend Snake;
	friend void printMap(Snake& snake, Apple& apple);
	int x, y;
	int *defaults = new int[2]{5, 5};
	int getRandomNumber(int min, int max, const int* exclusions);
public:
	void Spawn(int* exclusions);
	Apple() {
		Spawn(defaults);
	}
};

#endif
