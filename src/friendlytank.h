/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for a friendly tank

#ifndef FRIENDLYTANK_H
#define FRIENDLYTANK_H

#include <list>
#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "aitank.h"
#include "collidable.h"
#include "controllable.h"
#include "healthbar.h"
#include "renderable.h"
#include "point.h"
#include "tank.h"
#include "turret.h"

class World;

class FriendlyTank : public AiTank
{
public:
    // basic constructor
    FriendlyTank(World* world, const int& layer, const Turret::Ptr& turret,
                 const std::string& texturename, const Point& init_pos,
                 const int& engine_force, const int& mass, const int& rpm,
                 std::list<int>& captors);
    // from input file
    FriendlyTank(World* world, const int& layer, const Turret::Ptr& turret,
                 const Point& init_pos, const std::string& inputfile,
                 std::list<int>& captors);
    virtual ~FriendlyTank();

    virtual void draw();

    // control functions
    virtual void update();
    virtual void update(SDL_Event& event);

    // make it follow a tank
    void follow(Tank* tank);

    // convenience typedef
    typedef std::tr1::shared_ptr<FriendlyTank> Ptr;

private:
    Tank* leading_tank;

    // sets whether tank is following or not
    bool is_following;

    HealthBar::Ptr health_bar;

    // list of captors, these are ids of enemies
    std::list<int> captors;
    
    virtual void move();
    virtual void rotate();
    virtual void rotate_turret();
};

#endif // FRIENDLYTANK_H
