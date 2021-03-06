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

class World;

class Tank : public Renderable, public Collidable
{
public:
    // constructor needs pointer to world, pointer to turret, texture
    // for tank, initial world position, and engine force, rotate
    // speed etc.
    Tank(World* world, const int& layer, const Turret::Ptr& turret, 
	 const std::string& texturename,
	 const Point& init_pos,
	 const int& engine_force, const int& mass, const int& rpm);
    // constructor for making tank for XML input file
    Tank(World* world, const int& layer, const Turret::Ptr& turret,
	 const Point& init_pos,
	 const std::string& inputfile);
    virtual ~Tank();
    
    // rendering functions
    virtual void draw();

    // colliding functions
    virtual bool isCollidedR(const Point& centre,
			     const float& radius) const;
    virtual bool isCollidedV(const std::vector<Point>& vertices) const;

    // returns world position
    virtual Point getWorldPos() const;

    // fire a bullet
    virtual void fire();
    // check if tank is loaded
    virtual bool isLoaded() const;
    // check if tank is alive
    virtual bool isAlive() const;

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
    int accn, dccn, top_speed, rpm;
    // current physical properties
    float curr_velocity, target_velocity, curr_rpm;
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
    // health and reload status
    float curr_health, max_health, loaded;
    // false if tank has been destroyed
    bool alive;
    
    // timer
    Timer timer;

    // make drawing list
    void makeDrawingList();
    // for parsing input file
    void parseInputFile(const std::string& filename);

    // perform transformations
    virtual void move();
    virtual void rotate();
    virtual void rotate_turret();

    // reloads the tank
    virtual void reload();

    // tank dies
    virtual void die();
};

#endif /* TANK_H */
