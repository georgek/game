/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Interface for World Objects. Objects must implement this interface
// if they are part of the World, ie. they move across the screen with
// the ground

#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "renderable.h"

class WorldObject 
{
public:
    virtual void giveWorld(Renderable* world) = 0;
};

#endif // WORLDOBJECT_H
