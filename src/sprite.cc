/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implements sprite class

#include <string>

#include "sprite.h"
#include "texture.h"
#include "world.h"

Sprite::Sprite(World* world, const std::string& texturename,
               const int& frames) :
    world (world),
    texture (texturename, Texture::automatic),
    frames (frames),
    running (false)
{
    // construct
}

Sprite::~Sprite()
{
    // destruct
}

void Sprite::draw()
{
    // draw stuff
}

void Sprite::run(World::RendMap::iterator you)
{
    // start running
}

