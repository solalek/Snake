#include "apple.h"
#include <stdlib.h>
#include <random>
#include <unistd.h>
#include <termios.h>
#include <ctime>


int Apple::getRandomNumber(int min, int max, const int* exclusions) {
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

void Apple::Spawn(int *exclusions) {
		x = getRandomNumber(1, 17, exclusions);
		y = getRandomNumber(1, 8, exclusions);
		delete exclusions;
}
