/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for the user controlled tank

#ifndef USERTANK_H
#define USERTANK_H

#include <string>
#include <tr1/memory>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "collidable.h"
#include "controllable.h"
#include "point.h"
#include "renderable.h"
#include "tank.h"
#include "texture.h"
#include "timer.h"
#include "turret.h"
#include "world.h"

class UserTank : public Tank, public Controllable
{
public:
    // constructor needs pointer to world, pointer to turret, texture
    // for tank, initial world position, and engine force, rotate
    // speed etc.
    UserTank(World* world, const int& layer, const Turret::Ptr& turret, 
	 const std::string& texturename,
	 const int& init_x, const int& init_y, 
	 const int& engine_force, const int& mass, const int& rpm);
    virtual ~UserTank();
    
    // rendering functions
    virtual void draw();

    // colliding functions
    virtual bool isCollidedR(const float& centre_x,
			     const float& centre_y,
			     const float& radius) const;
    virtual bool isCollidedV(const std::vector<Point>& vertices) const;

    // control functions
    virtual void update(SDL_Event& event);

    // convenience typedef
    typedef std::tr1::shared_ptr<UserTank> Ptr;

private:
    void move();
    void rotate();
    void rotate_turret();

    // temp
    float speed;
};

#endif /* USERTANK_H */
