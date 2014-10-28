/*
 * inventory.cpp
 *
 *  Created on: 10.12.2012
 *      Author: iikka
 */

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include "inventory.h"
#include "item.h"

Inventory::Inventory() {
	for (int i = 0; i < 10; i++) {
		items[i] = 0;
	}
}

Inventory::~Inventory() {
	for (int i = 0; i < 10; i++) {
		delete items[i];
	}
}

Item * Inventory::getItem(int pos) {
	if (pos >= 10) return 0;
	return items[pos];
}

void Inventory::setItem(int pos, Item * item) {
	if (pos < 10) items[pos] = item;
}

bool Inventory::addItem(Item * item) {
	int i = -1;
	while (i < 9 && items[++i] != 0);
	if (items[i] == 0) {
		items[i] = item;
		return true;
	}
	else return false;
}

int Inventory::selectMenu(ItemPosition ip) {
	mvprintw(0, 28, "  Select item.\t\t");
	int p = 1;
	for (int i = 0; i < 10; i++) {
		if (items[i] != 0 && items[i]->type == ip) mvprintw(p++, 28, "  %d) %s \t\t", i, items[i]->name.c_str());
	}
	int key = 0;
	while ((key=getch()) < 48 || key > 57) if (key == 27 || key == 'q') return -1;
	return key-48;
}

int Inventory::selectNotMenu(ItemPosition ip) {
	mvprintw(0, 28, "  Select item.\t\t");
	int p = 1;
	for (int i = 0; i < 10; i++) {
		if (items[i] != 0 && items[i]->type != ip) mvprintw(p++, 28, "  %d) %s \t\t", i, items[i]->name.c_str());
	}
	int key = 0;
	while ((key=getch()) < 48 || key > 57) if (key == 27 || key == 'q') return -1;
	return key-48;
}

void Inventory::showMenu(std::string message) {

	mvprintw(0, 28, ("  "+message+"\t\t").c_str());
	int p = 1;
	for (int i = 0; i < 10; i++) {
		if (items[i] != 0) mvprintw(p++, 28, "  %d) %s \t\t", i, items[i]->name.c_str());
	}
	int tmp;
	while ((tmp = getch()) != 27 && tmp != 'q');
}
