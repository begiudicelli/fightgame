#ifndef ARENALIB_H_
#define ARENALIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

typedef struct Spell{
	int id;
	char *name;
	int damage;
	struct Spell *next;
}Spell;

typedef struct Player{
	char *name;
	double health;
	double attack;
	//Spell *SpellBar;
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
Enemy* createEnemy(int id, char *name, double health, double attack);
Enemy* getRandomEnemy();
void battle(Player *player, Enemy *enemy);
void gameLoop();

#endif
