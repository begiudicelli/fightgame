#include "arenalib.h"

//GLOBAL IS EVIL BUT THATS OKAY

Enemy enemyPool[] = {
    {1, "Goblin", 30, 5, 1}, // ID 1 -> Fireball
    {2, "Orc", 50, 10, 2},   // ID 2 -> Lightning Bolt
    {3, "Troll", 80, 15, 3},  // ID 3 -> Healing Spell
    {4, "Dragon", 120, 20, 4}  // ID 4 -> Icebolt
};

Spell spells[] = {
    {1, "Fart", 25, NULL},
    {2, "Lightning Bolt", 30, NULL},
    {3, "Healing Spell", -20, NULL}, // Healing spell
    {4, "Icebolt", 40, NULL}
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
        *head = newRoom; // se a lista ta vazia, coloca a nova room no inicio da head
    } else {
        Room *current = *head;
        while (current->next != NULL) {
            current = current->next; //se move ate o final da lista
        }
        current->next = newRoom; // adiciona a nova sala pro final da list
    }
}

Enemy* createEnemy(int id, char *name, double health, double attack, int spellId){
	Enemy *newEnemy = (Enemy*) malloc (sizeof(Enemy));
	newEnemy->id = id;
	newEnemy->name = strdup(name);
	newEnemy->health = health;
	newEnemy->attack = attack;
	newEnemy->spellId = spellId;
	return newEnemy;
}

Enemy* getRandomEnemy() {
    int index = rand() % enemyPoolSize;
    return createEnemy(enemyPool[index].id, enemyPool[index].name,
                       enemyPool[index].health, enemyPool[index].attack,
                       enemyPool[index].spellId);
}

Spell *createSpell(int id, char *name, double damage){
	Spell *newSpell = (Spell*) malloc(sizeof(Spell));
	newSpell->id = id;
	newSpell-> name = strdup(name);
	newSpell->damage = damage;
	newSpell->next = NULL;
	return newSpell;
}

void addSpell(Player *player, Spell *spell) {
    if (player->spellBar == NULL) {
        player->spellBar = spell;
    } else {
        Spell *current = player->spellBar;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = spell;
    }
}

void useSpell(Player *player, Enemy *enemy, int spellId) {
    Spell *current = player->spellBar;
    while (current != NULL) {
        if (current->id == spellId) {
            enemy->health -= current->damage;
            printf("\n%s ataca %s com %s causando %.2f de dano! Vida do inimigo: %.2f\n",
                   player->name, current->name, enemy->name, current->damage, enemy->health);
            return;
        }
        current = current->next;
    }
}

void battle(Player *player, Enemy *enemy) {
    printf("%s aparece!  HP: %.2f | ATK: %.2f\n", enemy->name, enemy->health, enemy->attack);
    printf("%s esta vivo!  HP: %.2f | MP : %.2f\n", player->name, player->health, player->mana);
    while (player->health > 0 && enemy->health > 0) {
        printf("Spells: \n");
        Spell *currentSpell = player->spellBar;
        while (currentSpell != NULL) {
            printf("ID: %d, Nome: %s, Dano: %.2f\n", currentSpell->id, currentSpell->name, currentSpell->damage);
            currentSpell = currentSpell->next;
        }

        int spellId;
        printf("Escolha o id do spell: ");
        scanf("%d", &spellId);

        useSpell(player, enemy, spellId);

        if (enemy->health <= 0) {
            printf("Voce derrotou %s!\n\n", enemy->name);
            return;
        }

        player->health -= enemy->attack;
        printf("%s ataca %s causando %.2f de dano! Vida do jogador: %.2f\n",
               enemy->name, player->name, enemy->attack, player->health);

        if (player->health <= 0) {
            printf("%s te matou!\n", enemy->name);
            return;
        }
    }
}


void gameLoop() {
    Room *roomList = NULL;
    Spell *basicSpell = createSpell(0, "Fireball", 25);
    Player player = {"Hero", 100, 100, basicSpell};

    //cria as salas na ordem de inimigos do vetor
    for (int i = 0; i < enemyPoolSize; i++) {
        Enemy *enemy = createEnemy(enemyPool[i].id, enemyPool[i].name,
                                    enemyPool[i].health, enemyPool[i].attack,
                                    enemyPool[i].spellId);
        addRoom(&roomList, i + 1, enemy);
    }

    while (player.health > 0) {
        Room *currentRoom = roomList;

        while (currentRoom != NULL) {
            battle(&player, &currentRoom->enemy);

            if (currentRoom->enemy.health <= 0) {
            	//variaveis p guardar o spell id que o inimigo guarda
                int gainedSpellId = currentRoom->enemy.spellId;
                Spell *newSpell = NULL;

                //acha o spell correspondente do inimigo derrotado(enums?)
                for (int i = 0; i < sizeof(spells) / sizeof(spells[0]); i++) {
                    if (spells[i].id == gainedSpellId) {
                        newSpell = createSpell(spells[i].id, spells[i].name, spells[i].damage);
                        break;
                    }
                }

                //depois que acha o spell adiciona
                if (newSpell != NULL) {
                    addSpell(&player, newSpell);
                }

                currentRoom = currentRoom->next;
            } else {
                break; // derrota
            }
        }

        if (roomList == NULL || player.health <= 0) {
            break; // fim do jogo
        }
    }
}


