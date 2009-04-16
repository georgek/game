/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of user tank class

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <tr1/memory>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

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
    Tank(world, layer, turret, init_pos, inputfile)
{
    // set world offset
    world->setOffset(init_pos);
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
    // rotate tank
    rotate();
    // move tank
    move();
    // rotate turret
    rotate_turret();

    // set turret position
    turret->setWorldPos (worldpos);
}

void UserTank::update (SDL_Event& event) 
{
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
    float ticks = timer.get_ticks();
    // apply acceleration
    if (curr_velocity < target_velocity && target_velocity > 0) {
	// accelerate forwards
	curr_velocity += accn*ticks/1000.0;
	if (curr_velocity > target_velocity) curr_velocity = target_velocity;
    }
    else if (curr_velocity > target_velocity && target_velocity < 0) {
	// accelerate backwards
	curr_velocity -= accn*ticks/1000.0;
	if (curr_velocity < target_velocity) curr_velocity = target_velocity;
    }
    else if (curr_velocity > target_velocity && target_velocity == 0) {
	// deccelerate
	curr_velocity -= dccn*ticks/1000.0;
	if (curr_velocity < target_velocity) curr_velocity = target_velocity;
    }
    else if (curr_velocity < target_velocity && target_velocity == 0) {
	// deccelerate
	curr_velocity += dccn*ticks/1000.0;
	if (curr_velocity > target_velocity) curr_velocity = target_velocity;
    }

    // move according to current velocities
    Point new_worldpos;
    new_worldpos.setX(worldpos.getX() + curr_velocity*xf*ticks/1000.0);
    new_worldpos.setY(worldpos.getY() + curr_velocity*yf*ticks/1000.0);

    if (new_worldpos == worldpos) {
	// nothing do so return
	return;
    }

    // check for collisions, first check turret
    if (turret->isCollided(new_worldpos, 0)) {
	return;
    }

    // work out new vertices
    std::vector<Point> new_vertices;
    // transform the vertices using the transform functor
    std::transform(vertices.begin(), vertices.end(), // source
                   std::back_inserter(new_vertices), // destination
                   Transform(new_worldpos, heading));

    // now check collisions with tank
    if (world->isCollided(new_worldpos, radius, new_vertices, layer, this)) {
        // it has collided
        curr_velocity = 0;
        return;
    }

    // no collision, set new world coords
    worldpos = new_worldpos;
    // set world offset
    world->setOffset(worldpos);
}

void UserTank::rotate() 
{
    // rotate tank according to current rpm
    float new_heading = heading + 
	(curr_rpm*timer.get_ticks())/(60000.0f/360.0f);
    if (new_heading > 360) new_heading = new_heading - 360;
    if (new_heading < 0) new_heading = 360 + new_heading;

    if (new_heading == heading) {
	// nothing to do
	return;
    }

    // work out new vertices
    std::vector<Point> new_vertices;
    // transform the vertices using the transform functor
    std::transform(vertices.begin(), vertices.end(), // source
                   std::back_inserter(new_vertices), // destination
                   Transform(worldpos, new_heading));

    // check for collisions
    if (world->isCollided(worldpos, radius, new_vertices, layer, this)) {
        // it has collided
        return;
    }

    // no collision, set new rotation
    heading = new_heading;

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
    Point screenpos = worldpos - world->getOffset();
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

    if (theta == turret->getTurretAngle()) {
	// nothing to do
	return;
    }

    // find out how far the turret can move in one tick
    float tick_angle = (turret_rpm*timer.get_ticks())/(60000.0f/360.0f);

    // difference between current turret direction and mouse direction
    float diff = theta - turret->getTurretAngle();

    // now set turret rpm
    if (std::fabs(diff) < tick_angle) {
	curr_turret_rpm = 0;
	// check for collisions
	if (!turret->isCollided(worldpos, theta-turret->getTurretAngle())) {
	    // not collided, set angle exactly to mouse
	    turret->setTurretAngle(theta);
	}
	// return because there is nothing else to do
	return;
    }
    else if (diff >= -180 && diff < 180) {
	curr_turret_rpm = (diff > 0 ? turret_rpm : -turret_rpm) + curr_rpm;
    }
    else {
	curr_turret_rpm = (diff > 0 ? -turret_rpm : turret_rpm) + curr_rpm;
    }
    
    // find change in angle based on current rpm
    float dtheta = (curr_turret_rpm*timer.get_ticks())/(60000.0f/360.0f);

    // check for collisions
    if (turret->isCollided(worldpos, dtheta)) {
	// collided
	return;
    }
    // not collided, set new turret angle
    turret->incTurretAngle(dtheta);
}
