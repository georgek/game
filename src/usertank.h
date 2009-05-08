/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for the user controlled tank

#ifndef USERTANK_H
#define USERTANK_H

#include <list>
#include <string>
#include <tr1/memory>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "collidable.h"
#include "controllable.h"
#include "friendlytank.h"
#include "healthbar.h"
#include "point.h"
#include "renderable.h"
#include "tank.h"
#include "texture.h"
#include "timer.h"
#include "turret.h"

class World;

class UserTank : public Tank, public Controllable
{
public:
    // constructor needs pointer to world, pointer to turret, texture
    // for tank, initial world position, and engine force, rotate
    // speed etc.
    UserTank(World* world, const int& layer, const Turret::Ptr& turret, 
	     const std::string& texturename,
	     const Point& init_pos,
	     const int& engine_force, const int& mass, const int& rpm);
    // from input file
    UserTank(World* world, const int& layer, const Turret::Ptr& turret,
	     const Point& init_pos,
	     const std::string& inputfile);
    virtual ~UserTank();
    
    // rendering functions
    virtual void draw();

    // colliding functions
    virtual bool isCollidedR(const Point& centre,
			     const float& radius) const;
    virtual bool isCollidedV(const std::vector<Point>& vertices) const;

    // control functions
    virtual void update();
    virtual void update(SDL_Event& event);

    // add a friendly tank to the friendlies list
    void addFriendly(const FriendlyTank::Ptr& friendly);

    virtual void fire();
    void multiFire();

    // sets the objective
    void setObjective(const Point& pos, const int& radius);

    // convenience typedef
    typedef std::tr1::shared_ptr<UserTank> Ptr;

private:
    typedef std::list<FriendlyTank::Ptr> ConvoyList;
    // the current following tanks
    ConvoyList convoy;
    // friendlies in the field, that aren't in the convoy
    ConvoyList friendlies;

    virtual void move();
    virtual void rotate();
    virtual void rotate_turret();

    virtual void die();

    // health bar
    HealthBar::Ptr health_bar;

    // the location of the obective
    Point objective_pos;
    int objective_radius;

    // function to add a friend tank to the map, for testing only
    void addFriend();
};

#endif /* USERTANK_H */
