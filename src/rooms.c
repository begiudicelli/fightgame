#include "arenalib.h"

//GLOBAL IS EVIL BUT THAT'S OKAY

#define MAX_SPELLS 6

Enemy enemyPool[] = {
    {1, "Goblin", 30, 5},
    {2, "Orc", 50, 10},
    {3, "Troll", 80, 15},
    {4, "Dragon", 120, 20}
};

const int enemyPoolSize = sizeof(enemyPool) / sizeof(enemyPool[0]);

Spell createSpell(int id, const char *name, double damage, int cooldown) {
    Spell spell;
    spell.id = id;
    spell.name = strdup(name);
    spell.damage = damage;
    spell.cooldown = 0;
    spell.originalCooldown = cooldown;
    return spell;
}

Enemy* createEnemy(int id, const char *name, double health, double attack) {
    Enemy *newEnemy = (Enemy*) malloc(sizeof(Enemy));
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

Room* createRoom(int id, Enemy *enemy) {
    Room *newRoom = (Room*) malloc(sizeof(Room));
    newRoom->id = id;
    newRoom->enemy = *enemy;
    newRoom->next = NULL;
    return newRoom;
}

void addRoom(Room **head, int id, Enemy *enemy) {
    Room *newRoom = createRoom(id, enemy);
    if (*head == NULL) {
        *head = newRoom;
    } else {
        Room *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newRoom;
    }
}

void initializePlayerSpells(Player *player) {
    const char *spellNames[] = {"Fireball", "Lightning Bolt", "Healing Spell", "Icebolt", "Wind Slash", "Earthquake"};
    const double spellDamages[] = {25, 30, -20, 40, 15, 50};
    const int spellCooldowns[] = {1, 2, 5, 3, 1,8};

    player->spellCount = 0;
    for (int i = 0; i < MAX_SPELLS; i++) {
        player->spells[i] = createSpell(i + 1, spellNames[i], spellDamages[i], spellCooldowns[i]);
        player->spellCount++;
    }
}

void useSpell(Player *player, Enemy *enemy, int spellId) {
    for (int i = 0; i < player->spellCount; i++) {
        if (player->spells[i].id == spellId) {
            if (player->spells[i].cooldown > 0) {
                printf("O feitiço %s está em cooldown por %d turnos!\n", player->spells[i].name, player->spells[i].cooldown);
                return;
            }
            enemy->health -= player->spells[i].damage;
            printf("\n%s ataca %s com %s causando %.2f de dano! Vida do inimigo: %.2f\n",
                   player->name, enemy->name, player->spells[i].name, player->spells[i].damage, enemy->health);

            player->spells[i].cooldown = player->spells[i].originalCooldown;
            return;
        }
    }
}


void battle(Player *player, Enemy *enemy) {
    printf("%s aparece!  HP: %.2f | ATK: %.2f\n", enemy->name, enemy->health, enemy->attack);
    printf("%s esta vivo!  HP: %.2f | MP : %.2f\n", player->name, player->health, player->mana);

    while (player->health > 0 && enemy->health > 0) {
    	int spellId;
        do{
            printf("Spells: \n");
            for (int i = 0; i < player->spellCount; i++) {
                printf("ID: %d, Nome: %s, Dano: %.2f, Cooldown: %d\n",
                       player->spells[i].id, player->spells[i].name, player->spells[i].damage,
                       player->spells[i].cooldown);
            }


            printf("Escolha o id do spell: ");
            scanf("%d", &spellId);

            if(spellId < 1 || spellId > player->spellCount){
            	printf("ID invalido.Tente novamente\n");
            	continue;
            }

            useSpell(player, enemy, spellId);

            if (enemy->health <= 0) {
                printf("Voce derrotou %s!\n\n", enemy->name);
                return;
            }
        }while(spellId < 1 || spellId > player->spellCount || player->spells->cooldown < player->spells->originalCooldown);


        player->health -= enemy->attack;
        printf("%s ataca %s causando %.2f de dano! Vida do jogador: %.2f\n",
               enemy->name, player->name, enemy->attack, player->health);

        if (player->health <= 0) {
            printf("%s te matou!\n", enemy->name);
            return;
        }

        //atualiza o cooldown
        for(int i = 0; i < player->spellCount; i++){
        	if(player->spells[i].cooldown > 0){
        		player->spells[i].cooldown--;
        	}

        }
    }
}

void freeRoomList(Room *head) {
    while (head != NULL) {
        Room *temp = head;
        head = head->next;
        free(temp->enemy.name);
        free(temp);
    }
}

void gameLoop() {
    Room *roomList = NULL;
    Player player = {"Hero", 100, 100, {}, 0};

    initializePlayerSpells(&player);

    Enemy *initialEnemy = getRandomEnemy();
    addRoom(&roomList, 1, initialEnemy);

    while (player.health > 0) {
        Room *currentRoom = roomList;

        while (currentRoom != NULL) {
            battle(&player, &currentRoom->enemy);

            if (currentRoom->enemy.health <= 0) {
                Enemy *newEnemy = getRandomEnemy();
                addRoom(&roomList, currentRoom->id + 1, newEnemy);
                currentRoom = currentRoom->next;
            } else {
                break;
            }
        }

        if (roomList == NULL || player.health <= 0) {
            break;
        }
    }

    for (int i = 0; i < player.spellCount; i++) {
        free(player.spells[i].name);
    }
    freeRoomList(roomList);
}
