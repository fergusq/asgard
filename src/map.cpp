/*
 * map.cpp
 *
 *  Created on: 8.12.2012
 *      Author: iikka
 */

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include <vector>
#include "map.h"
#include "creature.h"
#include "player.h"
#include "CustomCreature.h"
#include "game.h"
#include "item.h"

Map::Map(AsgardGame* g) {
	this->game = g;
	createMap();
	scrollX = 0;
	scrollY = 0;

	int maxX, maxY;
	getmaxyx(stdscr,maxY,maxX);

	screenX = maxX-28;
	screenY = maxY-2;
}

Map::~Map() {
	deleteMap();
}

bool Map::isFree(int x, int y) {
	return x >= 0 && y >= 0 && x < MAP_SIZE_X && y < MAP_SIZE_Y && map[x][y]->type == FREE;
}

void Map::addWall(int x, int y) {
	if (isFree(x, y)) map[x][y]->type = WALL;
}

void Map::removeWall(int x, int y) {
	if (inRange(x, y)) {
		if (getNode(x, y)->type == WALL) map[x][y]->type = FREE;
	}
}

void Map::fillWall(int x1, int y1, int x2, int y2) {
	if (x1 > x2) {
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y1 > y2) {
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			addWall(x, y);
		}
	}
}

void Map::clearWall(int x1, int y1, int x2, int y2) {
	if (x1 > x2) {
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y1 > y2) {
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			removeWall(x, y);
		}
	}
}

