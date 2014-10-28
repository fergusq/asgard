/*
 * item.h
 *
 *  Created on: 10.12.2012
 *      Author: iikka
 */

#ifndef ITEM_H_
#define ITEM_H_

struct ItemModel;

#include <string>

enum itemFlags {
	POTION=0x02
};

const char* const STR_POTION= "POTION";

enum ItemPosition {
	N_item=0,
	HEAD=1,
	BODY=2,
	FOOT=3,
	HAND1=4,
	HAND2=5,
	INVENTORY=6,
	RING1=7,
	RING2=8,
	NECK=9
};

struct DamageTo {
	int damage;
	int maxDamage;
};

class Item {
public:
	Item(std::string name, int picture, ItemPosition type, int, int, int, int, int);
	Item(ItemModel*);
	virtual ~Item();

	virtual void drawItem();

	ItemPosition type;
	int attackPoints;
	int rollTimes;
	int armorPoints;
	int light;
	std::string name;

	int picture;

	int flags;
};

#endif /* ITEM_H_ */
