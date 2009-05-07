/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for an enemy tank

#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "aitank.h"
#include "point.h"
#include "timer.h"
#include "turret.h"

class World;

class EnemyTank : public AiTank
{
public:
    // basic constructor
    EnemyTank(World* world, const int& layer, const Turret::Ptr& turret,
              const std::string& texturename, const Point& init_pos,
              const int& engine_force, const int& mass, const int& rpm,
              const int& id);
    // from input file
    EnemyTank(World* world, const int& layer, const Turret::Ptr& turret,
              const Point& init_pos, const std::string& inputfile,
              const int& id);
    virtual ~EnemyTank();

    virtual void draw();

    // control functions
    virtual void update();
    virtual void update(SDL_Event& event);

    // convenience typedef
    typedef std::tr1::shared_ptr<EnemyTank> Ptr;

private:
    // id of enemy, used for releasing friendlies
    int id;
    // real user position
    Point user_pos;
    // timer for generating new random positions
    Timer error_timer;
    // random vector for adjusting the aim
    Point aim_error;
    // timer for regulating fire frequency
    Timer fire_timer;
    
    virtual void move();
    virtual void rotate();
    virtual void rotate_turret();

    virtual void fire();
    virtual void die();
};

#endif // ENEMYTANK_H
