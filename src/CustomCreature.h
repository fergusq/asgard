/*
 * CustomCreature.h
 *
 *  Created on: 9.12.2012
 *      Author: iikka
 */

#ifndef CUSTOMCREATURE_H_
#define CUSTOMCREATURE_H_

struct CreatureModel;

#include "creature.h"

class CustomCreature: public Creature {
public:
	CustomCreature(AsgardGame*, std::string, int, int, int, int);
	CustomCreature(AsgardGame*, CreatureModel * m);
	virtual ~CustomCreature();

	virtual std::string name();
	virtual void drawCreature();
	virtual int getMaxHealt();
private:
	std::string c_name;
	int c_maxhealt;
};

#endif /* CUSTOMCREATURE_H_ */
