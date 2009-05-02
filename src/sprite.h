/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for an animated sprite, the sprite can loop infinitely, or
// delete itself after a certain number of loops

#ifndef SPRITE_H
#define SPRITE_H

#include <string>

#include "texture.h"
#include "world.h"

class Sprite : public Renderable
{
public:
    Sprite(World* world, const std::string& texturename,
           const int& frames);
    virtual ~Sprite();

    // renderable function
    virtual void draw();
    
    // run this
    virtual void run(World::RendMap::iterator you);
    
private:
    World* world;
    Texture texture;
    int frames;

    World::RendMap::iterator me;

    bool running;
    
};

#endif // SPRITE_H
