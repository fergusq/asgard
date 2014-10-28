/*
 * creature.cpp
 *
 *  Created on: 8.12.2012
 *      Author: iikka
 */

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include <string>
#include <stdlib.h>
#include "game.h"
#include "map.h"
#include "creature.h"
#include "player.h"
#include "mathlib.h"

Creature::Creature(AsgardGame*g) {
	game = g;
	x = 0;
	y = 0;
	healt = getMaxHealt();
	c_speed = 1;
}

Creature::~Creature() {

}

bool Creature::tryMove(int x, int y) {
	if (game->map->isFree(x, y)) {
		game->map->getNode(this->x, this->y)->type = game->map->getNode(this->x, this->y)->type==ITEM_AND_CREATURE?ITEM:FREE;
		game->map->getNode(this->x, this->y)->creature = 0;
		this->x = x;
		this->y = y;
		game->map->getNode(x, y)->type = CREATURE;
		game->map->getNode(x, y)->creature = this;
		return true;
	} else if (game->map->inRange(x, y) && game->map->getNode(x, y)->type == ITEM) {
		game->map->getNode(this->x, this->y)->type = game->map->getNode(this->x, this->y)->type==ITEM_AND_CREATURE?ITEM:FREE;
		game->map->getNode(this->x, this->y)->creature = 0;
		this->x = x;
		this->y = y;
		game->map->getNode(x, y)->type = ITEM_AND_CREATURE;
		game->map->getNode(x, y)->creature = this;
		return true;
	}
	else
		return false;
}

DamageTo Creature::calculateDamage(Creature * enemy) {
	// TODO lumotut esineet
	int helmet = 0;
	int body = 0;
	int boots = 0;

	if (armor.getItem(HEAD) != 0) {
		helmet = armor.getItem(HEAD)->armorPoints;
	}
	if (armor.getItem(BODY) != 0) {
		body = armor.getItem(BODY)->armorPoints;
	}
	if (armor.getItem(FOOT) != 0) {
		boots = armor.getItem(FOOT)->armorPoints;
	}

	int totalDR = ((helmet + boots) / 3 + body) / 3;
	if (totalDR == 0) totalDR = 1;

	int totalAR = 0;
	int maxAR = 0;

	for (int i = 0; i < 10; i++) {
		if (enemy->armor.getItem(i) == 0) continue;
		int harmPoints = enemy->armor.getItem(i)->attackPoints;
		int harm = 0;

		for (int j = 0; j < enemy->armor.getItem(i)->rollTimes; j++) {
			harm += rand() % harmPoints + 1;
		}

		totalAR += harm;
		maxAR += enemy->armor.getItem(i)->rollTimes*harmPoints;
	}

	if (rand()%20 < 4) totalAR = 0;

	if (totalAR <= totalDR) {
		totalAR = 0;
	}


	DamageTo dmg;
	dmg.damage = totalAR;
	dmg.maxDamage = maxAR;

	return dmg;
}

void Creature::takeDamage_Creature(Creature * enemy) {
	// TODO taiat, muut suojaavat vaikutukset
	DamageTo dmg0 = calculateDamage(enemy);

	int dmg = dmg0.damage;

	if (dmg == 0) game->message("Hit missed.");

	healt -= dmg;
	if (healt <= 0) {
		game->message("The " + name() + " died.");
		healt = 0;
		die();
	} else {
		if (dmg0.damage == dmg0.maxDamage) game->message("It was a great hit.");
		if (dmg0.maxDamage - dmg < 3) game->message("The " + name() + " screams in pain!");
	}
}

void Creature::takeDamage(int dmg) {
	// TODO armorit, taiat, muut suojaavat vaikutukset
	healt -= dmg;
	if (healt <= 0) {
		healt = 0;
		die();
	} else {

	}
}

void Creature::die() {
	game->map->getNode(this->x, this->y)->type = FREE;
	game->map->getNode(this->x, this->y)->creature = 0;
	game->map->removeCreature(this);
	delete this;
}

void Creature::tryMoveTo(int x, int y) {
	if (!tryMove(x, y)) {
		int r = rand()%2;
		if (r) {
			if (!tryMove(x, this->y))
				tryMove(this->x, y);
		} else {
			if (!tryMove(this->x, y)) {
				tryMove(x, this->y);
			}
		}
	}
}

void Creature::onTurn() { // TODO parempi tekoäky, hyökkäystaktiikat, yms


	if (x+nvec(game->player->x-x) == game->player->x && y+nvec(game->player->y-y) == game->player->y && (flags & A_HIT) && state == ATTACK) { // jos pelaaja on vieressä

		game->message("The " + name() + " hits you!");
		game->player->takeDamage_Creature(this); // lyö pelaajaa!
		return;
	}

	if (healt < getMaxHealt() / 2 && !(flags & AGGRESSIVE)) // jos elämiä on liian vähän JA olento ei ole aggressiivinen
	{
		if (state != ESCAPE) {
			game->message("The " + name() + " flees in terror!");
		}
		state = ESCAPE;
	}

	if (state == ESCAPE && healt == getMaxHealt()) state = ATTACK; // täysillä elämillä hyökkää

	if (state == ATTACK && !(flags & CANT_MOVE))
		tryMoveTo(x+
			nvec(nvec(game->player->x-x)+(rand()%3-1)), // satunnaisuus
			y+
			nvec(nvec(game->player->y-y)+(rand()%3-1))); // liiku pelaajaa kohti

	else if (state == ESCAPE && !(flags & CANT_MOVE))
		tryMoveTo(x-
			nvec(nvec(game->player->x-x)+(rand()%3-1)), // satunnaisuus
			y-
			nvec(nvec(game->player->y-y)+(rand()%3-1))); // liiku pakoon pelaajaa
}

void Creature::onUpdate() {
	if (rand()%10==0) healt++;
	if (healt > getMaxHealt()) healt = getMaxHealt();
}

bool Creature::canSee(int x, int y, int dis) {
	int sx = this->x;
	int sy = this->y;

	if (abs(x-sx) + abs(y-sy) > dis*2) return false;

	if (sx == x && sy == y) return true;

	return SMath::los(sx, sy, x, y, dis, game->map);
}

void Creature::drawCreature() {
	addch('c'|COLOR_PAIR(6)|A_BOLD);
}

int Creature::getHealt() {
	return healt;
}

bool Creature::canSmell() {
	return true; // TODO
}

int Creature::getMaxHealt() {
	return 10; // TODO
}

int Creature::speed() {
	return c_speed; // TODO
}

int Creature::picture() {
	return c_picture;
}

std::string Creature::name() {
	return std::string("creature"); // TODO
}
