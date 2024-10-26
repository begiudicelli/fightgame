#include "arenalib.h"

//GLOBAL IS EVIL BUT THAT'S OKAY
Enemy enemyPool[] = {
		{ 1, "Goblin", 30, 5, 10, 20 },
		{ 2, "Orc", 50, 10, 20, 40 },
		{ 3,"Troll", 80, 15, 40, 60 },
		{ 4, "Dragon", 120, 20, 80, 120 }
};

const int enemyPoolSize = sizeof(enemyPool) / sizeof(enemyPool[0]);

Enemy* createEnemy(int id, const char *name, double health, double attack, double experience, double gold) {
	Enemy *newEnemy = (Enemy*) malloc(sizeof(Enemy));
	newEnemy->id = id;
	newEnemy->name = strdup(name);
	newEnemy->health = health;
	newEnemy->attack = attack;
	newEnemy->experience = experience;
	newEnemy->gold = gold;
	return newEnemy;
}

Enemy* getRandomEnemy() {
	int index = rand() % enemyPoolSize;
	return createEnemy(enemyPool[index].id, enemyPool[index].name,
			enemyPool[index].health, enemyPool[index].attack, enemyPool[index].experience, enemyPool[index].gold);
}
