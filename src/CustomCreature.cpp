/*
 * CustomCreature.cpp
 *
 *  Created on: 9.12.2012
 *      Author: iikka
 */

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include "CustomCreature.h"
#include "item.h"
#include "ConfigLoader.h"

CustomCreature::CustomCreature(AsgardGame *g, std::string name, int picture, int max_healt, int speed, int attack_strength) : Creature(g) {
	c_name = name;
	c_picture = picture;
	c_maxhealt = max_healt;
	healt = getMaxHealt();
	c_speed = speed;

	armor.setItem(HAND1, new Item("attack", '@', HAND1, attack_strength, 1, 0, 0, 0));
}

CustomCreature::CustomCreature(AsgardGame *g, CreatureModel * m) : Creature(g) {
	c_name = m->name;
	c_picture = m->picture;
	c_maxhealt = m->maxhealt;
	healt = getMaxHealt();
	c_speed = m->speed;
	flags = m->flags;

	armor.setItem(HAND1, new Item("attack", '@', HAND1, m->attack, 1, 0, 0, 0));
	armor.setItem(BODY, new Item("armor", '@', BODY, 0, 0, m->armor, 0, 0));
}

CustomCreature::~CustomCreature() {
}

std::string CustomCreature::name() {
	return c_name;
}

void CustomCreature::drawCreature() {
	addch(c_picture);
}

int CustomCreature::getMaxHealt() {
	return c_maxhealt;
}
