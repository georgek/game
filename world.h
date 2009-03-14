/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// World class for drawing everything in the game

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <map>
#include <string>
#include <fstream>

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
    // worldfile is a file describing the world to be created
    World(const std::string& worldfile);
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
    virtual bool isCollidedR(const float& centre_x, 
			     const float& centre_y, 
			     const float& radius) const;

    // for arbitrary shapes
    virtual bool isCollidedV(const std::vector<float>& vertices) const;

    // implement controllable method
    virtual void update(SDL_Event& event);

private:
    // the worldfile that this world is made from
    std::ifstream worldfile;
    
    // current offset of world
    float xOffset, yOffset;

    // containers for references to objects
    // multimap of renderables
    std::multimap<int, Renderable::Ptr> renderables;
    // multimap of collidables
    std::multimap<int, Collidable::Ptr> collidables;
    // vector of controllables
    std::vector<Controllable::Ptr> controllables;
};

#endif /* WORLD_H */
