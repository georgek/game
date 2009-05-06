/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for an ornament

#ifndef ORNAMENT_H
#define ORNAMENT_H

#include <string>
#include <tr1/memory>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "point.h"
#include "world.h"

class Ornament : public Renderable, public Collidable
{
public:
    Ornament(World* world, const Point& pos, const std::string& filename,
             float rotation);
    virtual ~Ornament();
    
    virtual void draw();
    
    virtual bool isCollidedR(const Point& centre,
                             const float& radius) const;
    virtual bool isCollidedV(const std::vector<Point>& vertices) const;
    
    // convenience typedef
    typedef std::tr1::shared_ptr<Ornament> Ptr;
    
private:
    World* world;
    Point worldpos;
    float heading;
    Texture texture;
    GLuint drawing_list;
    std::vector<Point> vertices;
    float radius;
};

#endif // ORNAMENT_H

