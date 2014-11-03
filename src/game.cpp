/*
 * game.cpp
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
#include "player.h"
#include "map.h"
#include "creature.h"
#include "CustomCreature.h"
#include "inventory.h"
#include "item.h"
#include "ConfigLoader.h"
#include "asgård.h"

AsgardGame::AsgardGame() {
	player = new Player(this);
	map = new Map(this);

	messageStr = new std::string;
	shortCreatureInfo = 0;

	cmodels = ConfigLoader::parseCreatures("config/creatures.txt");
	imodels = ConfigLoader::parseItems("config/items.txt");

	if (!loadGame()) { // TODO parempi lataaminen, aloitusasetelmat tiedostosta
		player = new Player(this);
		player->inv.addItem(new Item(imodels[1]));
		player->inv.addItem(new Item(imodels[3]));
		player->inv.addItem(new Item(imodels[4]));
		map->addCreature(2, 2, player);

		level = 1;

		generateMap();
	}

	// TODO debug
	level = 1; // TODO tasot
	//player->inv.addItem(new Item(imodels[0]));
	//player->inv.addItem(new Item(imodels[1]));
	//player->inv.addItem(new Item(imodels[4]));

	//map->addItem(5, 5, new Item(imodels[4]));
	//map->addItem(5, 4, new Item(imodels[3]));
	//map->addCreature(30, 15, new CustomCreature(this, cmodels[0]));
	//map->addCreature(30, 16, new CustomCreature(this, cmodels[1]));
	/*map->addCreature(30, 16, new Creature(this));
	map->addCreature(30, 17, new Creature(this));
	map->addCreature(30, 18, new Creature(this));
	map->addCreature(30, 19, new CustomCreature(this, std::string("rogue"), 'r'|COLOR_PAIR(9)|A_BOLD, 10, 5, 1));
	map->addCreature(30, 20, new CustomCreature(this, std::string("cat"), 'f'|COLOR_PAIR(10), 5, 2, 3));*/
	/*map->addWall(5, 6);
	map->addWall(5, 7);
	map->addWall(5, 8);
	map->addWall(5, 9);
	map->addWall(5, 10);
	map->addWall(6, 10);
	map->addWall(7, 10);
	map->addWall(8, 10);
	map->addWall(9, 10);
	map->addWall(9, 9);
	map->addWall(9, 8);
	map->addWall(9, 7);
	map->addWall(9, 6);
	map->addWall(8, 6);
	map->addWall(6, 6);*/
}

AsgardGame::~AsgardGame() {
	delete player; player = 0;
	delete map; map = 0;
	for (unsigned int i = 0; i < cmodels.size(); i++) {
		delete cmodels[i];
	}
	for (unsigned int i = 0; i < imodels.size(); i++) {
		delete imodels[i];
	}
}

bool AsgardGame::loadGame() {
	std::ifstream * in = new std::ifstream("save.asgard");

	if (!in->is_open()) return false;

	map->loadMap(in);
	delete in;

	return true;
}

void AsgardGame::saveGame() {
	std::ofstream * out = new std::ofstream("save.asgard");
	map->saveMap(out);
	delete out;
}

void AsgardGame::generateMap() {
	srand(time(0));

	map->fillWall(0, 0, MAP_SIZE_X, MAP_SIZE_Y);

	int lx = 2;
	int ly = 2;

	for (int y = 0; y < MAP_SIZE_Y; y++) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			if(rand()%200==0){
				int sizex = x+(rand()%10);
				int sizey = y+(rand()%10);
				map->clearWall(x, y, sizex, sizey);
				map->lineRWall(lx, ly, x, y);
				lx = x;
				ly = y;

				/*if (rand()%4==0) {
					for (unsigned int i = 0; i < cmodels.size(); i++) {
						if (cmodels[i]->level <= level) {
							//map->addCreature(x+rand()%(sizex-x), y+rand()%(sizey-y), new CustomCreature(this, cmodels[i]));
						}
					}
				} else*/ {
					for (unsigned int i = 0; i < imodels.size(); i++) {
						if (imodels[i]->level <= level && rand()%2 == 0) {
							map->addItem(sizex+1, sizey+1, new Item(imodels[i]));
						}
					}
				}

			}
		}
	}
	for (int y = 0; y < MAP_SIZE_Y; y++) {
		for (int x = 0; x < MAP_SIZE_X; x++) {
			if(rand()%80==0){

				if (rand()%2==0) {
					for (unsigned int i = 0; i < cmodels.size(); i++) {
						if (cmodels[i]->level <= level && rand()%cmodels.size() == 0) {
							map->addCreature(x, y, new CustomCreature(this, cmodels[i]));
						}
					}
				} else for (unsigned int i = 0; i < imodels.size(); i++) {
					if (imodels[i]->level <= level && rand()%cmodels.size() == 0) {
						map->addItem(x, y, new Item(imodels[i]));
					}
				}


			}
		}
	}
}

