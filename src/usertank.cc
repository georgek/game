/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of user tank class

#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <tr1/memory>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "bullet.h"
#include "friendlytank.h"
#include "healthbar.h"
#include "point.h"
#include "transform.h"
#include "turret.h"
#include "usertank.h"
#include "world.h"

const double PI = 4.0*std::atan(1.0);

UserTank::UserTank(World* world, const int& layer, const Turret::Ptr& turret, 
		   const std::string& texturename,
		   const Point& init_pos,
		   const int& engine_force, const int& mass, const int& rpm) :
    Tank (world, layer, turret, texturename, init_pos, engine_force, 
	  mass, rpm)
{
    // set world offset
    world->setOffset(init_pos);
}

UserTank::UserTank(World* world, const int& layer, const Turret::Ptr& turret,
		   const Point& init_pos,
		   const std::string& inputfile) :
    Tank(world, layer, turret, init_pos, inputfile),
    health_bar (new HealthBar (Point(0,0), 400, 400))
{
    // set world offset
    world->setOffset(init_pos);

    // user tank has more health
    curr_health = max_health = 400;

    // add healthbar to world
    world->addRenderable (health_bar, 10);
}

UserTank::~UserTank () 
{
    
}

void UserTank::draw () 
{
    Tank::draw();
}

bool UserTank::isCollidedR(const Point& centre,
			   const float& radius) const 
{
    return Tank::isCollidedR(centre, radius);
}

bool UserTank::isCollidedV(const std::vector<Point>& vertices) const 
{
    return Tank::isCollidedV(vertices);
}

void UserTank::update ()
{
    // update healthbar position
    health_bar->update(worldpos - world->getOffset(), curr_health, loaded);

    if (!alive) {
        return;
    }
    
    // rotate tank
    rotate();
    // move tank
    move();
    // rotate turret
    rotate_turret();
    // reload
    reload();

    // set turret position
    turret->setWorldPos (worldpos);

    // push an event with the new position
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = 2;
    Point* posi = new Point (worldpos);
    event.user.data1 = posi;
    SDL_PushEvent(&event);

    // check to see if we have reached the objective
    if (worldpos % objective_pos < objective_radius) {
        // we have
        world->setGameStatus(1);
        return;
    }

    // check to see if any friendlies have become active
    ConvoyList::iterator pos = friendlies.begin();
    while (pos != friendlies.end()) {
        if ((*pos)->isActive()) {
            // this one has become active
            // follow tank at end of convoy, or this user tank
            if (!convoy.empty()) {
                (*pos)->follow (convoy.back().get());
            }
            else {
                (*pos)->follow (this);
            }
            // move it to convoy list
            convoy.splice(convoy.end(), friendlies, pos++);
        }
        else {
            ++pos;
        }
    }
    // check to see if any friendlies in the convoy have died
    pos = convoy.begin();
    bool prev_removed = false;
    while (pos != convoy.end()) {
        if (!(*pos)->isAlive()) {
            // it has died, remove from convoy
            convoy.erase(pos++);
            prev_removed = true;
        }
        else if (prev_removed) {
            // previous tank(s) have gone, follow another one
            if (pos == convoy.begin()) {
                // this is now first in convoy, follow user
                (*pos)->follow(this);
            }
            else {
                // follow previous in convoy
                ConvoyList::iterator pos2 = pos;
                --pos2;
                (*pos)->follow((pos2)->get());
            }
            prev_removed = false;
            ++pos;
        }
        else {
            ++pos;
        }
    }
}

