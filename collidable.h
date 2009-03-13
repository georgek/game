/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Interface for Collidable objects. Objects must implement this
// interface if they are tangible ie. they can collide with other
// objects and the world

#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "tools/shared_ptr.h"

class Collidable 
{
public:
    virtual bool isCollidedR(float centre_x, 
			     float centre_y, float radius) = 0;
    virtual bool isCollidedV(int n_vertices, 
			     float* x_vert, float* y_vert) = 0;

    // convenience typdef
    typedef gk::shared_ptr<Collidable> Ptr;
};

#endif // COLLIDABLE_H
