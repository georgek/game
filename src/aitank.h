/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// This is for an AI tank.  An object of this class will just try to
// drive to a certain place on the map and stay there until that place
// changes

#ifndef AITANK_H
#define AITANK_H

#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "collidable.h"
#include "controllable.h"
#include "renderable.h"
#include "point.h"
#include "tank.h"
#include "turret.h"

class World;

class AiTank : public Tank, public Controllable
{
public:
    // basic constructor
    AiTank(World* world, const int& layer, const Turret::Ptr& turret,
                 const std::string& texturename,
                 const Point& init_pos,
                 const int& engine_force, const int& mass, const int& rpm);
    // from input file
    AiTank(World* world, const int& layer, const Turret::Ptr& turret,
                 const Point& init_pos,
                 const std::string& inputfile);
    virtual ~AiTank();

    // overload draw function
    virtual void draw();

    // implement controllable functions
    virtual void update();
    virtual void update(SDL_Event& event);

    // convenience typedef
    typedef std::tr1::shared_ptr<AiTank> Ptr;

protected:
    virtual void move();
    virtual void rotate();
    virtual void rotate_turret();

    // location that the tank wants to be at
    Point destination;
    // distance from destination to aim for
    float destination_pad;
};

#endif // AITANK_H
