#include <iostream>
#include <ctime>
#include <termios.h>
#include "stuff/stuff.h"

class Game {
	double speed = CLOCKS_PER_SEC;

	void Start();

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
	
	int Main();
	int Options();
	int Speed();
public:
	void Run();
};


