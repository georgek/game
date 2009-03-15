/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Interface for Controllable objects. Objects must implement this
// interface if they are to be controlled by the user with SDL events

#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

#include "SDL.h"

#include <tr1/memory>

class Controllable 
{
public:
    virtual void update(SDL_Event& event) = 0;

    // convenience typdef
    typedef std::tr1::shared_ptr<Controllable> Ptr;
};

#endif // CONTROLLABLE_H
