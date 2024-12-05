#include "arenalib.h"

Enemy enemyPool[] = {
    { 1, "Goblin", 30, 5, 10, 20, 1 },
    { 2, "Orc", 50, 10, 20, 40, 1 },
    { 3, "Troll", 80, 15, 40, 60, 1 },
    { 4, "Dragon", 120, 20, 80, 120, 1 },
    { 5, "Skeleton", 40, 8, 15, 30, 1 }
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

void adjustEnemyStatsForLevel(Player *player, Enemy *enemy) {
    double levelMultiplier = 1.0 + (0.1 * player->level);
    enemy->health *= levelMultiplier;
    enemy->attack *= levelMultiplier;
    enemy->experience *= levelMultiplier;
    enemy->gold *= levelMultiplier;
}

