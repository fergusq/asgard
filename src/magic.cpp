#include "magic.h"
#include "game.h"
#include "creature.h"
#include "map.h"

bool spell_fireball(AsgardGame* game, int x, int y) {
	if (game->map->getNode(x, y)->type == CREATURE) {
		DamageTo dmg;
		dmg.damage = 10;
		dmg.maxDamage = 10;

		game->map->getNode(x, y)->creature->takeDamage(dmg, "Fireball missed.", "");
		return true;
	}
	else {
		game->message("Spell not casted.");
		return false;
	}
}
