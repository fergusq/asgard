/*
 * inventory.h
 *
 *  Created on: 10.12.2012
 *      Author: iikka
 */

#ifndef INVENTORY_H_
#define INVENTORY_H_

class Item;

#include "item.h"

class Inventory {
public:
	Inventory();
	virtual ~Inventory();

	Item * getItem(int pos);
	void setItem(int pos, Item * item);

	bool addItem(Item * item);

	int selectMenu(ItemPosition);
	int selectNotMenu(ItemPosition);
	void showMenu(std::string);
private:
	Item *(items[10]);
};

#endif /* INVENTORY_H_ */
