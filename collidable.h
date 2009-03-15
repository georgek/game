/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Interface for Collidable objects. Objects must implement this
// interface if they are tangible ie. they can collide with other
// objects and the world

#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include <vector>
#include <tr1/memory>

#include "point.h"

class Collidable 
{
public:
    virtual bool isCollidedR(const float& centre_x, 
			     const float& centre_y, 
			     const float& radius) const = 0;

    virtual bool isCollidedV(const std::vector<Point>& vertices) const = 0;

    // convenience typdef
    typedef std::tr1::shared_ptr<Collidable> Ptr;
};

#endif // COLLIDABLE_H
