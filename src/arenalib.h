#ifndef ARENALIB_H_
#define ARENALIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SPELLS 6

typedef struct Spell{
	int id;
	char *name;
	double damage;
}Spell;

typedef struct Player{
	char *name;
	double health;
	double mana;
	Spell spells[MAX_SPELLS];
	int spellCount;
}Player;

typedef struct Enemy{
	int id;
	char *name;
	double health;
	double attack;
}Enemy;

typedef struct Room{
	int id;
	Enemy enemy;
	struct Room *next;
}Room;

Room* createRoom(int id, Enemy *enemy);
void addRoom(Room **head, int id, Enemy *enemy);
Enemy* createEnemy(int id, const char *name, double health, double attack);
Enemy* getRandomEnemy();
void battle(Player *player, Enemy *enemy);
void gameLoop();

#endif
