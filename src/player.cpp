/*
 * player.cpp
 *
 *  Created on: 8.12.2012
 *      Author: iikka
 */

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include "asgård.h"
#include "map.h"
#include "game.h"
#include "player.h"

bool Player::tryMove(int x, int y) {
	bool success = Creature::tryMove(x, y);

	if (!success && game->map->inRange(x, y)) {
		mapNode * node = game->map->getNode(x, y);
		if (node->type == CREATURE) {
			game->message("You hit the " + node->creature->name() + ".");

			node->creature->takeDamage_Creature(this);
			if (node->type == CREATURE) game->drawShortCreatureInfo(node->creature);
			else {
				// TODO
			}
			return true;
		}
	}

	if (success && game->map->getNode(x, y)->type == ITEM_AND_CREATURE) {
		game->message("You see " + game->map->getNode(x, y)->item->name + ".");
	}

	return success;
}

void Player::takeDamage_Creature(Creature* enemy) {
	// TODO taiat, muut suojaavat vaikutukset
	int dmg = calculateDamage(enemy).damage;

	healt -= dmg;
	if (healt <= 0) {
		game->message(std::string("You died!"));
		healt = 0;
		die();
	} else {
		//game->message(std::string("You scream in pain!"));
	}
}

void Player::die() {
	// TODO häviäminen

	// temp

	attron(A_REVERSE);

	game->drawGame();
	getch();

	clear();
	attron(COLOR_PAIR(8));

	mvprintw(10, 10, "Game Over!");
	getch();
	running = false;
}

void Player::onTurn() {
	if (!running) return;
	bool legalCommand = true;
	do {
		game->drawGame();
		refresh();
		int ch = getch();
		switch (ch) {
		case '8':
		case KEY_UP:
			legalCommand = tryMove(x, y - 1);
			break;
		case '2':
		case KEY_DOWN:
			legalCommand = tryMove(x, y + 1);
			break;
		case '4':
		case KEY_LEFT:
			legalCommand = tryMove(x - 1, y);
			break;
		case '6':
		case KEY_RIGHT:
			legalCommand = tryMove(x + 1, y);
			break;
		case '7':
			legalCommand = tryMove(x - 1, y - 1);
			break;
		case '9':
			legalCommand = tryMove(x + 1, y - 1);
			break;
		case '1':
			legalCommand = tryMove(x - 1, y + 1);
			break;
		case '3':
			legalCommand = tryMove(x + 1, y + 1);
			break;
		case 'i': // 'inventory' näytä esineluettelo
			inv.showMenu("Inventory");
			legalCommand = false;
			break;
		case 'e': // 'equipment' näytä varusteet
			armor.showMenu("Equipment");
			legalCommand = false;
			break;
		case 'w':{ // 'wear' pane esine ylle (ase, vaate, yms)
			int item = inv.selectNotMenu(INVENTORY);	// vaihtaa esineet inventorystä ylle
			if (item != -1) {
				Item * i = inv.getItem(item);
				inv.setItem(item, armor.getItem(i->type));
				armor.setItem(i->type, i);

				game->message("You are now wearing " + i->name);
			}
			legalCommand = false;
		}
			break;
		case 't':{ // 'take off' ota esine pois yltä
				int item = armor.selectNotMenu(INVENTORY);	// vaihtaa esineen inventoryyn

				if (item != -1) {
					Item * i = armor.getItem(item);

					if (inv.addItem(i)) {
						armor.setItem(item, 0);
						game->message("You took off " + i->name);
					} else {
						game->message("Your inventory is full!");
					}
				}
				legalCommand = false;
			}
			break;
		case 'd':{ // 'drop' pudota esine
			int item = inv.selectNotMenu(N_item);
			if (item != -1) {
				Item * i = inv.getItem(item);
				if (game->map->getNode(x, y)->type != ITEM_AND_CREATURE) {
					inv.setItem(item, 0);
					game->map->addItem(x, y, i);

					game->message("You drop " + i->name);
				} else game->message("There's no space.");
			}
			legalCommand = false;
		}
			break;
		case 'g': // 'get' nouki esine maasta
			if (game->map->getNode(x, y)->type == ITEM_AND_CREATURE) { // jos maassa on esine
				Item * item = game->map->getNode(x, y)->item;
				if (inv.addItem(item)) { // jos esine mahtui inventoryyn
					game->map->getNode(x, y)->type = CREATURE; // poistetaan se maasta
					game->map->getNode(x, y)->item = 0;

					game->message("You have " + item->name + ".");
				} else {
					game->message("Your inventory is full!");
				}
			} else {
				game->message("There's nothing to collect.");
			}
			legalCommand = false;
			break;
		case 'R': // 'rest' älä tee mitään
		case '.':
			game->message("Resting...");
			legalCommand = true;
			break;
		case 's': // 's' save
			game->saveGame();
			game->message("Game saved!");
			legalCommand = false;
			break;
		case 'l': // 'log' näytä loki
			game->showMessageLog();
			legalCommand = false;
			break;
		case 'q':
		case 27: // ESC
			game->saveGame();
			running = false; // lopetetaan ohjelman suoritus
			goto out;
		default:
			legalCommand = false;
			break;
		}
	} while (!legalCommand);
	out:;
}

int Player::viewDistance() { // TODO
	if (armor.getItem(HAND2) != 0) {
		return armor.getItem(HAND2)->light;
	}
	return 1;
}

void Player::drawCreature() {
	addch('@');
}

bool Player::canSmell() {
	return true; // TODO
}

std::string Player::name() {
	return "player"; // TODO
}

int Player::getMaxHealt() {
	return 20; // TODO
}
