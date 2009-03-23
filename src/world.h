/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// World class for drawing everything in the game

#ifndef WORLD_H
#define WORLD_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

#include "SDL.h"
#include "SDL_opengl.h"

// implements renderable
#include "renderable.h"
// implements controllable
#include "controllable.h"

#include "collidable.h"
#include "point.h"
#include "timer.h"

class World : public Renderable, public Controllable
{
public:
    // worldfile is a file describing the world to be created
    World(const std::string& worldfile);
    virtual ~World();

    // set world offset (e.g. to be done by main character)
    void	setOffset(const Point& new_offset);
    // get world offset (e.g. for stuff that moves with the world)
    Point	getOffset() const;

    // implement renderable method
    virtual void draw();

    // implement collidable methods
    // for simple circles
    bool isCollidedR(const Point& centre,
		     const float& radius,
		     const int& layer,
		     const Collidable* caller) const;

    // for arbitrary shapes
    bool isCollidedV(const std::vector<Point>& vertices,
		     const int& layer,
		     const Collidable* caller) const;

    // implement controllable methods
    virtual void update();
    virtual void update(SDL_Event& event);

private:
    // the worldfile that this world is made from
    xmlpp::TextReader worldfile;
    
    // current offset of world
    Point offset;
    
    // drawing list
    GLuint map_list;

    // containers for references to objects
    // multimap of renderables
    typedef std::multimap<int, Renderable::Ptr> RendMap;
    RendMap renderables;
    // multimap of collidables
    typedef std::multimap<int, Collidable::Ptr> CollMap;
    CollMap collidables;
    // vector of controllables
    typedef std::vector<Controllable::Ptr> ConVect;
    ConVect controllables;

    // timer
    Timer timer;

    // utility functions for parsing the worldfile
    void parseWorldFile();
    void parseMap();
    void parseOrnaments();
    void parseUser();
    void parseEnemies();
};

#endif // WORLD_H
