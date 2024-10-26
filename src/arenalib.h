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

typedef struct Item {
    int id;
    char *name;
    int amount;
    struct Item *next;
} Item;

typedef struct Inventory {
    Item *head;
    int itemCount;
} Inventory;

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
	double maxHealth;
	double mana;
	Spell spells[MAX_SPELLS];
	int spellCount;
	double gold;
	double experience;
	int level;
	Inventory *inventory;
}Player;

typedef struct Enemy{
	int id;
	char *name;
	double health;
	double attack;
	double experience;
	double gold;
}Enemy;

typedef struct Room{
	int id;
	Enemy enemy;
	struct Room *next;
}Room;

//Player
Player* createPlayer(const char *name, double health, double mana);
void initializePlayerSpells(Player *player);
void defeatEnemy(Player *player, double enemyExperience, double enemyGold);
void checkLevelUp(Player *player);
double calculateExperienceForLevel(int level);

//Enemy
Enemy* createEnemy(int id, const char *name, double health, double attack, double experience, double gold);
Enemy* getRandomEnemy();

//Spells
Spell createSpell(int id, const char *name, double damage, int cooldown);
bool useSpell(Player *player, Enemy *enemy, int spellId);

//Rooms
Room* createRoom(int id, Enemy *enemy);
void addRoom(Room **head, int id, Enemy *enemy);
void freeRoomList(Room *head);

//Battle
void battle(Player *player, Enemy *enemy);
void printStartFight(Room* currentRoom, Player *player);
void printSpellBar(Player *player);

//Item
Inventory* createInventory();
void addItem(Inventory *inventory, int id, const char *name);
bool removeItem(Inventory *inventory, int id);
void listItems(Inventory *inventory);
Item getRandomItem();


#endif
