/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Implementation of World class

#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "world.h"

World::World (const std::string& worldfile) :
    worldfile (worldfile.c_str()),
    xOffset (0),
    yOffset (0) 
{
    // construct stuffx
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
    // draw stuff
}

bool World::isCollidedR (const float& centre_x,
			 const float& cetnre_y,
			 const float& radius) const 
{
    // do collision detection
    return false;
}

bool World::isCollidedV (const std::vector<float>& vertices) const 
{
    // do collision detection
    return false;
}

void World::update (SDL_Event& event) 
{
    // update world
}
