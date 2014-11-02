/*
 * ConfigLoader.h
 *
 *  Created on: 10.12.2012
 *      Author: iikka
 */

#ifndef CONFIGLOADER_H_
#define CONFIGLOADER_H_

#include <string>
#include <vector>
#include "item.h"

struct CreatureModel {
	std::string name;
	int picture;
	int maxhealth;
	int speed;
	int attack;
	int armor;
	int drop;
	int level;
	int flags;
};

struct ItemModel {
	std::string name;
	int picture;
	ItemPosition pos;
	int attack;
	int dice;
	int armor;
	int light;
	int level;
	int flags;
};

namespace ConfigLoader {

	std::vector<CreatureModel*> parseCreatures(std::string file);
	std::vector<ItemModel*> parseItems(std::string file);
};

#endif /* CONFIGLOADER_H_ */
