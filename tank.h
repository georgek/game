/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for the tank body, a tank must have a Turret

#ifndef TANK_H
#define TANK_H

#include <vector>
#include <string>
#include <tr1/memory>

#include "SDL.h"
#include "SDL_opengl.h"

#include "texture.h"
#include "timer.h"
#include "renderable.h"
#include "collidable.h"
#include "point.h"
#include "world.h"
#include "turret.h"

class Tank : public Renderable, public Collidable
{
public:
    // constructor needs pointer to world, pointer to turret, texture
    // for tank, initial world position, and engine force, rotate
    // speed etc.
    Tank(World* world, const Turret::Ptr& turret, 
	 const std::string& texturename,
	 const int& init_x, const int& init_y, 
	 const int& engine_force, const int& mass, const int& rpm);
    virtual ~Tank();
    
    // rendering functions
    virtual void draw();

    // colliding functions
    virtual bool isCollidedR(const float& centre_x,
			     const float& centre_y,
			     const float& radius) const;
    virtual bool isCollidedV(const std::vector<Point>& vertices) const;

    // convenience typedef
    typedef std::tr1::shared_ptr<Tank> Ptr;

protected:
    // pointer to world
    World* world;
    // turret
    Turret::Ptr turret;
    // texture
    Texture texture;
    // world position
    Point worldpos;
    // constant physical properties
    int engine_force, mass, rpm;
    // current physical properties
    float velocity, friction;
    // current heading
    float heading;
    // vertex list
    std::vector<Point> vertices;
    // drawing list
    GLuint drawing_list;
    
    // timer
    Timer timer;
};

#endif /* TANK_H */
