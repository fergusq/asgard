/*
 * mathlib.h
 *
 *  Created on: 8.12.2012
 *      Author: iikka
 */

#ifndef MATHLIB_H_
#define MATHLIB_H_

class Map;

inline int nvec(int i) {
	if (i < 0) return -1;
	if (i > 0) return 1;
	return 0;
}

namespace SMath {

	bool los(int, int, int, int, int, Map*);

};

#endif /* MATHLIB_H_ */
