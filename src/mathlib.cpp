/*
 * mathlib.cpp
 *
 *  Created on: 12.12.2012
 *      Author: iikka, los author unknown
 */

#include <stdlib.h>
#include <math.h>
#include "mathlib.h"
#include "map.h"

/**
 * Bresenham's Line Algorithm + line of sight; between two points, a and b
 *
 * @param x1	point a's x
 * @param y1	point a's y
 * @param x2	point b's x
 * @param y2	point b's y
 * @param dis	max distance (TODO no effect)
 * @param m		map
 *
 * @returns true if there's line of sight
 *
 * @author unknown, from http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm
 */
bool SMath::los(int x1, int y1, int x2, int y2, int dis, Map* m) { // TODO etäisyydet

	    int delta_x(x2 - x1);
	    // if x1 == x2, then it does not matter what we set here
	    signed char const ix((delta_x > 0) - (delta_x < 0));
	    delta_x = abs(delta_x) << 1;

	    int delta_y(y2 - y1);
	    // if y1 == y2, then it does not matter what we set here
	    signed char const iy((delta_y > 0) - (delta_y < 0));
	    delta_y = abs(delta_y) << 1;

	    //if (!m->isFree(x1, y1)) return false;

	    if (delta_x >= delta_y)
	    {
	        // error may go below zero
	        int error(delta_y - (delta_x >> 1));

	        while (x1 != x2)
	        {
	            if ((error >= 0) && (error || (ix > 0)))
	            {
	                error -= delta_x;
	                y1 += iy;
	            }
	            // else do nothing

	            error += delta_y;
	            x1 += ix;

	            if (x1 == x2 && y1 == y2) return true;
	            if (!m->inRange(x1, y1)) return false;
	            if (m->getNode(x1, y1)->type == WALL) return false;
	        }
	    }
	    else
	    {
	        // error may go below zero
	        int error(delta_x - (delta_y >> 1));

	        while (y1 != y2)
	        {
	            if ((error >= 0) && (error || (iy > 0)))
	            {
	                error -= delta_y;
	                x1 += ix;
	            }
	            // else do nothing

	            error += delta_x;
	            y1 += iy;

	            if (x1 == x2 && y1 == y2) return true;
	            if (!m->inRange(x1, y1)) return false;
	            if (m->getNode(x1, y1)->type == WALL) return false;
	        }
	    }
	    return true;
}
