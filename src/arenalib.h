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
#define MAX_SHOP_ITEMS 3

typedef struct Item {
    int id;
    char *name;
    float dropChance;
    float value;
    int amount;
    struct Item *next;
} Item;

typedef struct Inventory {
    Item *head;
    int itemCount;
} Inventory;

typedef struct ShopItem {
    Item item;
    float price;
    int type;
} ShopItem;

typedef struct EquippedItem{
	Item *wand;
	Item *hat;
	Item *spellbook;
}EquippedItem;

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
	EquippedItem equippedItems;
	bool purchasedItems[MAX_SHOP_ITEMS];
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
void defeatEnemy(Player *player, Enemy *enemy);
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
void dropItem(Player *player);
void displayShop(Player *player);

void equippItem(Player *player, Item *item);
void displayEquippableItems(Player *player);
void listEquippedItems(Player *player);
void equipItemFromInventory(Player *player);

#endif
