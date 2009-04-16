/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for a friendly tank

#ifndef FRIENDLYTANK_H
#define FRIENDLYTANK_H

#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "collidable.h"
#include "controllable.h"
#include "renderable.h"
#include "point.h"
#include "world.h"
#include "tank.h"
#include "turret.h"

class FriendlyTank : public Tank, public Controllable
{
public:
    // basic constructor
    FriendlyTank(World* world, const int& layer, const Turret::Ptr& turret,
                 const Tank::Ptr& leading_tank,
                 const std::string& texturename,
                 const Point& init_pos,
                 const int& engine_force, const int& mass, const int& rpm);
    // from input file
    FriendlyTank(World* world, const int& layer, const Turret::Ptr& turret,
                 const Tank::Ptr& leading_tank,
                 const Point& init_pos,
                 const std::string& inputfile);
    virtual ~FriendlyTank();

private:
    Tank::Ptr leading_tank;
    
};

#endif // FRIENDLYTANK_H
