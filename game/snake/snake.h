#ifndef SNAKE_H
#define SNAKE_H

#include "../stuff/stuff.h"

class Game;
class Apple;

class Snake {
	friend Game;
	friend void printMap(Snake& snake, Apple& apple);
	struct Body {
		int x;
		int y;
		Body* next;
		Body(int x = 1, int y = 1, Body* next = nullptr) : x(x), y(y), next(next) {};
	};
	Body* head;
	Body* tail;
	Body* preTail;
	int Size;
public:
	Snake();

	void AddBody();

	void move(Apple& apple);

	void eatApple(Apple &apple);

	int* exclusions();

	int GetSize() { return Size; }
};

#endif
