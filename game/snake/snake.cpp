#include "snake.h"
#include "../apple/apple.h"
#include "../stuff/stuff.h"
#include <utility>

Snake::Snake() {
    	head = new Body();
		tail = head;
		preTail = head;
		Size = 1;
}

void Snake::AddBody() {
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

int* Snake::exclusions() {
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
