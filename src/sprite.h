/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for an animated sprite, the sprite can loop infinitely, or
// delete itself after a certain number of loops

#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <tr1/memory>

#include "point.h"
#include "texture.h"
#include "timer.h"
#include "world.h"

class Sprite : public Renderable
{
public:
    Sprite(World* world, const std::string& texturename,
           const Point& worldpos, const int& frames, const int& fps = 30,
           const int& loops = 0, const float& rotation = 0);
    virtual ~Sprite();

    // renderable function
    virtual void draw();
    
    // run this
    virtual void run(World::RendMap::iterator you);

    // convenience typedef
    typedef std::tr1::shared_ptr<Sprite> Ptr;
    
private:
    World* world;
    Texture texture;
    Point worldpos;
    int frames;
    int fps;
    int loops;
    float rotation;
    Timer timer;

    World::RendMap::iterator me;

    bool running;
    int curr_frame;
    int loops_done;
};

#endif // SPRITE_H
