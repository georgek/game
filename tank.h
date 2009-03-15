/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for the tank body, a tank must have a Turret

#ifndef TANK_H
#define TANK_H

#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "texture.h"
#include "renderable.h"
#include "collidable.h"
#include "point.h"
#include "world.h"

class Tank : public Renderable, public Collidable
{
public:
    // constructor needs pointer to world, texture for tank, initial
    // world position, and engine force, rotate speed etc.
    Tank(const World* world, const std::string& texture, 
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

protected:
    // pointer to world
    World* world;
    // texture
    Texture texture;
    // world position
    int world_x, world_y;
    // constant physical properties
    int engine_force, mass, rpm;
    
    // move according to current velocity
    void move();
    // rotate according to current rpm
    void rotate();
    // rotate turret according to mouse coords and position
    void rotate_turret();

private:
    bool user;
    // display lists
    GLuint body_list;
    GLuint turret_list;

    // mouse coords
    int mouse_x, mouse_y;
    // turret rotation
    float turret_rotation;

    // vertices
    float vert_x[4];
    float vert_y[4];
};

#endif /* TANK_H */
