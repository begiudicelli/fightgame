#include "arenalib.h"

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	srand(time(NULL));

	gameLoop();

	system("pause");
	return 0;
}