void UserTank::update (SDL_Event& event) 
{
    if (!alive) {
        return;
    }
    
    // respond to WASD keys
    // W and S keys make it accelerate forwards and backwards
    // D steers right and A steers left, to simulate a tank's
    // skid steering, the current velocity is halved while
    // steering
    if (event.type == SDL_KEYDOWN) {
	switch (event.key.keysym.sym) {
	case SDLK_w:
	    target_velocity += top_speed;
	    break;
	case SDLK_s:
	    target_velocity += -top_speed;
	    break;
	case SDLK_d:
	    curr_rpm += -rpm;
	    curr_velocity = curr_velocity/2;
	    target_velocity = target_velocity/2;
	    top_speed = top_speed/2;
	    break;
	case SDLK_a:
	    curr_rpm += rpm;
	    curr_velocity = curr_velocity/2;
	    target_velocity = target_velocity/2;
	    top_speed = top_speed/2;
	    break;
	default:
	    break;
	}
    }

    if (event.type == SDL_KEYUP) {
	switch (event.key.keysym.sym) {
	case SDLK_w:
	    target_velocity -= top_speed;
	    break;
	case SDLK_s:
	    target_velocity -= -top_speed;
	    break;
	case SDLK_d:
	    curr_rpm -= -rpm;
	    curr_velocity = curr_velocity*2;
	    target_velocity = target_velocity*2;
	    top_speed = top_speed*2;
	    break;
	case SDLK_a:
	    curr_rpm -= rpm;
	    curr_velocity = curr_velocity*2;
	    target_velocity = target_velocity*2;
	    top_speed = top_speed*2;
	    break;
        case SDLK_n:
            // add new friendly tank
            addFriend();
            break;
	default:
	    break;
	}
    }

    // mouse movement
    if (event.type == SDL_MOUSEMOTION) {
	mouse_x = event.motion.x;
	mouse_y = event.motion.y;
    }

    // mouse buttons
    if(event.type == SDL_MOUSEBUTTONDOWN) {
	switch (event.button.button) {
	case SDL_BUTTON_LEFT:
            fire();
	    break;
	case SDL_BUTTON_RIGHT:
            multiFire();
	    break;
	}
    }

    // user events
    if(event.type == SDL_USEREVENT) {
        switch (event.user.code) {
        case 1: 
        {
            int damage = *static_cast<int*>(event.user.data1);
            Point location = *static_cast<Point*>(event.user.data2);
            if (worldpos % location < 300) {
                // do some damage
                curr_health -= damage*(300.0f-(worldpos%location))/300;
                if (curr_health < 0) {
                    curr_health = 0;
                    die();
                }
            }
            break;
        }
        case 3:                 // enemy dead, get some health
        {
            curr_health += 50;
            if (curr_health > max_health) {
                curr_health = max_health;
            }
            break;
        }
        }
    }
}

void UserTank::addFriendly(const FriendlyTank::Ptr& friendly)
{
    friendlies.push_back(friendly);
}

void UserTank::fire()
{
    if (this->isLoaded()) {
        Tank::fire();
        return;
    }
    // make one tank in convoy fire
    for (ConvoyList::iterator pos = convoy.begin();
         pos != convoy.end(); ++pos) {
        if ((*pos)->isLoaded()) {
            (*pos)->fire();
            return;
        }
    }    
}

void UserTank::multiFire()
{
    Tank::fire();
    // make all tanks in convoy fire
    for (ConvoyList::iterator pos = convoy.begin();
         pos != convoy.end(); ++pos) {
        (*pos)->fire();
    }
}

void UserTank::setObjective (const Point& pos, const int& radius)
{
    objective_pos = pos;
    objective_radius = radius;
}

void UserTank::move() 
{
    Tank::move();

    // set world offset
    world->setOffset(worldpos);
}

void UserTank::rotate() 
{
    Tank::rotate();
}

void UserTank::rotate_turret()
{
    Tank::rotate_turret();
}

void UserTank::die()
{
    Tank::die();
    // end of game
    world->setGameStatus(2);
}

void UserTank::addFriend()
{
    Turret::Ptr turret (new Turret(world, Point(512,512), 
                                   "turret2.xml", 2));
    world->addRenderable(turret, 2);
    world->addCollidable(turret, 2);
    std::list<int> captors;
    FriendlyTank::Ptr tank (new FriendlyTank(world, 1, turret,
                                             Point(512,512), 
                                             "tank2.xml",
                                             captors));
    world->addRenderable(tank, 1);
    world->addCollidable(tank, 1);
    world->addControllable(tank);

    // follow tank at end of convoy, or this user tank
    if (!convoy.empty()) {
        tank->follow (convoy.back().get());
    }
    else {
        tank->follow (this);
    }

    // add to convoy
    convoy.push_back(tank);
}
