/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Turret class is for the turret of a tank, it should be part of a
// Tank object

#ifndef TURRET_H
#define TURRET_H

#include <string>
#include <tr1/memory>
#include <vector>

#include "collidable.h"
#include "point.h"
#include "renderable.h"
#include "texture.h"
#include "timer.h"
#include "world.h"

class Turret : public Renderable, public Collidable 
{
public:
    Turret(World* world, const std::string& texture,
	   const int& init_x, const int& init_y,
	   const int& rpm);
    virtual ~Turret();

    // rendering functions
    virtual void draw();

    // colliding functions
    virtual bool isCollidedR(const float& centre_x,
			     const float& centre_y,
			     const float& radius) const;
    virtual bool isCollidedV(const std::vector<Point>& vertices) const;

    // sets world coords
    void setWorldX (const float& x);
    void setWorldY (const float& y);

    // convenience typedef
    typedef std::tr1::shared_ptr<Turret> Ptr;

protected:
    // pointer to world
    World* world;
    // world position
    Point worldpos;
    // current angle
    float curr_angle;
    // texture
    Texture texture;
    // vertex list
    std::vector<Point> vertices;
    // drawing list
    GLuint drawing_list;

    // timer
    Timer timer;
};  

#endif // TURRET_H
