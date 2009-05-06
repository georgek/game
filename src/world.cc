/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Implementation of World class

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <tr1/memory>
#include <vector>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

#include "SDL.h"
#include "SDL_opengl.h"

#include "aitank.h"
#include "cursor.h"
#include "ornament.h"
#include "pnpoly.h"
#include "point.h"
#include "tank.h"
#include "texture.h"
#include "turret.h"
#include "usertank.h"
#include "world.h"

World::World (const std::string& inputworldfile) :
    worldfile (inputworldfile.c_str())
{
    // now parse the worldfile and build the world
    try {
	parseWorldFile();
    }
    catch (const std::exception& e) {
	std::cout << "XML Exception: " << e.what() << std::endl;
    }

    // print tangibility for debugging
    // print rows
    for (int r = 0; r < map_height; ++r) {
        // print columns
        for (int c = 0; c < map_width; ++c) {
            std::cout << tile_tangibility[r*map_width+c];
        }
        std::cout << std::endl;
    }

    // find boundary vertices using the Marching Squares algorithm
    // find the first tangible square
    int x = 1;
    int y = 1;
    while (tile_tangibility[(x-1)+(y-1)*map_width]
           && tile_tangibility[x+(y-1)*map_width]
           && tile_tangibility[(x-1)+y+map_width]
           && tile_tangibility[x+y*map_width]) {
        // advance point
        if (++x >= map_width) {
            x = 0;
            if (++y >= map_height) {
                break;
            }
        }
    }
    Point curr (x, y);
    // directions
    Point N (0,-1);
    Point E (1, 0);
    Point S (0, 1);
    Point W (-1,0);

    // first point must be a vertex
    boundary.push_back(curr);
    // trace shape
    Point prev_direction;
    do {
        // find direction we need to move based on which squares are
        // tangible

        // something clever found here:
        // <http://www.tomgibara.com/computer-vision/MarchingSquares.java>
        int sum = 0;
        if (!tile_tangibility[(curr.getX()-1)+(curr.getY()-1)*map_width])
            sum |= 1;
        if (!tile_tangibility[curr.getX()+(curr.getY()-1)*map_width])
            sum |= 2;
        if (!tile_tangibility[(curr.getX()-1)+curr.getY()*map_width])
            sum |= 4;
        if (!tile_tangibility[curr.getX()+curr.getY()*map_width])
            sum |= 8;
        switch (sum) {
        case  1:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += N;
            prev_direction = N;
            break;
        case  2:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += E;
            prev_direction = E;
            break;
        case 3:
            curr += E;
            prev_direction = E;
            break;
        case  4:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += W;
            prev_direction = W;
            break;
        case  5:
            curr += N;
            prev_direction = N;
            break;
        case  6:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += prev_direction == N ? W : E;
            prev_direction = prev_direction == N ? W : E;
            break;
        case  7:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += E;
            prev_direction = E;
            break;
        case  8:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += S;
            prev_direction = S;
            break;
        case  9:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += prev_direction == E ? N : S;
            prev_direction = prev_direction == E ? N : S;
            break;
        case 10:
            curr += S;
            prev_direction = S;
            break;
        case 11:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += S;
            prev_direction = S;
            break;
        case 12:
            curr += W;
            prev_direction = W;
            break;
        case 13:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += N;
            prev_direction = N;
            break;
        case 14:
            if (curr != boundary.front()) {
                // this is a vertex so add it
                boundary.push_back(curr);
            }
            curr += W;
            prev_direction = W;
            break;
        default: throw 1;
        }
    } while (curr != boundary.front());

    // print each vertex for debugging
    // for (std::vector<Point>::iterator pos = boundary.begin();
    //      pos != boundary.end(); ++pos) {
    //     std::cout << *pos << std::endl;
    // }

    // convert them to world coordinates
    for (std::vector<Point>::iterator pos = boundary.begin();
         pos != boundary.end(); ++pos) {
        pos->setX(pos->getX()*tw);
        pos->setY((map_height-pos->getY())*th);
    }

    // for (std::vector<Point>::iterator pos = boundary.begin();
    //      pos != boundary.end(); ++pos) {
    //     std::cout << *pos << std::endl;
    // }
}

World::~World () 
{
    // destruct stuff
}

void World::setOffset(const Point& new_offset) 
{
    offset = new_offset;
}

Point World::getOffset() const 
{
    return offset;
}

void World::draw()
{
    // clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // offset world
    glPushMatrix();
    glTranslatef(-offset.getDispX(), -offset.getDispY(), 0);

    // draw map
    glCallList(map_list);

    glPopMatrix();

    // draw all renderables
    // note iterators may be invalidated after draw is called
    RendMap::iterator pos = renderables.begin();
    while (pos != renderables.end()) {
	(pos++)->second->draw();
    }
}

