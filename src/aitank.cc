/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of AI tank class

#include <algorithm>
#include <cmath>
#include <iostream>

#include "SDL.h"
#include "SDL_opengl.h"

#include "aitank.h"
#include "transform.h"
#include "world.h"

const double PI = 4.0*std::atan(1.0);

AiTank::AiTank (World* world, const int& layer,
                const Turret::Ptr& turret,
                const std::string& texturename,
                const Point& init_pos,
                const int& engine_force, const int& mass,
                const int& rpm) :
    Tank (world, layer, turret, texturename, init_pos, engine_force,
          mass, rpm),
    destination(init_pos),
    destination_pad(0)
{
    // construct stuff
}

AiTank::AiTank (World* world, const int& layer, 
                const Turret::Ptr& turret,
                const Point& init_pos,
                const std::string& inputfile) :
    Tank (world, layer, turret, init_pos, inputfile),
    destination (init_pos),
    destination_pad(0)
{
    // construct from input file
}

AiTank::~AiTank () 
{
    // destruct stuff
}

void AiTank::draw()
{
    Tank::draw();
}

void AiTank::update()
{
    // find out how much tank will move after stopping
    float decc_distance = ((top_speed * top_speed)/dccn)/2;
    // find out how far tank can rotate in one tick
    float tick_angle = (rpm*timer.get_ticks())/(60000.0f/360.0f);

    // set current speed etc. so it can get to the destination
    if (worldpos % destination > decc_distance + destination_pad) {
        // move to destination
        // find angle to destination
        float dest_heading = worldpos / destination;

        // decide how much we need to move
        float dtheta = dest_heading - heading;
        if (std::fabs(dtheta) < tick_angle) {
            // pretty much facing the right direction
            target_velocity = top_speed;
            heading = dest_heading;
            curr_rpm = 0;
            // calculate new x and y factors for movement
            xf = std::cos((heading+90) * PI/180.0);
            yf = std::sin((heading+90) * PI/180.0);
        }
        else if (std::fabs(dtheta) < 40) {
            // not far to turn, drive and steer
            target_velocity = top_speed/2;
            if (dtheta >= -180 && dtheta < 180) {
                curr_rpm = dtheta>0 ? rpm : -rpm;
            }
            else {
                curr_rpm = dtheta>0 ? -rpm : rpm;
            }
        }
        else {
            // long way to turn, stop and rotate
            target_velocity = 0;
            if (dtheta >= -180 && dtheta < 180) {
                curr_rpm = dtheta>0 ? rpm : -rpm;
            }
            else {
                curr_rpm = dtheta>0 ? -rpm : rpm;
            }
        }
    }
    else {
        // tank can deccelerate to correct position now
        target_velocity = 0;
        curr_rpm = 0;
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
}

void AiTank::update(SDL_Event& event)
{
    // mouse movement
    if (event.type == SDL_MOUSEMOTION) {
	mouse_x = event.motion.x;
	mouse_y = event.motion.y;
    }

    // user events
    if(event.type == SDL_USEREVENT) {
        switch (event.user.code) {
        case 1:
            int damage = *static_cast<int*>(event.user.data1);
            Point location = *static_cast<Point*>(event.user.data2);
            if (worldpos % location < 300) {
                // do some damage
                health -= damage*(300.0f-(worldpos%location))/300;
                if (health < 0) {
                    health = 0;
                    std::cout << "Dead!" << std::endl;
                }
            }
            break;
        }
    }
}

void AiTank::move()
{
    Tank::move();
}

void AiTank::rotate()
{
    Tank::rotate();
}

void AiTank::rotate_turret()
{
    Tank::rotate_turret();
}
