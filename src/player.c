#include "arenalib.h"

Player* createPlayer(const char *name, double health, double mana) {
	Player *newPlayer = (Player*) malloc(sizeof(Player));
	newPlayer->name = strdup(name);
	newPlayer->health = health;
	newPlayer->maxHealth = health;
	newPlayer->mana = mana;
	newPlayer->spellCount = 0;
	newPlayer->experience = 0;
	newPlayer->gold = 0;
	newPlayer->level = 1;
	return newPlayer;
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

double calculateExperienceForLevel(int level) {
    return level * level * 100; // (nível^2 * 100)
}

void checkLevelUp(Player *player) {
    while (player->level < MAX_LEVEL && player->experience >= calculateExperienceForLevel(player->level)) {
        player->level++;
        printf("Parabéns! Você subiu para o nível %d!\n", player->level);
        //add gains
        player->maxHealth += 20;
    }
}

