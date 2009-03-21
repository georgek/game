/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of user tank class

#include <cmath>
#include <iostream>
#include <string>
#include <tr1/memory>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "point.h"
#include "turret.h"
#include "usertank.h"
#include "world.h"

const double PI = 4.0*std::atan(1.0);

UserTank::UserTank(World* world, const int& layer, const Turret::Ptr& turret, 
		   const std::string& texturename,
		   const int& init_x, const int& init_y, 
		   const int& engine_force, const int& mass, const int& rpm) :
    Tank (world, layer, turret, texturename, init_x, init_y, engine_force, 
	  mass, rpm),
    speed (180)
{
    
}

UserTank::UserTank(World* world, const int& layer, const Turret::Ptr& turret,
	 const int& init_x, const int& init_y,
	 const std::string& inputfile) :
    Tank(world, layer, turret, init_x, init_y, inputfile),
    speed(180) 
{
    
}

UserTank::~UserTank () 
{
    
}

void UserTank::draw () 
{
    // rotate tank
    rotate();
    // move tank
    move();
    // rotate turret
    rotate_turret();

    // set turret position
    turret->setWorldX (worldpos.getX());
    turret->setWorldY (worldpos.getY());

    Tank::draw();
}

bool UserTank::isCollidedR(const float& centre_x,
			   const float& centre_y,
			   const float& radius) const 
{
    return false;
}

bool UserTank::isCollidedV(const std::vector<Point>& vertices) const 
{
    return false;
}

void UserTank::update(SDL_Event& event) 
{
    // respond to WASD keys
    // W and S keys make it accelerate forwards and backwards
    // D steers right and A steers left, to simulate a tank's
    // skid steering, the current velocity is halved while
    // steering
    if (event.type == SDL_KEYDOWN) {
	switch (event.key.keysym.sym) {
	case SDLK_w:
	    velocity += speed;
	    break;
	case SDLK_s:
	    velocity += -speed;
	    break;
	case SDLK_d:
	    curr_rpm += -rpm;
	    velocity = velocity/2;
	    speed = speed/2;
	    break;
	case SDLK_a:
	    curr_rpm += rpm;
	    velocity = velocity/2;
	    speed = speed/2;
	    break;
	default:
	    break;
	}
    }

    if (event.type == SDL_KEYUP) {
	switch (event.key.keysym.sym) {
	case SDLK_w:
	    velocity -= speed;
	    break;
	case SDLK_s:
	    velocity -= -speed;
	    break;
	case SDLK_d:
	    curr_rpm -= -rpm;
	    velocity = velocity*2;
	    speed = speed*2;
	    break;
	case SDLK_a:
	    curr_rpm -= rpm;
	    velocity = velocity*2;
	    speed = speed*2;
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
	    std::cout << "Shoot!" << std::endl;
	    break;
	case SDL_BUTTON_RIGHT:
	    std::cout << "Reload!" << std::endl;
	    break;
	}
    }

    if(event.type == SDL_MOUSEBUTTONUP) {
	switch (event.button.button) {
	case SDL_BUTTON_LEFT:
	    break;
	}
    }
}

void UserTank::move() 
{
    // move according to current velocities
    // velocities are in pixels/second
    
    float ticks = timer.get_ticks();
    float new_world_x = worldpos.getX() + velocity*xf*ticks/1000.0;
    float new_world_y = worldpos.getY() + velocity*yf*ticks/1000.0;

    // no collision, set new world coords
    worldpos.setX(new_world_x);
    worldpos.setY(new_world_y);
    // set world offset
    world->setXOffset(new_world_x);
    world->setYOffset(new_world_y);
}

void UserTank::rotate() 
{
    // rotate tank according to current rpm
    float new_rotation = heading + 
	(curr_rpm*timer.get_ticks())/(60000.0f/360.0f);
    if (new_rotation > 360) new_rotation = new_rotation - 360;
    if (new_rotation < 0) new_rotation = 360 + new_rotation;

    // no collision, set new rotation
    heading = new_rotation;

    if (curr_rpm) {
	// calculate new x and y factors for movement
	xf = std::cos((heading+90) * PI/180.0);
	yf = std::sin((heading+90) * PI/180.0);
    }
}

void UserTank::rotate_turret()
{
    // rotate turret
    // get current resolution
    int vw = SDL_GetVideoInfo()->current_w;
    int vh = SDL_GetVideoInfo()->current_h;
    
    // convert opengl coords to sdl coords
    Point screenpos (worldpos.getX() - world->getXOffset(),
    		     worldpos.getY() - world->getYOffset());
    float sdlxOffset = screenpos.getDispX() + vw/2;
    float sdlyOffset = vh/2 - screenpos.getDispY();
    
    // angle between tank and mouse
    float dx = sdlxOffset - mouse_x;
    float dy = sdlyOffset - mouse_y;
    float theta;

    // y=atan(x) gives only -PI/2 < y < PI/2 so the answer must be
    // adjusted, also dx = 0 is a special case since we cannot divide
    // by zero
    if (dx == 0) {
	theta = dy>0 ? 0 : PI;
    }
    else if (dx>0) {
	theta = -std::atan(dy/dx)+PI/2;
    }
    else { /* dx<0 */
	theta = -std::atan(dy/dx)+3*PI/2;
    }

    // convert to degrees
    theta = theta*180/PI;

    // find out how far the turret can move in one tick
    float tick_angle = (turret_rpm*timer.get_ticks())/(60000.0f/360.0f);

    // difference between current turret direction and mouse direction
    float diff = theta - turret->getTurretAngle();

    // now set turret rpm
    if (std::fabs(diff) < tick_angle) {
	curr_turret_rpm = 0;
	turret->setTurretAngle(theta);
    }
    else if (diff >= -180 && diff < 180) {
	curr_turret_rpm = (diff > 0 ? turret_rpm : -turret_rpm) + curr_rpm;
    }
    else {
	curr_turret_rpm = (diff > 0 ? -turret_rpm : turret_rpm) + curr_rpm;
    }
    
    // adjust turret rotation based on current rpm
    turret->incTurretAngle(
	(curr_turret_rpm*timer.get_ticks())
	/(60000.0f/360.0f)
	);
}
