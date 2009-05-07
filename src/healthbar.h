/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// class for a health bar

#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <tr1/memory>

#include "point.h"
#include "renderable.h"

class HealthBar : public Renderable
{
public:
    HealthBar(const Point& init_pos, const int& init_health = 100,
              const int& max_health = 100, const int& init_reload = 100);
    virtual ~HealthBar();
    
    virtual void draw();

    void update(const Point& pos, const int& health, const int& reload);
    
    // convenience typedef
    typedef std::tr1::shared_ptr<HealthBar> Ptr;

private:
    Point screenpos;
    float curr_health;
    float max_health;
    float reload;
};

#endif // HEALTHBAR_H
