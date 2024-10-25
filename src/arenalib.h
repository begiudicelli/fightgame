#ifndef ARENALIB_H_
#define ARENALIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SPELLS 6
#define MAX_LEVEL 100

typedef struct Spell{
	int id;
	char *name;
	double damage;
	int cooldown;
	int originalCooldown;
}Spell;

typedef struct Player{
	char *name;
	double health;
	double mana;
	Spell spells[MAX_SPELLS];
	int spellCount;
	double gold;
	double experience;
	int level;
}Player;

typedef struct Enemy{
	int id;
	char *name;
	double health;
	double attack;
	double experience;
}Enemy;

typedef struct Room{
	int id;
	Enemy enemy;
	struct Room *next;
}Room;

Room* createRoom(int id, Enemy *enemy);
void addRoom(Room **head, int id, Enemy *enemy);
Enemy* createEnemy(int id, const char *name, double health, double attack, double experience);
Enemy* getRandomEnemy();
void battle(Player *player, Enemy *enemy);
void gameLoop();


void printStartFight(Room* currentRoom, Player *player);
void printSpellBar(Player *player);

//LEVEL
void defeatEnemy(Player *player, double enemyExperience);
void checkLevelUp(Player *player);
double calculateExperienceForLevel(int level);


#endif
