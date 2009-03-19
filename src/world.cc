/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Implementation of World class

#include <stdexcept>
#include <string>
#include <tr1/memory>
#include <vector>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

#include "SDL.h"
#include "SDL_opengl.h"

#include "point.h"
#include "tank.h"
#include "turret.h"
#include "usertank.h"
#include "world.h"

World::World (const std::string& inputworldfile) :
    worldfile (inputworldfile.c_str()),
    xOffset (0),
    yOffset (0) 
{
    if (!worldfile.is_open()) {
	std::string s = "No such worldfile: ";
	s += inputworldfile;
	throw std::runtime_error (s);
    }

    // now parse the worldfile and build the world
    // for now just put in some test objects

    // add a turret
    Turret::Ptr turret (new Turret(this, "textures/tankturret4.png", 
				   128, 128, 18));
    // add to renderable map
    renderables.insert(std::make_pair(2, turret));
    // add to collidable map
    collidables.insert(std::make_pair(2, turret));
    
    // add tank
    UserTank::Ptr tank (new UserTank(this, turret, "textures/tankbody4.png",
			     128, 128, 10, 100, 18));
    // add to renderable map
    renderables.insert(std::make_pair(1, tank));
    // add to collidable map
    collidables.insert(std::make_pair(1, tank));
    // add to controllable vector
    controllables.push_back(tank);

    // add another
    // add a turret
    Turret::Ptr turret2 (new Turret(this, "textures/tankturret4.png", 
				   256, 256, 18));
    // add to renderable map
    renderables.insert(std::make_pair(2, turret2));
    // add to collidable map
    collidables.insert(std::make_pair(2, turret2));
    
    // add tank
    Tank::Ptr tank2 (new Tank(this, turret, "textures/tankbody4.png",
			     256, 256, 10, 100, 18));
    // add to renderable map
    renderables.insert(std::make_pair(1, tank2));
    // add to collidable map
    collidables.insert(std::make_pair(1, tank2));
}

World::~World () 
{
    // destruct stuff
}

void World::setXOffset(const float& x) 
{
    xOffset = x;
}

void World::setYOffset(const float& y) 
{
    yOffset = y;
}

float World::getXOffset() const 
{
    return xOffset;
}

float World::getYOffset() const 
{
    return yOffset;
}

void World::draw()
{
    // clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // draw all renderables
    RendMap::iterator pos;
    for (pos = renderables.begin(); pos != renderables.end(); ++pos) {
	pos->second->draw();
    }

    // swap buffers
    SDL_GL_SwapBuffers();
}

bool World::isCollidedR (const float& centre_x,
			 const float& cetnre_y,
			 const float& radius) const 
{
    // do collision detection
    return false;
}

bool World::isCollidedV (const std::vector<Point>& vertices) const 
{
    // do collision detection
    return false;
}

void World::update (SDL_Event& event) 
{
    // update all controllables
    ConVect::iterator pos;
    for (pos = controllables.begin(); pos != controllables.end(); ++pos) {
	(*pos)->update(event);
    }
}