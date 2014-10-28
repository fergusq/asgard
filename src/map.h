/*
 * map.h
 *
 *  Created on: 8.12.2012
 *      Author: iikka
 */

#ifndef MAP_H_
#define MAP_H_

class AsgardGame;
class Creature;
class Player;
class Item;

#include <vector>
#include <fstream>

enum nodeType {
	ITEM='x', CREATURE='X', ITEM_AND_CREATURE='w', WALL='#', FREE='.', UNKNOWN=' '
};

struct mapNode {
	int size;
	nodeType type;
	Creature* creature;
	Item * item;
};

struct itemNode {
	int x;
	int y;
	Item * item;
};

const int MAP_SIZE_X = 256;
const int MAP_SIZE_Y = 128;

class Map {
private:
	void createMap();
	void deleteMap();
	mapNode *(map[MAP_SIZE_X][MAP_SIZE_Y]);
	std::vector<Creature*> creatures;
	AsgardGame * game;
	int scrollX;
	int scrollY;

	int screenX;
	int screenY;
public:
	Map(AsgardGame*);
	~Map();
	bool isFree(int, int);
	void addWall(int, int);
	void removeWall(int, int);
	void fillWall(int, int, int, int);
	void clearWall(int, int, int, int);
	void lineWall(int, int, int, int);
	void lineRWall(int, int, int, int);
	void addCreature(int, int, Creature*);
	void removeCreature(Creature*);
	void addItem(int, int, Item*);
	mapNode *getNode(int, int);
	bool inRange(int, int);
	void onUpdate();
	void drawMap(Player*);
	void saveMap(std::ofstream*);
	void loadMap(std::ifstream*);
};
#endif /* MAP_H_ */
