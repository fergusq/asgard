/*
 * ConfigLoader.cpp
 *
 *  Created on: 10.12.2012
 *      Author: iikka
 */

#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <vector>
#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include "ConfigLoader.h"
#include "creature.h"
#include "asgård.h"
#include "item.h"

std::vector<CreatureModel*> ConfigLoader::parseCreatures(std::string file) {
	std::vector<CreatureModel*> models;
	CreatureModel * m = 0;

	std::ifstream in(file.c_str());

	std::string line;

	while (std::getline(in, line)) {
		if (line.size() > 1 && line[0] != '#') {
			if (line.find(':') != std::string::npos) {
				int at = line.find(':')+2;

				std::string op = line.substr(0, at);
				std::string arg = line.substr(at, line.size()-at);

				if (op == "NAME: ") {
					if (m != 0) models.push_back(m);
					m = new CreatureModel;
					m->name = arg;
					m->flags = 0;
				} else if (op == "CHAR: ") {
					m->picture = arg.c_str()[0];
				} else if (op == "COLOR: ") {
					if (arg=="BLUE") {
						m->picture |= COLOR_PAIR(CP_BLUE);
					} else if (arg=="RED") {
						m->picture |= COLOR_PAIR(CP_RED);
					} else if (arg=="YELLOW") {
						m->picture |= COLOR_PAIR(CP_YELLOW);
					} else if (arg=="GREEN") {
						m->picture |= COLOR_PAIR(CP_GREEN);
					} else if (arg=="MAGENTA") {
						m->picture |= COLOR_PAIR(CP_MAGENTA);
					} else if (arg=="CYAN") {
						m->picture |= COLOR_PAIR(CP_CYAN);
					} else if (arg=="BOLD") {
						m->picture |= A_BOLD;
					}
				} else if (op == "HP: ") {
					m->maxhealth = atoi(arg.c_str());
				} else if (op == "SPEED: ") {
					m->speed = atoi(arg.c_str());
				} else if (op == "ATK: ") {
					m->attack = atoi(arg.c_str());
				} else if (op == "DEF: ") {
					m->armor = atoi(arg.c_str());
				} else if (op == "DROP: ") {
					m->drop = atoi(arg.c_str());
				} else if (op == "LEVEL: ") {
					m->level = atoi(arg.c_str());
				} else if (op == "FLAG: ") {
					if (arg == STR_AGGRESSIVE) {
						m->flags |= AGGRESSIVE;
					}
					if (arg == STR_A_HIT) {
						m->flags |= A_HIT;
					}
					if (arg == STR_CANT_MOVE) {
						m->flags |= CANT_MOVE;
					}
				}
			}
		}
	}
	if (m != 0) models.push_back(m);

	return models;
}

std::vector<ItemModel*> ConfigLoader::parseItems(std::string file) {
	std::vector<ItemModel*> models;
	ItemModel * m = 0;

	std::ifstream in(file.c_str());

	std::string line;

	while (std::getline(in, line)) {
		if (line.size() > 1 && line[0] != '#') {
			if (line.find(':') != std::string::npos) {
				int at = line.find(':') + 2;

				std::string op = line.substr(0, at);
				std::string arg = line.substr(at, line.size() - at);
				if (op == "NAME: ") {
					if (m != 0)
						models.push_back(m);
					m = new ItemModel;
					m->name = arg;
					m->flags = 0;
				} else if (op == "CHAR: ") {
					m->picture = arg.c_str()[0];
				} else if (op == "COLOR: ") {
					if (arg=="BLUE") {
						m->picture |= COLOR_PAIR(CP_BLUE);
					} else if (arg=="RED") {
						m->picture |= COLOR_PAIR(CP_RED);
					} else if (arg=="YELLOW") {
						m->picture |= COLOR_PAIR(CP_YELLOW);
					} else if (arg=="GREEN") {
						m->picture |= COLOR_PAIR(CP_GREEN);
					} else if (arg=="MAGENTA") {
						m->picture |= COLOR_PAIR(CP_MAGENTA);
					} else if (arg=="CYAN") {
						m->picture |= COLOR_PAIR(CP_CYAN);
					}
				} else if (op == "POS: ") {
					if (arg == "HAND1" || arg == "HAND") {
						m->pos = HAND1;
					}
					if (arg == "HAND2") {
						m->pos = HAND2;
					}
					if (arg == "HEAD") {
						m->pos = HEAD;
					}
					if (arg == "BODY") {
						m->pos = BODY;
					}
					if (arg == "FOOT") {
						m->pos = FOOT;
					}
					if (arg == "RING1") {
						m->pos = RING1;
					}
					if (arg == "RING2") {
						m->pos = RING2;
					}
					if (arg == "NECK") {
						m->pos = NECK;
					}
					if (arg == "NONE") {
						m->pos = INVENTORY;
					}
				} else if (op == "ATK: ") {
					m->attack = atoi(arg.c_str());
				} else if (op == "ROLL: ") {
					m->dice = atoi(arg.c_str());
				} else if (op == "ARMOR: ") {
					m->armor = atoi(arg.c_str());
				} else if (op == "LIGHT: ") {
					m->light = atoi(arg.c_str());
				} else if (op == "LEVEL: ") {
					m->level = atoi(arg.c_str());
				} else if (op == "FLAG: ") {
					if (arg == STR_POTION) {
						m->flags |= POTION;
					}
				}
			}
		}
	}
	if (m != 0) models.push_back(m);

	return models;
}
