#include "arenalib.h"

//GLOBAL IS EVIL BUT THAT'S OKAY

#define MAX_SPELLS 6

Enemy enemyPool[] = { { 1, "Goblin", 30, 5 }, { 2, "Orc", 50, 10 }, { 3,
		"Troll", 80, 15 }, { 4, "Dragon", 120, 20 } };

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

Player* createPlayer(const char *name, double health, double mana) {
	Player *newPlayer = (Player*) malloc(sizeof(Player));
	newPlayer->name = strdup(name);
	newPlayer->health = health;
	newPlayer->mana = mana;
	newPlayer->spellCount = 0;
	return newPlayer;
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
	const char *spellNames[] = { "Fireball", "Lightning Bolt", "Healing Spell",
			"Icebolt", "Wind Slash", "Earthquake" };
	const double spellDamages[] = { 25, 30, -20, 40, 15, 50 };
	const int spellCooldowns[] = { 1, 2, 5, 3, 1, 8 };

	player->spellCount = 0;
	for (int i = 0; i < MAX_SPELLS; i++) {
		player->spells[i] = createSpell(i, spellNames[i], spellDamages[i],
				spellCooldowns[i]);
		player->spellCount++;
	}
}

bool useSpell(Player *player, Enemy *enemy, int spellId) {
	if (spellId < 0 || spellId >= player->spellCount) {
		printf("ID de feitiço inválido!\n");
		return false;
	}

	if (player->spells[spellId].cooldown > 0) {
		printf("O feitiço %s está em cooldown por %d turnos!\n",
				player->spells[spellId].name, player->spells[spellId].cooldown);
		return false;
	}

	enemy->health -= player->spells[spellId].damage;
	printf("\n%s ataca %s com %s causando %.2f de dano! Vida do inimigo: %.2f\n",
			player->name, enemy->name, player->spells[spellId].name,
			player->spells[spellId].damage, enemy->health);

	player->spells[spellId].cooldown = player->spells[spellId].originalCooldown + 1;

	return true;
}

void battle(Player *player, Enemy *enemy) {
	while (player->health > 0 && enemy->health > 0) {
		int spellId;
		bool used = false;
		printf("Spells: \n");
		for (int i = 0; i < player->spellCount; i++) {
			printf("ID: %d, Nome: %s, Dano: %.2f, Cooldown: %d\n",
					player->spells[i].id, player->spells[i].name,
					player->spells[i].damage, player->spells[i].cooldown);
		}

		printf("Escolha o id do spell: ");
		scanf("%d", &spellId);

		used = useSpell(player, enemy, spellId);

		if (enemy->health <= 0) {
			printf("Você derrotou %s!\n\n", enemy->name);
			for (int i = 0; i < player->spellCount; i++) { // cleans cooldown
					player->spells[i].cooldown = 0;
			}
			return;
		}

		if(used){
			player->health -= enemy->attack;
			printf("%s ataca %s causando %.2f de dano! Vida do jogador: %.2f\n",
					enemy->name, player->name, enemy->attack, player->health);

			for (int i = 0; i < player->spellCount; i++) { // depois que ataca, diminue todos os cooldowns por 1
				if (player->spells[i].cooldown > 0) {
					player->spells[i].cooldown--;
				}
			}

			if (player->health <= 0) {
				printf("%s te matou!\n", enemy->name);
				return;
			}
		}

	}
}
void gameLoop() {
	Room *roomList = NULL;
	Player *player = createPlayer("Belnaldo", 100, 60);

	initializePlayerSpells(player);

	Enemy *initialEnemy = getRandomEnemy();
	addRoom(&roomList, 1, initialEnemy);

	char opt;
	do {
		player->health = 100;

		printf("\nMenu Principal: \n");
		printf("A - Lutar\n");
		printf("B - Shop\n");
		printf("G - Finalizar\n");
		printf("Escolha uma opcao: ");
		scanf(" %c", &opt);

		switch (opt) {
		case 'A':
			while (player->health > 0) {
				Room *currentRoom = roomList;
				while (currentRoom != NULL) {
					if (currentRoom->enemy.health > 0) {
						printStartFight(currentRoom, player);
					}
					battle(player, &currentRoom->enemy);
					if (currentRoom->enemy.health <= 0) { // WIN
						Enemy *newEnemy = getRandomEnemy();
						addRoom(&roomList, currentRoom->id + 1, newEnemy);
						currentRoom = currentRoom->next;
					} else { // DIES
						break;
					}
				}
			}
			break;
		case 'B':
			break;
		case 'G':
			printf("\nPrograma finalizado.");
			break;
		default:
			printf("\nOpção inválida.");
			break;
		}

	} while (opt != 'G');
}

void printStartFight(Room *currentRoom, Player *player) {
	printf("Seu HP: %.2f | MP: %.2f\n", player->health, player->mana);

	printf("%s aparece! HP: %.2f | ATK: %.2f\n", currentRoom->enemy.name,
			currentRoom->enemy.health, currentRoom->enemy.attack);
}

void freeRoomList(Room *head) { // just in case
	while (head != NULL) {
		Room *temp = head;
		head = head->next;
		free(temp->enemy.name);
		free(temp);
	}
}

