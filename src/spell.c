#include "arenalib.h"

Spell createSpell(int id, const char *name, double damage, int cooldown) {
	Spell spell;
	spell.id = id;
	spell.name = strdup(name);
	spell.damage = damage;
	spell.cooldown = 0;
	spell.originalCooldown = cooldown;
	return spell;
}

bool useSpell(Player *player, Enemy *enemy, int spellId) {
    if (spellId < 0 || spellId >= player->spellCount) {
        printf("ID de feitico invalido!\n");
        return false;
    }

    if (player->spells[spellId].cooldown > 0) {
        printf("O feitico %s esta em cooldown por %d turnos!\n",
                player->spells[spellId].name, player->spells[spellId].cooldown);
        return false;
    }

    double randomChance = (double) rand() / RAND_MAX;

    double finalDamage = player->spells[spellId].damage;
    if (randomChance <= player->critChance) {
        finalDamage *= 2.0;
        printf("CRITICO! O feitico causou %.2f de dano!\n", finalDamage);
    }

    enemy->health -= finalDamage;
    printf("\n%s ataca %s com %s causando %.2f de dano! Vida do inimigo: %.2f\n",
           player->name, enemy->name, player->spells[spellId].name,
           finalDamage, enemy->health);

    player->spells[spellId].cooldown = player->spells[spellId].originalCooldown + 1;

    return true;
}
