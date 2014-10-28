/*
 * creature.h
 *
 *  Created on: 8.12.2012
 *      Author: iikka
 */

#ifndef CREATURE_H_
#define CREATURE_H_

class AsgardGame;

#include <string>
#include "inventory.h"
#include "item.h"

enum creatureFlags {
	AGGRESSIVE=0x02,
	A_HIT=0x04,
	CANT_MOVE=0x08
};

const char* const STR_AGGRESSIVE = "AGGRESSIVE";
const char* const STR_A_HIT = "A_HIT";
const char* const STR_CANT_MOVE = "CANT_MOVE";

enum AI_STATE {
	ATTACK=0,
	ESCAPE=1
};

class Creature {
public:
	Creature(AsgardGame*);
	virtual ~Creature();

	virtual bool tryMove(int, int);
	virtual void takeDamage_Creature(Creature*);
	virtual void takeDamage(int);
	virtual void die();
	virtual void onTurn();
	virtual void onUpdate();
	virtual void drawCreature();
	virtual int speed();
	virtual std::string name();
	virtual int picture();

	int getHealt();
	virtual int getMaxHealt();

	bool canSee(int, int, int);

	int x;
	int y;
	Inventory inv;
	Inventory armor;


	int flags;
	AI_STATE state;
protected:
	virtual bool canSmell();
	DamageTo calculateDamage(Creature*);

	AsgardGame* game;
	short int healt;
	short int c_speed; // TODO temp
	int c_picture;

private:
	void tryMoveTo(int, int);
};

#endif /* CREATURE_H_ */
