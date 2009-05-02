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

class World;

class Turret : public Renderable, public Collidable 
{
public:
    Turret(World* world, const std::string& texture,
	   const Point& init_pos, const int& layer);
    Turret(World* world, const Point& init_pos,
	   const std::string& filename, const int& layer);
    virtual ~Turret();

    // rendering functions
    virtual void draw();

    // colliding functions
    virtual bool isCollidedR(const Point& centre,
			     const float& radius) const;
    virtual bool isCollidedV(const std::vector<Point>& vertices) const;

    // sets world coords
    void setWorldPos (const Point& pos);

    // gets turret angle
    float getTurretAngle () const;
    // sets turret angle
    void setTurretAngle (const float& theta);
    // increments turret angle
    void incTurretAngle (const float& theta);
    // checks whether the turret will collide in a new position
    bool isCollided (const Point& new_worldpos, const float& angle_inc) const;

    // convenience typedef
    typedef std::tr1::shared_ptr<Turret> Ptr;

protected:
    // pointer to world
    World* world;
    // world position
    Point worldpos;
    // current angle
    float curr_angle;
    // radius
    float radius;
    // offset relative to tank (for OpenGL)
    Point offset;
    // corrected absolute offset (for collision detection)
    Point real_offset;
    // texture
    Texture texture;
    // vertex list
    std::vector<Point> vertices;
    // drawing list
    GLuint drawing_list;
    // layer
    int layer;

    // timer
    Timer timer;

    void makeDrawingList();
    void parseInputFile(const std::string& filename);
};  

#endif // TURRET_H
