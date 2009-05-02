/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for a bullet

#ifndef BULLET_H
#define BULLET_H

#include <tr1/memory>

#include "SDL.h"
#include "SDL_opengl.h"

#include "point.h"
#include "texture.h"
#include "timer.h"
#include "world.h"

class Bullet : public Renderable
{
public:
    Bullet(World* world, const std::string& texturename,
           const Point& init_pos, const Point& destination,
           const float& start_radius,
           const int& speed, const int& damage);
    virtual ~Bullet();
    
    // rendering functions
    virtual void draw();

    // makes it run
    void run(World::RendMap::iterator thisBullet);

    // convenience typedef
    typedef std::tr1::shared_ptr<Bullet> Ptr;
    
private:
    World* world;
    World::RendMap::iterator this_bullet;
    GLuint drawing_list;
    
    Point worldpos;
    Point destination;
    float start_radius;

    Texture texture;

    int damage;
    int speed;
    
    float heading;
    float xf, yf;
    bool running;

    Timer timer;
};

#endif // BULLET_H
