/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for the tank body, a tank must have a Turret

#ifndef TANK_H
#define TANK_H

#include <string>
#include <tr1/memory>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "collidable.h"
#include "controllable.h"
#include "point.h"
#include "renderable.h"
#include "texture.h"
#include "timer.h"
#include "turret.h"
#include "world.h"

class Tank : public Renderable, public Collidable
{
public:
    // constructor needs pointer to world, pointer to turret, texture
    // for tank, initial world position, and engine force, rotate
    // speed etc.
    Tank(World* world, const int& layer, const Turret::Ptr& turret, 
	 const std::string& texturename,
	 const int& init_x, const int& init_y, 
	 const int& engine_force, const int& mass, const int& rpm);
    // constructor for making tank for XML input file
    Tank(World* world, const int& layer, const Turret::Ptr& turret,
	 const int& init_x, const int& init_y,
	 const std::string& inputfile);
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
    // radius
    float radius;
    // constant physical properties
    int engine_force, mass, rpm;
    // current physical properties
    float velocity, friction, curr_rpm;
    // current heading
    float heading;
    // velocity components
    float xf, yf;
    // mouse coordinate
    int mouse_x, mouse_y;
    // turret rpm
    float turret_rpm, curr_turret_rpm;
    // vertex list
    std::vector<Point> vertices;
    // drawing list
    GLuint drawing_list;
    // layer in world
    int layer;
    
    // timer
    Timer timer;

    // make drawing list
    void makeDrawingList();
    // for parsing input file
    void parseInputFile(const std::string& filename);
};

#endif /* TANK_H */
