/*
 * game.h
 *
 *  Created on: 8.12.2012
 *      Author: iikka
 */

#ifndef GAME_H_
#define GAME_H_

class Creature;
class Player;
class Map;

struct CreatureModel;
struct ItemModel;

#include <string>
#include <vector>

class AsgardGame {
public:
	AsgardGame();
	~AsgardGame();
	void updateGame();
	void drawGame();
	void message(std::string);
	void drawShortCreatureInfo(Creature*);
	void showMessageLog();

	int getLevel();

	bool loadGame();
	void saveGame();

	void generateMap();

	Player * player;

	Map * map;


	int level;
private:
	void drawPlayerInfo();
	void drawEnemyInfo();


	std::string * messageStr;
	Creature * shortCreatureInfo;

	std::vector<std::string> messageList;

	std::vector<CreatureModel *> cmodels;
	std::vector<ItemModel *> imodels;
};
#endif /* GAME_H_ */
