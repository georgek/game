/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Interface for Controllable objects. Objects must implement this
// interface if they move around the map, either automatically or with
// user input via SDL events

#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

#include <tr1/memory>

#include "SDL.h"

class Controllable 
{
public:
    virtual void update() = 0;
    virtual void update(SDL_Event& event) = 0;

    // convenience typdef
    typedef std::tr1::shared_ptr<Controllable> Ptr;
};

#endif // CONTROLLABLE_H
