/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// World class for drawing everything in the game

#ifndef WORLD_H
#define WORLD_H

#include <fstream>
#include <list>
#include <map>
#include <string>

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
    void setOffset(const Point& new_offset);
    // get world offset (e.g. for stuff that moves with the world)
    Point getOffset() const;

    // implement renderable method
    virtual void draw();

    // collision detection
    bool isCollided(const Point& centre, const float& radius,
                    const std::vector<Point>& vertices,
                    const int& layer, const Collidable* caller) const;

    // implement controllable methods
    virtual void update();
    virtual void update(SDL_Event& event);

    typedef std::multimap<int, Renderable::Ptr> RendMap;
    typedef std::multimap<int, Collidable::Ptr> CollMap;
    typedef std::list<Controllable::Ptr> ConList;

    // allows other classes to add to the containers
    RendMap::iterator addRenderable(const Renderable::Ptr& rend,
                                    const int& layer);
    CollMap::iterator addCollidable(const Collidable::Ptr& coll,
                                    const int& layer);
    ConList::iterator addControllable(const Controllable::Ptr& cont);

    // allows other classes to remove from the containers
    void remRenderable(const RendMap::iterator& pos);
    void remCollidable(const CollMap::iterator& pos);
    void remControllable(const ConList::iterator& pos);

private:
    // the worldfile that this world is made from
    xmlpp::TextReader worldfile;
    
    // current offset of world
    Point offset;
    
    // drawing list
    GLuint map_list;

    // containers for references to objects
    // multimap of renderables
    RendMap renderables;
    // multimap of collidables
    CollMap collidables;
    // vector of controllables
    ConList controllables;

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
