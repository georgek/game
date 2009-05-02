/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of friendly tank class

#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "friendlytank.h"
#include "world.h"
#include "turret.h"
#include "point.h"
#include "tank.h"

FriendlyTank::FriendlyTank (World* world, const int& layer,
                            const Turret::Ptr& turret,
                            const std::string& texturename,
                            const Point& init_pos,
                            const int& engine_force, const int& mass,
                            const int& rpm) :
    AiTank (world, layer, turret, texturename, init_pos, engine_force,
          mass, rpm),
    leading_tank(0),
    is_following(true)
{
    // construct stuff
    destination_pad = 180;
}

FriendlyTank::FriendlyTank (World* world, const int& layer, 
                            const Turret::Ptr& turret,
                            const Point& init_pos,
                            const std::string& inputfile) :
    AiTank (world, layer, turret, init_pos, inputfile),
    leading_tank(0),
    is_following(true)
{
    // construct from input file
    destination_pad = 180;
}

FriendlyTank::~FriendlyTank () 
{
    // destruct stuff
}

void FriendlyTank::update()
{
    if (!leading_tank) {
        // no tank to follow
        AiTank::update();
        return;
    }
    
    if (is_following) {
        // find out where the leading tank is
        Point leadingtankpos = leading_tank->getWorldPos();
        destination = leadingtankpos;
    }

    AiTank::update();
}

void FriendlyTank::update(SDL_Event& event)
{
    // respond to F key to make it start/stop following
    if (event.type == SDL_KEYUP) {
	switch (event.key.keysym.sym) {
        case SDLK_f:
            // toggle following
            is_following = !is_following;
            break;
	default:
	    break;
	}
    }
    AiTank::update(event);
}

void FriendlyTank::follow (Tank* tank)
{
    leading_tank = tank;
}

void FriendlyTank::move()
{
    Tank::move();
}

void FriendlyTank::rotate()
{
    Tank::rotate();
}

void FriendlyTank::rotate_turret()
{
    Tank::rotate_turret();
}
