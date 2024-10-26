#include "arenalib.h"

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	srand(time(NULL));

	Room *roomList = NULL;
	Player *player = createPlayer("Belnaldo", 100, 60);

	initializePlayerSpells(player);

	char opt;
	do {
		player->health = player->maxHealth;
		printf("Jogador: %s | Level: %d | Experiencia: %.2f | Gold: %.2f | Max HP: %.2f | Max MP: %.2f\n",
				player->name, player->level, player->experience, player->gold,
				player->maxHealth, player->mana);
		printf("Opcoes: \n");
		printf("A - Lutar\n");
		printf("B - Shop\n");
		printf("G - Finalizar\n");
		printf("Escolha uma opcao: ");
		scanf(" %c", &opt);

		switch (opt) {
		case 'A':
			while (player->health > 0) {
				Room *currentRoom = roomList;
				Enemy *initialEnemy = getRandomEnemy();
				addRoom(&roomList, 1, initialEnemy);

				while (currentRoom != NULL) {
					if (currentRoom->enemy.health > 0) {
						printStartFight(currentRoom, player);
					}
					battle(player, &currentRoom->enemy);
					if (currentRoom->enemy.health <= 0) { // WIN
						Enemy *newEnemy = getRandomEnemy();
						addRoom(&roomList, currentRoom->id + 1, newEnemy);
						currentRoom = currentRoom->next;
					} else {
						printf("Você morreu! Limpando a lista de salas...\n");
						freeRoomList(currentRoom);
						roomList = NULL;
						break;
					}
				}
			}
			break;
		case 'B':
			break;
		case 'C':
			break;
		case 'G':
			printf("\nPrograma finalizado.");
			break;
		default:
			printf("\nOpção inválida.");
			break;
		}

	} while (opt != 'G');

	system("pause");
	return 0;
}