void Map::lineWall(int x1, int y1, int x2, int y2) {
	if (x1 > x2) {
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y1 > y2) {
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	for (int x = x1; x < x2; x++) {
		addWall(x, y1);
	}
	for (int y = y1; y < y2; y++) {
		addWall(x2, y);
	}
}

void Map::lineRWall(int x1, int y1, int x2, int y2) {
	if (x1 > x2) {
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y1 > y2) {
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	for (int x = x1; x < x2; x++) {
		removeWall(x, y1);
	}
	for (int y = y1; y < y2; y++) {
		removeWall(x2, y);
	}
}

void Map::addCreature(int x, int y, Creature* c) {
	if (isFree(x, y)) {
		c->x = x;
		c->y = y;
		map[x][y]->type = CREATURE;
		map[x][y]->creature = c;
		creatures.push_back(c);
	}
}

void Map::removeCreature(Creature* c) {
	if (getNode(c->x, c->y) != 0) {
		getNode(c->x, c->y)->type = FREE;
		getNode(c->x, c->y)->creature = 0;

		for (unsigned int i = 0; i < creatures.size(); i++) {
			if (creatures[i] == c) {
				creatures.erase(creatures.begin() + i);
			}
		}
	}
}

void Map::addItem(int x, int y, Item * item) {
	if (isFree(x, y)) {
		map[x][y]->type = ITEM;
		map[x][y]->item = item;
	} else if (inRange(x, y)) {
		if (getNode(x, y)->type == CREATURE) {
			map[x][y]->type = ITEM_AND_CREATURE;
			map[x][y]->item = item;
		}
	}
}

mapNode* Map::getNode(int x, int y) {
	return x >= 0 && y >= 0 && x < MAP_SIZE_X && y < MAP_SIZE_Y ? map[x][y] : 0;
}

bool Map::inRange(int x, int y) {
	return x >= 0 && y >= 0 && x < MAP_SIZE_X && y < MAP_SIZE_Y;
}

void Map::createMap() {
	for (int x = 0; x < MAP_SIZE_X; x++) {
		for (int y = 0; y < MAP_SIZE_Y; y++) {
			map[x][y] = new mapNode;
			map[x][y]->type = FREE;
		}
	}
}

void Map::deleteMap() {
	for (int x = 0; x < MAP_SIZE_X; x++) {
		for (int y = 0; y < MAP_SIZE_Y; y++) {
			delete map[x][y]; map[x][y] = 0;
		}
	}
}

void Map::onUpdate() {
	int highSpeed = 1;
	bool creaturesLeft = true;
	while (creaturesLeft) {
		creaturesLeft = false;
		for (unsigned int i = 0; i < creatures.size(); i++) {
			if (creatures[i]->speed() >= highSpeed) {
				creatures[i]->onTurn();
				creaturesLeft = true;
			}
		}
		highSpeed++;
	}

	for (unsigned int i = 0; i < creatures.size(); i++) {
		creatures[i]->onUpdate();

	}
}

void Map::drawMap(Player* p) {

	int spx = p->x - (scrollX+screenX/2);
	int spy = p->y - (scrollY+screenY/2);

	if (spx < -(screenX/2-screenX/5) || spx > (screenX/2-screenX/5) || spy < -(screenY/2-screenY/10) || spy > (screenY/2-screenY/10)) {
		scrollX = p->x-screenX/2;
		scrollY = p->y-screenY/2;
	}

	for (int x = 0; x < screenX; x++) {
		for (int y = 0; y < screenY; y++) {

			int mx = scrollX + x;
			int my = scrollY + y;

			mvaddch(y+1, x+27, ' ');

			if (!inRange(mx, my)){
				//mvaddch(y+1, x+27, '~');
				continue;
			}

			if (!p->canSee(mx, my, p->viewDistance())) {
				attron(A_DIM);
				if (p->memory_map[mx][my] == WALL) {
					mvaddch(y+1, x+27, ' '|A_REVERSE);
				}
				if (p->memory_map[mx][my] == FREE) {
					mvaddch(y+1, x+27, '.');
				}
				attroff(A_DIM);
				continue; // TODO
			}
			p->memory_map[mx][my] = map[mx][my]->type!=WALL ? FREE : WALL;
			if (map[mx][my]->type == WALL) {
				mvaddch(y+1, x+27, ' '|A_REVERSE);
			}
			if (map[mx][my]->type == FREE) {
				mvaddch(y+1, x+27, '.'|A_BOLD);
			}
			if (map[mx][my]->type == CREATURE || map[mx][my]->type == ITEM_AND_CREATURE) {
				move(y+1, x+27);
				map[mx][my]->creature->drawCreature();
			}
			if (map[mx][my]->type == ITEM) {
				move(y+1, x+27);
				map[mx][my]->item->drawItem();
			}
		}
	}
}

void Map::saveMap(std::ofstream *out) {
	std::vector<itemNode> items;

	*out << game->getLevel() << '\n';
	for (int y = 0; y < MAP_SIZE_Y; y++) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			*out << char(map[x][y]->type);

			if (map[x][y]->type==ITEM) {
				items.push_back({x, y, map[x][y]->item});
			}
		}
		*out << '\n';
	}
	*out << '\n';
	*out << items.size() << '\n';
	for (unsigned int i = 0; i < items.size(); i++) {
		*out << items[i].x << '\n';
		*out << items[i].y << '\n';
		*out << items[i].item->name << '\n';
		*out << items[i].item->picture << '\n';
		*out << items[i].item->armorPoints << '\n';
		*out << items[i].item->attackPoints << '\n';
		*out << items[i].item->rollTimes << '\n';
		*out << items[i].item->light << '\n';
		*out << items[i].item->type << '\n';
		*out << items[i].item->flags << '\n';
		*out << '\n';
	}

	for (int y = 0; y < MAP_SIZE_Y; y++) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			*out << char(game->player->memory_map[x][y] == WALL || game->player->memory_map[x][y]==FREE ? game->player->memory_map[x][y] : UNKNOWN);
		}
		*out << '\n';
	}

	*out << creatures.size() << '\n';

	*out << game->player->name() << '\n';
	*out << game->player->picture() << '\n';
	*out << game->player->x << '\n';
	*out << game->player->y << '\n';
	*out << game->player->getHealt() << '\n';
	*out << game->player->getMaxHealt() << '\n';
	*out << game->player->speed() << '\n';
	*out << game->player->flags << '\n';
	*out << game->player->state << '\n';
	for (int i = 0; i < 10; i++) {
		if (game->player->inv.getItem(i) == 0){
			*out << '0' << '\n';
			*out << '\n';
			continue;
		}
		*out << '1' << '\n';
		*out << game->player->inv.getItem(i)->name << '\n';
		*out << game->player->inv.getItem(i)->picture << '\n';
		*out << game->player->inv.getItem(i)->armorPoints << '\n';
		*out << game->player->inv.getItem(i)->attackPoints << '\n';
		*out << game->player->inv.getItem(i)->rollTimes << '\n';
		*out << game->player->inv.getItem(i)->light << '\n';
		*out << game->player->inv.getItem(i)->type << '\n';
		*out << game->player->inv.getItem(i)->flags << '\n';
		*out << '\n';
	}
	for (int i = 0; i < 10; i++) {
		if (game->player->armor.getItem(i) == 0){
			*out << '0' << '\n';
			*out << '\n';
			continue;
		}
		*out << '1' << '\n';
		*out << game->player->armor.getItem(i)->name << '\n';
		*out << game->player->armor.getItem(i)->picture << '\n';
		*out << game->player->armor.getItem(i)->armorPoints << '\n';
		*out << game->player->armor.getItem(i)->attackPoints << '\n';
		*out << game->player->armor.getItem(i)->rollTimes << '\n';
		*out << game->player->armor.getItem(i)->light << '\n';
		*out << game->player->armor.getItem(i)->type << '\n';
		*out << game->player->armor.getItem(i)->flags << '\n';
		*out << '\n';
	}
	*out << '\n';

	for (unsigned int i = 0; i < creatures.size(); i++) {
		if (creatures[i] == game->player) continue;
		*out << creatures[i]->name() << '\n';
		*out << creatures[i]->picture() << '\n';
		*out << creatures[i]->x << '\n';
		*out << creatures[i]->y << '\n';
		*out << creatures[i]->getHealt() << '\n';
		*out << creatures[i]->getMaxHealt() << '\n';
		*out << creatures[i]->speed() << '\n';
		*out << creatures[i]->flags << '\n';
		*out << creatures[i]->state << '\n';
		for (int j = 0; j < 10; j++) {
			if (creatures[i]->inv.getItem(j) == 0){
				*out << '0' << '\n';
				*out << '\n';
				continue;
			}
			*out << '1' << '\n';
			*out << creatures[i]->inv.getItem(j)->name << '\n';
			*out << creatures[i]->inv.getItem(j)->picture << '\n';
			*out << creatures[i]->inv.getItem(j)->armorPoints << '\n';
			*out << creatures[i]->inv.getItem(j)->attackPoints << '\n';
			*out << creatures[i]->inv.getItem(j)->rollTimes << '\n';
			*out << creatures[i]->inv.getItem(j)->light << '\n';
			*out << creatures[i]->inv.getItem(j)->type << '\n';
			*out << creatures[i]->inv.getItem(j)->flags << '\n';
			*out << '\n';
		}
		for (int j = 0; j < 10; j++) {
			if (creatures[i]->armor.getItem(j) == 0){
				*out << '0' << '\n';
				*out << '\n';
				continue;
			}
			*out << '1' << '\n';
			*out << creatures[i]->armor.getItem(j)->name << '\n';
			*out << creatures[i]->armor.getItem(j)->picture << '\n';
			*out << creatures[i]->armor.getItem(j)->armorPoints << '\n';
			*out << creatures[i]->armor.getItem(j)->attackPoints << '\n';
			*out << creatures[i]->armor.getItem(j)->rollTimes << '\n';
			*out << creatures[i]->armor.getItem(j)->light << '\n';
			*out << creatures[i]->armor.getItem(j)->type << '\n';
			*out << creatures[i]->armor.getItem(j)->flags << '\n';
			*out << '\n';
		}
		*out << '\n';
	}
}

