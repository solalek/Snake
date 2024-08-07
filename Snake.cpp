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
class Game;

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

	int GetSize() { return Size; }
};

class Apple {
	friend Game;
	friend Snake;
	friend void printMap(Snake& snake, Apple& apple);
	int x, y;
	int *defaults = new int[2]{5, 5};
	int getRandomNumber(int min, int max, const int* exclusions) {
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
		if (current->y == 9) {
			current->y = 1;
		}
	}
	else if (dir == Directions::UP) {
		current->y--;
		if (current->y == 0) {
			current->y = 8;
		}
	}
	else if (dir == Directions::RIGHT) {
		current->x++;
		if (current->x == 18) {
			current->x = 1;
		}
	}
	else if (dir == Directions::LEFT) {
		current->x--;
		if (current->x == 0) {
			current->x = 17;
		}
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

class Game {
	double speed = CLOCKS_PER_SEC;

	void Start() {
		Snake snake;
		Apple apple;
		int time = clock();
		printMap(snake, apple);
		while (true) {
			if (GetAsyncKeyState('W') & 0x8000) {
				dir = Directions::UP;
			}
			if (GetAsyncKeyState('A') & 0x8000) {
				dir = Directions::LEFT;
			}
			if (GetAsyncKeyState('S') & 0x8000) {
				dir = Directions::DOWN;
			}
			if (GetAsyncKeyState('D') & 0x8000) {
				dir = Directions::RIGHT;
			}
			if ((clock() - time) / (speed) >= 1) {
				time = clock();
				snake.move(apple);
				printMap(snake, apple);
			}
		}
	}

	enum MainButtons {
		START = 1,
		OPTIONS,
		EXIT,
	};
	enum OptionsButtons {
		SPEED = 1,
		BACKOPT,
	};
	enum SpeedButtons {
		FAST = 1,
		MIDDLE,
		SLOW,
		BACKSPD,
	};
	
	int Main() {
		int choice;
		std::cout << "1. Start\n" << "2. Options\n" << "3. Exit\n" << "Enter: ";
		std::cin >> choice;
		return choice;
	}
	int Options() {
		int choice;
		std::cout << "1. Set the speed\n" << "2. Back\n" << "Enter: ";
		std::cin >> choice;
		return choice;
	}
	int Speed() {
		system("cls");
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
				system("cls");
				continue;
			case SpeedButtons::MIDDLE:
				speed = CLOCKS_PER_SEC;
				system("cls");
				continue;
			case SpeedButtons::SLOW:
				speed = CLOCKS_PER_SEC * 1.5;
				system("cls");
				continue;
			case SpeedButtons::BACKSPD:
				isRunning = false;
				break;
			}
		}
		return choice;
	}
public:
	void Run() {
		bool isRunning = true;
		while (isRunning) {
			int choice = Main();
			switch (choice) {
			case MainButtons::START:
				Start();
			case MainButtons::EXIT:
				return;
			case MainButtons::OPTIONS:
				system("cls");
				choice = Options();
				switch (choice) {
				case OptionsButtons::SPEED:
					Speed();
					system("cls");
					continue;
				case OptionsButtons::BACKOPT:
					system("cls");
					continue;
				}
			}
		}
	}

};

int main() {
	Game game;
	game.Run();
	return 0;
}