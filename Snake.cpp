#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <string>
#include <random>

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

void set_cursor(int x = 0, int y = 0)
{
	HANDLE handle;
	COORD coordinates;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	coordinates.X = x;
	coordinates.Y = y;
	SetConsoleCursorPosition(handle, coordinates);
}

class Apple;

class Snake {
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
	Snake() {
		head = new Body();
		tail = head;
		preTail = head;
		Size = 1;
	}

	void AddBody() {
		int new_x = tail->x;
		int new_y = tail->y;

		if (Size == 1) {
			switch (dir) {
			case Directions::DOWN:
				new_y -= 1;
				break;
			case Directions::UP:
				new_y += 1;
				break;
			case Directions::LEFT:
				new_x += 1;
				break;
			case Directions::RIGHT:
				new_x -= 1;
				break;
			}
		}
		else {
			if (preTail->x == tail->x) {
				new_y = tail->y + (preTail->y < tail->y ? 1 : -1);
			}
			else if (preTail->y == tail->y) {
				new_x = tail->x + (preTail->x < tail->x ? 1 : -1);
			}
		}

		tail->next = new Body(new_x, new_y);
		preTail = tail;
		tail = tail->next;
		Size++;
	}

	void move(Apple& apple);

	void eatApple(Apple &apple);

	int* exclusions() {
		int* nums = new int[Size * 2 + 1];
		Body* current = head;
		nums[0] = Size * 2;
		for (int i = 1; current != nullptr && i < Size * 2; i+=2) {
			nums[i] = current->x;
			nums[i + 1] = current->y;
			current = current->next;
		}
		return nums;
	}
};

class Apple {
	friend Snake;
	friend void printMap(Snake& snake, Apple& apple);
	int x, y;
	int *defaults = new int[2]{5, 5};
	int getRandomNumber(int min, int max, const int* exclusions) {
		/*std::srand(std::time(0));
		int randomNum = rand() % (max - min + 1) + min;
		for (int i = 1; i < exclusions[0] + 1; i++) {
			if (randomNum == exclusions[i]) {
				randomNum = rand() % (max - min + 1) + min;
				i = 1;
			}
		}
		return randomNum;*/
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(min, max);

		int randomNum;
		bool isExcluded;

		do {
			randomNum = dis(gen);
			isExcluded = false;
			for (int i = 0; i < exclusions[0]; ++i) {
				if (randomNum == exclusions[i]) {
					isExcluded = true;
					break;
				}
			}
		} while (isExcluded);

		return randomNum;
	}
public:
	void Spawn(int* exclusions) {
		x = getRandomNumber(1, 17, exclusions);
		y = getRandomNumber(1, 8, exclusions);
		delete exclusions;
	}
	Apple() {
		Spawn(defaults);
	}
};

void Snake::move(Apple& apple) {
	Body* current = head;
	int tempX = current->x, tempY = current->y;
	if (dir == Directions::DOWN) {
		current->y++;
	}
	else if (dir == Directions::UP) {
		current->y--;
	}
	else if (dir == Directions::RIGHT) {
		current->x++;
	}
	else if (dir == Directions::LEFT) {
		current->x--;
	}
	current = current->next;
	while (current != nullptr) {
		std::swap(current->x, tempX);
		std::swap(current->y, tempY);
		current = current->next;
	}
	if (head->x == apple.x and head->y == apple.y) {
		eatApple(apple);
	}
}

void Snake::eatApple(Apple& apple) {
	AddBody();
	apple.Spawn(exclusions());
}

void printMap(Snake &snake, Apple &apple) {
	system("cls");
	Snake::Body* snakeBody = snake.head;
	map[apple.y][apple.x] = '@';
	while (snakeBody != nullptr) {
		map[snakeBody->y][snakeBody->x] = '0';
		snakeBody = snakeBody->next;
	}
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

int main() {
	Snake snake;
	Apple apple;
	int time = clock();
	printMap(snake, apple);
	while (true) {
		if (GetKeyState('W') & 0x8000) {
			dir = Directions::UP;
		}
		if (GetKeyState('A') & 0x8000) {
			dir = Directions::LEFT;
		}
		if (GetKeyState('S') & 0x8000) {
			dir = Directions::DOWN;
		}
		if (GetKeyState('D') & 0x8000) {
			dir = Directions::RIGHT;
		}
		if ((clock() - time) / (CLOCKS_PER_SEC/2) >= 0.5) {
			time = clock();
			snake.move(apple);
			printMap(snake, apple);
		}
	}
	return 0;
}