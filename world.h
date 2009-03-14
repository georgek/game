/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// World class for drawing everything in the game

#ifndef WORLD_H
#define WORLD_H

#include "SDL.h"

// implements renderable
#include "renderable.h"
// implements collidable
#include "collidable.h"
// implements controllable
#include "controllable.h"

class World : public Renderable, public Collidable, public Controllable
{
public:
    World();
    virtual ~World();

    // set world offset (e.g. to be done by main character)
    void	setXOffset(const float& x);
    void	setYOffset(const float& y);
    // get world offset (e.g. for stuff that moves with the world)
    float	getXOffset() const;
    float	getYOffset() const;

    // implement renderable method
    virtual void draw();

    // implement collidable methods
    // for simple circles
    virtual bool isCollidedR(float centre_x, float centre_y, float radius);
    // for arbitrary shapes
    virtual bool isCollidedV(int n_vertices, float* vert_x, float* vert_y);

    // implement controllable method
    virtual void update(SDL_Event& event);

private:
    // current offset of world
    float xOffset, yOffset;
};

#endif /* WORLD_H */
