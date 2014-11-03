/*
 * player.h
 *
 *  Created on: 8.12.2012
 *      Author: iikka
 */

#ifndef PLAYER_H_
#define PLAYER_H_

class AsgardGame;

#include "creature.h"
#include "map.h"

class Player : public Creature {
public:
	Player(AsgardGame* g) : Creature(g) {c_speed = 2; health = getMaxHealth();}
	virtual ~Player() {}

	virtual bool tryMove(int, int);
	virtual void takeDamage_Creature(Creature*);
	virtual void takeDamage(DamageTo);
	virtual void takeDamage(DamageTo, std::string, std::string);
	virtual void die();
	virtual void onTurn();
	virtual void drawCreature();
	virtual std::string name();
	virtual int getMaxHealth();

	int viewDistance();

	nodeType memory_map[MAP_SIZE_X][MAP_SIZE_Y];

protected:
	virtual bool canSmell();
};

#endif /* PLAYER_H_ */