int AsgardGame::getLevel() {
	return level;
}

void AsgardGame::updateGame() {
	for (unsigned int i = 0; i < cmodels.size(); i++) {
		if (cmodels[i]->level <= level && rand()%5 == 0) {
			map->addCreature(rand()%MAP_SIZE_X, rand()%MAP_SIZE_Y, new CustomCreature(this, cmodels[i]));
		}
	}

	map->onUpdate();
}

void AsgardGame::drawGame() {
	//clear();

	move(0, 0);
	clrtoeol();

	map->drawMap(player);

	mvaddstr(0, 27, messageStr->c_str());
	drawEnemyInfo();

	delete messageStr;
	messageStr = new std::string;

	drawPlayerInfo();
}

void AsgardGame::drawPlayerInfo() {
	int line = 3;

	mvaddstr(line++, 3, "The Player");

	for (int i = 0; i < 10; i++) {
		move(line, 3+i);
		if (player->armor.getItem(i) != 0) {
			player->armor.getItem(i)->drawItem();
		}
	}
	line ++;

	mvprintw(line, 3, "                    ");
	if (player->getHealth() < player->getMaxHealth()) attron(COLOR_PAIR(8)); // korostetaan teksti jos pelaaja on menettänyt elämiä
	mvprintw(line++, 3, "HP: %d/%d", player->getHealth(), player->getMaxHealth());
	if (player->getHealth() < player->getMaxHealth())attroff(COLOR_PAIR(8));

	mvprintw(line, 3, "                    ");
	mvprintw(line++, 3, "LEVEL: %d", level);

}

void AsgardGame::drawEnemyInfo() {
	mvaddstr(24, 3, "                      ");

	if (shortCreatureInfo == 0) return;

	std::string shortEnemyInfoStr;

	int healtper = ((double(shortCreatureInfo->getHealth()) / double(shortCreatureInfo->getMaxHealth())) * 100);
	healtper /= 5;


	for (int i = 0; i < healtper; i++) {
		shortEnemyInfoStr.append("*");
	}

	if (healtper == 0 && shortCreatureInfo->getHealth() != 0) shortEnemyInfoStr += "*";

	mvaddch(24, 3, '[');

	attron(A_BOLD);
	if (healtper < 3) attron(COLOR_PAIR(CP_RED));
	else if (healtper < 5) attron(COLOR_PAIR(CP_YELLOW));
	else if (healtper < 10) attron(COLOR_PAIR(CP_BLUE));

	mvaddstr(24, 4, shortEnemyInfoStr.c_str());

	attroff(A_BOLD);
	if (healtper < 3) attroff(COLOR_PAIR(CP_RED));
	else if (healtper < 5) attroff(COLOR_PAIR(CP_YELLOW));
	else if (healtper < 10) attroff(COLOR_PAIR(CP_BLUE));

	mvaddch(24, 24, ']');

	shortCreatureInfo = 0;
}

void AsgardGame::message(std::string msg) {
	messageStr->append(msg);
	messageStr->append(" ");

	messageList.push_back(msg);

}

void AsgardGame::showMessageLog() {
	mvprintw(0, 28, ("  Message log\t\t"));
	int p = 1;
	for (unsigned int i = 0; i < messageList.size(); i++) {
		mvprintw(p++, 28, "  %d) %s \t\t", i, messageList[i].c_str());
	}
	int tmp;
	while ((tmp = getch()) != 27 && tmp != 'q');
}

bool AsgardGame::target(bool (*callback)(AsgardGame*,int, int)) {
	int x = player->x;
	int y = player->y;
	int chr;
	do {
		map->drawMap(player);
		map->drawOn(x, y, 'X');
		mvprintw(0, 27, "%50s", "");
		if (player->canSee(x, y, player->viewDistance()))
			mvprintw(0, 27, "%s", map->getNode(x, y)->getDescription().c_str());
		chr = getch();
		switch (chr) {
		case KEY_DOWN:
		case '2':
			if (y<MAP_SIZE_Y-1) y++;
			break;
		case KEY_UP:
		case '8':
			if (y>0) y--;
			break;
		case KEY_LEFT:
		case '4':
			if (x>0) x--;
			break;
		case KEY_RIGHT:
		case '6':
			if (x<MAP_SIZE_X-1) x++;
			break;
		case '1':
			if (y<MAP_SIZE_Y-1) y++;
			if (x>0) x--;
			break;
		case '3':
			if (y<MAP_SIZE_Y-1) y++;
			if (x<MAP_SIZE_X-1) x++;
			break;
		case '7':
			if (y>0) y--;
			if (x>0) x--;
			break;
		case '9':
			if (y>0) y--;
			if (x<MAP_SIZE_X-1) x++;
			break;
		case 27:
		case 'q':
			return false;
		}
	} while(chr != 10);
	if (callback != 0) return callback(this, x, y);
	else return false;
}

void AsgardGame::drawShortCreatureInfo(Creature* c) {
	shortCreatureInfo = c;

}
