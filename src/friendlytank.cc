/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of friendly tank class

#include <algorithm>
#include <list>
#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "friendlytank.h"
#include "healthbar.h"
#include "world.h"
#include "turret.h"
#include "point.h"
#include "tank.h"

FriendlyTank::FriendlyTank (World* world, const int& layer,
                            const Turret::Ptr& turret,
                            const std::string& texturename,
                            const Point& init_pos,
                            const int& engine_force, const int& mass,
                            const int& rpm, std::list<int>& captors) :
    AiTank (world, layer, turret, texturename, init_pos, engine_force,
          mass, rpm),
    leading_tank(0),
    is_following(true)
{
    // construct stuff
    destination_pad = 180;

    // move items from captor list into this class's list
    this->captors.splice(this->captors.end(), captors);
}

FriendlyTank::FriendlyTank (World* world, const int& layer, 
                            const Turret::Ptr& turret,
                            const Point& init_pos,
                            const std::string& inputfile,
                            std::list<int>& captors) :
    AiTank (world, layer, turret, init_pos, inputfile),
    leading_tank(0),
    is_following(true),
    health_bar(new HealthBar(Point(0,0)))
{
    // construct from input file
    destination_pad = 180;

    // add healthbar to world
    world->addRenderable (health_bar, 10);

    active = false;

    // move items from captor list into this class's list
    this->captors.splice(this->captors.end(), captors);
}

FriendlyTank::~FriendlyTank () 
{
    // destruct stuff
}

void FriendlyTank::draw()
{
    Tank::draw();
}

void FriendlyTank::update()
{
    // update healthbar position
    health_bar->update(worldpos - world->getOffset(), curr_health, loaded);

    if (!alive) {
        return;
    }
    
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
    // user events
    if(event.type == SDL_USEREVENT) {
        switch (event.user.code) {
        case 3:                 // enemy dead
            int enemy_id = *static_cast<int*>(event.user.data1);
            std::cout << "Enemy dead: " << enemy_id << std::endl;
            // check if this enemy is in the captor list
            std::list<int>::iterator pos =
                std::find(captors.begin(), captors.end(), enemy_id);
            if (pos != captors.end()) {
                // it is in the list, so remove it
                captors.erase(pos);
            }
            if (captors.empty()) {
                // all captors dead, so I am free
                active = true;
                std::cout << "I am free!" << std::endl;
            }
            break;
        }
    }

    if (!alive) {
        return;
    }

    if (!active) {
        return;
    }
    
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
