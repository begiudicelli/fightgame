#include "arenalib.h"

void battle(Player *player, Enemy *enemy) {
	while (player->health > 0 && enemy->health > 0) {
		int spellId;
		bool used = false;

		printSpellBar(player);
		printf("Escolha o id do spell: ");
		scanf("%d", &spellId);

		used = useSpell(player, enemy, spellId);

		if (enemy->health <= 0) {
			printf("Você derrotou %s!\n\n", enemy->name);
			defeatEnemy(player, enemy);
			for (int i = 0; i < player->spellCount; i++) { // cleans cooldown
				player->spells[i].cooldown = 0;
			}
			return;
		}
		if (used) {
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
				for (int i = 0; i < player->spellCount; i++) { // cleans cooldown se morrer tb
					player->spells[i].cooldown = 0;
				}
				return;
			}
		}

	}
}

void defeatEnemy(Player *player, Enemy *enemy) {
	player->experience += enemy->experience;
	player->gold += enemy->gold;

	dropItem(player);

	printf("Você ganhou %.2f XP! Total: %.2f XP\n", enemy->experience,
			player->experience);
	printf("Você ganhou %.2f GOLD! Total: %.2f GOLD\n", enemy->gold,
			player->gold);

	checkLevelUp(player);
}


void printStartFight(Room *currentRoom, Player *player) {
	printf("%s HP: %.2f | MP: %.2f\n", player->name, player->health,
			player->mana);

	printf("%s aparece! HP: %.2f | ATK: %.2f\n", currentRoom->enemy.name,
			currentRoom->enemy.health, currentRoom->enemy.attack);
}

void printSpellBar(Player *player) {
	printf("Spells: \n");
	for (int i = 0; i < player->spellCount; i++) {
		printf("ID: %d, Nome: %s, Dano: %.2f, Cooldown: %d\n",
				player->spells[i].id, player->spells[i].name,
				player->spells[i].damage, player->spells[i].cooldown);
	}
}
