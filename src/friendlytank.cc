/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of friendly tank class

#include <string>

#include "friendlytank.h"
#include "world.h"
#include "turret.h"
#include "point.h"
#include "tank.h"

FriendlyTank::FriendlyTank (World* world, const int& layer,
                            const Turret::Ptr& turret,
                            const Tank::Ptr& leading_tank,
                            const std::string& texturename,
                            const Point& init_pos,
                            const int& engine_force, const int& mass,
                            const int& rpm) :
    Tank (world, layer, turret, texturename, init_pos, engine_force,
          mass, rpm),
    leading_tank(leading_tank)
{
    // construct stuff
}

FriendlyTank::FriendlyTank (World* world, const int& layer, 
                            const Turret::Ptr& turret,
                            const Tank::Ptr& leading_tank,
                            const Point& init_pos,
                            const std::string& inputfile) :
    Tank (world, layer, turret, init_pos, inputfile),
    leading_tank(leading_tank)
{
    // construct from input file
}

FriendlyTank::~FriendlyTank () 
{
    // destruct stuff
}

