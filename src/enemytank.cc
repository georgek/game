/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of enemy tank

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "aitank.h"
#include "bullet.h"
#include "enemytank.h"
#include "sprite.h"
#include "world.h"

const double PI = 4.0*std::atan(1.0);

EnemyTank::EnemyTank(World* world, const int& layer, const Turret::Ptr& turret,
                     const std::string& texturename,
                     const Point& init_pos,
                     const int& engine_force, const int& mass, const int& rpm) :
    AiTank (world, layer, turret, texturename, init_pos, engine_force,
            mass, rpm)
{
    // construct stuff
}

EnemyTank::EnemyTank(World* world, const int& layer, const Turret::Ptr& turret,
                     const Point& init_pos,
                     const std::string& inputfile) :
    AiTank (world, layer, turret, init_pos, inputfile)
{
    destination_pad = 600;

    // start error timer
    error_timer.start();
    aim_error = Point::polarD (std::rand()%200, std::rand()%360);

    // start fire timer
    fire_timer.start();
}

EnemyTank::~EnemyTank()
{
    // destruct
}

void EnemyTank::draw()
{
    AiTank::draw();
}

void EnemyTank::update()
{
    if (!alive) {
        return;
    }
    
    if (!active) {
        if (worldpos % user_pos < 700) {
            // become active
            active = true;
            fire_timer.start();
        }
        else {
            return;
        }
    }
    
    destination = user_pos;

    // make a new random aim error if enough time has passed
    if (error_timer.get_ticks() > 1000) {
        aim_error = Point::polarD (std::rand()%200, std::rand()%360);
        error_timer.start();
    }

    if (fire_timer.get_ticks() > 2000) {
        fire();
        fire_timer.start();
    }

    AiTank::update();
}

void EnemyTank::update(SDL_Event& event)
{
    if (!alive) {
        return;
    }
    
    // get new user position
    if(event.type == SDL_USEREVENT) {
        switch (event.user.code) {
        case 2:
            user_pos = *static_cast<Point*>(event.user.data1);
            break;
        }
    }

    // user events
    if(event.type == SDL_USEREVENT) {
        switch (event.user.code) {
        case 1:
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
    }
}

void EnemyTank::move()
{
    AiTank::move();
}

void EnemyTank::rotate()
{
    AiTank::rotate();
}

void EnemyTank::rotate_turret()
{
    // rotate turret
    // get current resolution
    int vw = SDL_GetVideoInfo()->current_w;
    int vh = SDL_GetVideoInfo()->current_h;
    
    // convert opengl coords to sdl coords
    Point screenpos = worldpos - world->getOffset();
    
    // angle between tank and mouse
    float dx = (worldpos.getDispX()+vw/2) - (user_pos.getDispX()+vw/2);
    float dy = (vh/2-worldpos.getDispY()) - (vh/2-user_pos.getDispY());
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

void EnemyTank::fire()
{
    if (loaded < 100) {
        // not loaded, do nothing
        return;
    }
    
    // adjust for real turret position
    float radius = worldpos % user_pos;
    if (radius < 120) {
        // too this is inside the tank, do not fire
        return;
    }

    Point dest = worldpos +
        Point::polarD (radius, turret->getTurretAngle());

    // add the random aim error
    dest += aim_error;

    Point end_of_barrel = worldpos +
        Point::polarD (115, turret->getTurretAngle());
    
    // fire a bullet
    Bullet::Ptr bullet (new Bullet (world, "textures/bullet.png",
                                    end_of_barrel, dest, 0, 1500, 100));
    World::RendMap::iterator bullpos = world->addRenderable (bullet, 3);
    bullet->run(bullpos);

    // put a sprite near it for barrel animation
    Sprite::Ptr sprite (new Sprite (world, "textures/gunfire.png",
                                    end_of_barrel, 14, 30, 1,
                                    turret->getTurretAngle()));
    World::RendMap::iterator spritepos = world->addRenderable (sprite, 3);
    sprite->run(spritepos);

    // reload
    loaded = 0;
}