void Map::loadMap(std::ifstream *in) {
	*in >> (game->level);
	in->get();
	for (int y = 0; y < MAP_SIZE_Y; y++) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			map[x][y]->type = in->get() == WALL ? WALL : FREE;
		}
		in->get();
	}

	in->get();

	unsigned int items;
	*in >> items; in->get();

	for (unsigned int i = 0; i < items; i++) {
		int x, y;
		*in >> x; in -> get();
		*in >> y; in->get();

		std::string n;
		getline(*in, n);
		int p=0;
		*in >> p; in->get();
		int a2=0;
		*in >> a2; in->get();
		int a=0;
		*in >> a; in->get();
		int d=0;
		*in >> d; in->get();
		int l=0;
		*in >> l; in->get();
		int t=0;
		*in >> t; in->get();
		int f=0;
		*in >> f; in->get();

		Item * item = new Item(n, p, ItemPosition(t), a, d, a2, l, f);

		addItem(x, y, item);

		in->get();
	}

	for (int y = 0; y < MAP_SIZE_Y; y++) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			game->player->memory_map[x][y] = nodeType(in->get());
		}
		in->get();
	}

	int size;
	*in >> size; in->get();

	for (int i = 0; i < size; i++) {
		std::string name;
		getline(*in, name);

		int pic = -1;
		*in >> pic;in->get();

		int x = -1;
		*in >> x;in->get();

		int y = -1;
		*in >> y;in->get();

		int h = -1;
		*in >> h;in->get();

		int mh = -1;
		*in >> mh;in->get();

		int s = -1;
		*in >> s;in->get();

		int f = -1;
		*in >> f;in->get();

		int st = -1;
		*in >> st;in->get();

		Creature * creature;

		if (i != 0) {

			creature = new CustomCreature(game, name, pic, mh, s, 1);
			creature->flags = f;
			creature->state = AI_STATE(st);
			creature->takeDamage(mh-h);
		} else { // pelaaja
			creature = game->player;
			creature->flags = f;
			creature->state = AI_STATE(st);
			creature->takeDamage(mh-h);
		}

		for (int i = 0; i < 10; i++) {
			int is;
			*in >> is;in->get();

			if (is) {
				std::string n;
				getline(*in, n);
				int p=0;
				*in >> p; in->get();
				int a2=0;
				*in >> a2; in->get();
				int a=0;
				*in >> a; in->get();
				int d=0;
				*in >> d; in->get();
				int l=0;
				*in >> l; in->get();
				int t=0;
				*in >> t; in->get();
				int f=0;
				*in >> f; in->get();

				Item * item = new Item(n, p, ItemPosition(t), a, d, a2, l, f);

				creature->inv.setItem(i, item);
			}

			in->get();
		}
		for (int i = 0; i < 10; i++) {
			int is;
			*in >> is;in->get();

			if (is) {
				std::string n;
				getline(*in, n);
				int p=0;
				*in >> p; in->get();
				int a2=0;
				*in >> a2; in->get();
				int a=0;
				*in >> a; in->get();
				int d=0;
				*in >> d; in->get();
				int l=0;
				*in >> l; in->get();
				int t=0;
				*in >> t; in->get();
				int f=0;
				*in >> f; in->get();

				Item * item = new Item(n, p, ItemPosition(t), a, d, a2, l, f);

				creature->armor.setItem(i, item);
			}

			in->get();
		}

		addCreature(x, y, creature);



		in->get();
	}
}
