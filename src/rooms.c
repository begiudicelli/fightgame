#include "arenalib.h"

//GLOBAL IS EVIL BUT THATS OKAY

Enemy enemyPool[] = {
	    {1, "Goblin", 30, 5},
	    {2, "Orc", 50, 10},
	    {3, "Troll", 80, 15},
	    {4, "Dragon", 120, 20}
};

const int enemyPoolSize = sizeof(enemyPool) / sizeof(enemyPool[0]);


Room* createRoom(int id, Enemy *enemy){
	Room *newRoom = (Room*) malloc (sizeof(Room));
	newRoom->id = id;
	newRoom->enemy = *enemy;
	newRoom->next = NULL;
	return newRoom;
}

void addRoom(Room **head, int id, Enemy *enemy) {
    Room *newRoom = createRoom(id, enemy);

    if (*head == NULL) {
        *head = newRoom; // If the list is empty, set the new room as the head
    } else {
        Room *current = *head;
        while (current->next != NULL) {
            current = current->next; // Traverse to the end of the list
        }
        current->next = newRoom; // Add the new room at the end
    }
}

Enemy* createEnemy(int id, char *name, double health, double attack){
	Enemy *newEnemy = (Enemy*) malloc (sizeof(Enemy));
	newEnemy->id = id;
	newEnemy->name = strdup(name);
	newEnemy->health = health;
	newEnemy->attack = attack;
	return newEnemy;
}

Enemy* getRandomEnemy() {
    int index = rand() % enemyPoolSize;
    return createEnemy(enemyPool[index].id, enemyPool[index].name,
                       enemyPool[index].health, enemyPool[index].attack);
}

void battle(Player *player, Enemy *enemy) {
    printf("A wild %s appears!\n", enemy->name);

    while (player->health > 0 && enemy->health > 0) {
        // Player attacks enemy
        enemy->health -= player->attack;
        printf("%s attacks %s for %.2f damage! Enemy health: %.2f\n",
               player->name, enemy->name, player->attack, enemy->health);

        if (enemy->health <= 0) {
            printf("You defeated %s!\n", enemy->name);
            return;
        }

        // Enemy attacks player
        player->health -= enemy->attack;
        printf("%s attacks %s for %.2f damage! Player health: %.2f\n",
               enemy->name, player->name, enemy->attack, player->health);

        if (player->health <= 0) {
            printf("You have been defeated by %s!\n", enemy->name);
            return;
        }
    }
}

void gameLoop() {
    Room *roomList = NULL;
    Player player = {"Hero", 100, 10};

    // Add the first room with a random enemy
    Enemy *initialEnemy = getRandomEnemy();
    addRoom(&roomList, 1, initialEnemy);

    while (player.health > 0) {
        Room *currentRoom = roomList;

        while (currentRoom != NULL) {
            battle(&player, &currentRoom->enemy);

            if (currentRoom->enemy.health <= 0) {
                Enemy *newEnemy = getRandomEnemy(); // Get a new random enemy
                addRoom(&roomList, currentRoom->id + 1, newEnemy); // Increment room ID
                currentRoom = currentRoom->next; // Move to the next room
            } else {
                break; // Exit if player loses
            }
        }

        if (roomList == NULL || player.health <= 0) {
            break; // End the game
        }
    }
}