bool World::isCollided (const Point& centre, const float& radius,
                        const std::vector<Point>& vertices,
                        const int& layer, const Collidable* caller) const
{
    // check collision with world, check that all of the vertices are
    // inside the boundary
    for (std::vector<Point>::const_iterator pos = vertices.begin();
         pos < vertices.end(); ++pos) {
	if (!wrf::pnpoly(boundary, *pos)) {
	    return true;
	}
    }
    // now check that no boundary vertices are in the polygon
    for (std::vector<Point>::const_iterator pos = boundary.begin();
         pos < boundary.end(); ++pos) {
        if (wrf::pnpoly(vertices, *pos)) {
            return true;
        }
    }

    // check all collidables in layer
    for (CollMap::const_iterator pos = collidables.lower_bound(layer);
	 pos != collidables.upper_bound(layer); ++pos) {
	if (pos->second.get() != caller
	    && pos->second->isCollidedR(centre, radius)) {
	    // might have collided with this object, do vertices check
            if (pos->second->isCollidedV(vertices)) {
                // has collided
                return true;
            }
	}
    }
    return false;
}

void World::update ()
{
    // update all controllables
    ConList::iterator pos = controllables.begin();
    while (pos != controllables.end()) {
	(*(pos++))->update();
    }
}

void World::update (SDL_Event& event) 
{
    // update all controllables
    ConList::iterator pos = controllables.begin();
    while (pos != controllables.end()) {
	(*(pos++))->update(event);
    }
}

World::RendMap::iterator World::addRenderable (const Renderable::Ptr& rend, 
                                        const int& layer)
{
    return renderables.insert(std::make_pair(layer, rend));
}

World::CollMap::iterator World::addCollidable (const Collidable::Ptr& coll, 
                                        const int& layer)
{
    return collidables.insert(std::make_pair(layer, coll));
}

World::ConList::iterator World::addControllable (const Controllable::Ptr& cont)
{
    controllables.push_back(cont);
    return --controllables.end();
}

void World::remRenderable (World::RendMap::iterator& pos)
{
    renderables.erase(pos++);
}

void World::remCollidable (World::CollMap::iterator& pos)
{
    collidables.erase(pos++);
}

void World::remControllable (World::ConList::iterator& pos)
{
    controllables.erase(pos++);
}

// utility function for parsing the worldfile
void World::parseWorldFile () 
{
    // iterate over file
    while (worldfile.read()) {
	// if it is an Element (not an EndElement)
	if (worldfile.get_node_type() == xmlpp::TextReader::Element) {
	    // switch for element type
	    if (worldfile.get_name() == "map") {
		parseMap();
	    }
	    else if (worldfile.get_name() == "usertank") {
		// parse user tank
		int bodylayer, turretlayer, init_x, init_y;
		std::stringstream s;
		worldfile.move_to_first_attribute();
		std::string bodyfile = worldfile.get_value();
		worldfile.move_to_next_attribute();
		std::string turretfile = worldfile.get_value();
		worldfile.move_to_next_attribute();
		s << worldfile.get_value().raw();
		s >> bodylayer;
		s.str(""); s.clear();
		worldfile.move_to_next_attribute();
		s << worldfile.get_value().raw();
		s >> turretlayer;
		s.str(""); s.clear();
		worldfile.move_to_next_attribute();
		s << worldfile.get_value().raw();
		s >> init_x;
		s.str(""); s.clear();
		worldfile.move_to_next_attribute();
		s << worldfile.get_value().raw();
		s >> init_y;
		s.str(""); s.clear();

		// add the turret
		Turret::Ptr turret (new Turret(this, Point(init_x, init_y), 
					       turretfile, turretlayer));
		// add to renderable map
		renderables.insert(std::make_pair(turretlayer, turret));
		// add to collidable map
		collidables.insert(std::make_pair(turretlayer, turret));
    
		// add tank
		UserTank::Ptr tank (new UserTank(this, bodylayer, turret, 
						 Point(init_x, init_y), 
						 bodyfile));
                
		// add to renderable map
		renderables.insert(std::make_pair(bodylayer, tank));
		// add to collidable map
		collidables.insert(std::make_pair(bodylayer, tank));
		// add to controllable vector
		controllables.push_back(tank);
	    }
	    else if (worldfile.get_name() == "enemies") {
		// parse enemies here
	    }
	}
    }
}

