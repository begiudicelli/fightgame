#include "arenalib.h"

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	srand(time(NULL));

	Room *roomList = NULL;
	Player *player = createPlayer("Belnaldo", 100, 60);

	char opt;
	do {
		player->health = player->maxHealth;
		listItems(player->inventory);
		printf("Jogador: %s | Level: %d | Experiencia: %.2f | Gold: %.2f | Max HP: %.2f | Max MP: %.2f\n",
				player->name, player->level, player->experience, player->gold,
				player->maxHealth, player->mana);
		printf("Opcoes: \n");
		printf("A - Lutar\n");
		printf("B - Shop\n");
		printf("C - Equipar items\n");
		printf("D - Listar itens equipados\n");
		printf("G - Finalizar\n");
		printf("Escolha uma opcao: ");
		scanf(" %c", &opt);
		opt = toupper(opt);

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
						printf("Sala atingida: %d Limpando a lista de salas.\n", currentRoom->id);
						freeRoomList(currentRoom);
						roomList = NULL;
						break;
					}
				}
			}
			break;
		case 'B':
			displayShop(player);
			break;
		case 'C':
			equipItemFromInventory(player);
			break;
		case 'D':
			listEquippedItems(player);
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

