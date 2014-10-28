/*
 * item.cpp
 *
 *  Created on: 10.12.2012
 *      Author: iikka
 */

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include "item.h"
#include "ConfigLoader.h"

Item::Item(std::string name, int picture, ItemPosition type, int attack, int dice, int armor, int light, int flags) {
	this->name = name;
	this->type = type;
	this->attackPoints = attack;
	this->rollTimes = dice;
	this->armorPoints = armor;
	this->picture = picture;
	this->light = light;
	this->flags = flags;
}

Item::Item(ItemModel * m) {
	this->name = m->name;
	this->type = m->pos;
	this->attackPoints = m->attack;
	this->rollTimes = m->dice;
	this->armorPoints = m->armor;
	this->picture = m->picture;
	this->light = m->light;
	this->flags = m->flags;
}

Item::~Item() {

}

void Item::drawItem() {
	addch(picture);
}