// parse the map
void World::parseMap () 
{
    // get map attributes
    std::stringstream s;
    worldfile.move_to_first_attribute();
    s << worldfile.get_value().raw();
    s >> map_width;
    s.str(""); s.clear();
    worldfile.move_to_next_attribute();
    s << worldfile.get_value().raw();
    s >> map_height;
    s.str(""); s.clear();
    
    // read rest of the map
    // textures
    // reserve space for all tiles
    tile_textures.reserve(map_width*map_height);
    tile_tangibility.reserve(map_width*map_height);
    // tile dimensions
    Point td (0, 0);
    while (worldfile.read()) {
	if (worldfile.get_node_type() == xmlpp::TextReader::EndElement) {
	    // end of of map element
	    break;
	}
	else if (worldfile.get_name() == "texture") {
	    // insert texture into texture map
	    // get id
	    worldfile.move_to_first_attribute();
	    int texture_id;
	    s << worldfile.get_value().raw();
	    s >> texture_id;
	    s.str(""); s.clear();
	    // get filename
	    worldfile.move_to_next_attribute();
	    std::string filename = worldfile.get_value();
	    // get type
	    worldfile.move_to_next_attribute();
	    Texture::texture_types type;
	    if (worldfile.get_value() == "png") {
		type = Texture::png;
	    }
	    else if (worldfile.get_value() == "raw") {
		type = Texture::raw;
	    }
	    else {
		type = Texture::automatic;
	    }
	    // add to map
	    texture_list[texture_id] = Texture::sPtr(
		new Texture (filename, type));
	}
	else if (worldfile.get_name() == "tiles") {
	    // get tile dimensions
	    worldfile.move_to_first_attribute();
	    s << worldfile.get_value().raw();
	    s >> tw;
	    s.str(""); s.clear();
	    worldfile.move_to_next_attribute();
	    s << worldfile.get_value().raw();
	    s >> th;
	    s.str(""); s.clear();
	    td = Point(tw, th);
	    
	    // read each tile element
	    while (worldfile.read()) {
		if (worldfile.get_node_type() == 
		    xmlpp::TextReader::EndElement) {
		    break;
		}
		if (worldfile.get_name() == "t") {
		    // it's a tile
		    int texture_id;
		    bool tangible;
		    worldfile.move_to_first_attribute();
		    s << worldfile.get_value().raw();
		    s >> texture_id;
		    s.str(""); s.clear();
		    worldfile.move_to_next_attribute();
		    s << worldfile.get_value().raw();
		    s >> tangible;
		    s.str(""); s.clear();
		    // insert into vectors
		    tile_textures.push_back(texture_id);
		    tile_tangibility.push_back(tangible);
		}
	    }
	}
	else if (worldfile.get_name() == "ornaments") {
	    // create all ornaments
	    while (worldfile.read()) {
		if (worldfile.get_node_type() == 
		    xmlpp::TextReader::EndElement) {
		    break;
		}
		else if (worldfile.get_name() == "ornament") {
		    // make ornament
		    int init_x, init_y, rotation, renderlayer, collidelayer;
                    //std::string collidelayers;
		    std::stringstream s;
		    worldfile.move_to_first_attribute();
		    std::string file = worldfile.get_value();
		    worldfile.move_to_next_attribute();
//		    collidelayers = worldfile.get_value();
                    s << worldfile.get_value().raw();
                    s >> collidelayer;
                    s.str(""); s.clear();
                    worldfile.move_to_next_attribute();
                    s << worldfile.get_value().raw();
                    s >> renderlayer;
                    s.str(""); s.clear();
		    worldfile.move_to_next_attribute();
		    s << worldfile.get_value().raw();
		    s >> init_x;
		    s.str(""); s.clear();
		    worldfile.move_to_next_attribute();
		    s << worldfile.get_value().raw();
		    s >> init_y;
		    s.str(""); s.clear();
                    worldfile.move_to_next_attribute();
                    s << worldfile.get_value().raw();
                    s >> rotation;
                    s.str(""); s.clear();

		    // make an ornament
		    Ornament::Ptr ornament (new Ornament(this,
                                                         Point(init_x, init_y),
                                                         file, rotation));

		    // add to renderable map
		    renderables.insert(std::make_pair(renderlayer, ornament));
		    // add to collidable map
		    collidables.insert(std::make_pair(collidelayer, ornament));
		}
	    }
	}
    }
    
    // make map from that info
    // make new drawing list
    map_list = glGenLists(1);
    glNewList (map_list, GL_COMPILE);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    // texture size
    float tw = td.getDispX();
    float th = td.getDispY();
    
    // draw tiles
    int x, y;
    std::vector<int>::iterator pos = tile_textures.begin();
    for (int j = map_height-1; j >= 0; --j) {
	y = th*j;
	for (int i = 0; i < map_width; ++i) {
	    x = tw*i;
	    // bind texture
	    glBindTexture(GL_TEXTURE_2D, texture_list[*pos]->getTexId());
	    // draw square
	    glBegin(GL_QUADS);
	    glColor4f(0, 0, 0, 0);
	    glTexCoord2f(0.0, 1.0); glVertex3f(x,y+th,0);
	    glTexCoord2f(1.0, 1.0); glVertex3f(x+tw,y+th,0);
	    glTexCoord2f(1.0, 0.0); glVertex3f(x+tw,y,0);
	    glTexCoord2f(0.0, 0.0); glVertex3f(x,y,0);
	    glEnd();

	    // next tile
	    if (pos < tile_textures.end()) ++pos;
	}
    }
    glDisable(GL_TEXTURE_2D);
    glEndList();
}
