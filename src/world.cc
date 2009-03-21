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

#include "point.h"
#include "tank.h"
#include "texture.h"
#include "turret.h"
#include "usertank.h"
#include "world.h"

World::World (const std::string& inputworldfile) :
    worldfile (inputworldfile.c_str()),
    xOffset (0),
    yOffset (0) 
{
    // now parse the worldfile and build the world
    try {
	parseWorldFile();
    }
    catch (const std::exception& e) {
	std::cout << "XML Exception: " << e.what() << std::endl;
    }

    // // add a turret
    // Turret::Ptr turret (new Turret(this, 512, 512, "turret.xml"));
    // // add to renderable map
    // renderables.insert(std::make_pair(2, turret));
    // // add to collidable map
    // collidables.insert(std::make_pair(2, turret));
    
    // // add tank
    // UserTank::Ptr tank (new UserTank(this, 1, turret, 512, 512, "tank.xml"));

    // // add to renderable map
    // renderables.insert(std::make_pair(1, tank));
    // // add to collidable map
    // collidables.insert(std::make_pair(1, tank));
    // // add to controllable vector
    // controllables.push_back(tank);

    // add another
    // add a turret
    Turret::Ptr turret2 (new Turret(this, "textures/tankturret4.png", 
				   256, 256));
    // add to renderable map
    renderables.insert(std::make_pair(2, turret2));
    // add to collidable map
    collidables.insert(std::make_pair(2, turret2));
    
    // add tank
    Tank::Ptr tank2 (new Tank(this, 1, turret2, "textures/tankbody4.png",
			     256, 256, 10, 100, 18));
    // add to renderable map
    renderables.insert(std::make_pair(1, tank2));
    // add to collidable map
    collidables.insert(std::make_pair(1, tank2));
}

World::~World () 
{
    // destruct stuff
}

void World::setXOffset(const float& x) 
{
    xOffset = x;
}

void World::setYOffset(const float& y) 
{
    yOffset = y;
}

float World::getXOffset() const 
{
    return xOffset;
}

float World::getYOffset() const 
{
    return yOffset;
}

void World::draw()
{
    // clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // offset world
    glPushMatrix();
    Point offset (-xOffset, -yOffset);
    glTranslatef(offset.getDispX(), offset.getDispY(), 0);

    // draw map
    glCallList(map_list);

    glPopMatrix();

    // draw all renderables
    RendMap::iterator pos;
    for (pos = renderables.begin(); pos != renderables.end(); ++pos) {
	pos->second->draw();
    }

    // swap buffers
    SDL_GL_SwapBuffers();
}

bool World::isCollidedR (const float& centre_x,
			 const float& cetnre_y,
			 const float& radius,
			 const int& layer) const 
{
    // do collision detection
    return false;
}

bool World::isCollidedV (const std::vector<Point>& vertices,
			 const int& layer) const 
{
    // do collision detection
    return false;
}

void World::update (SDL_Event& event) 
{
    // update all controllables
    ConVect::iterator pos;
    for (pos = controllables.begin(); pos != controllables.end(); ++pos) {
	(*pos)->update(event);
    }
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
		Turret::Ptr turret (new Turret(this, init_x, init_y, 
					       turretfile));
		// add to renderable map
		renderables.insert(std::make_pair(turretlayer, turret));
		// add to collidable map
		collidables.insert(std::make_pair(turretlayer, turret));
    
		// add tank
		UserTank::Ptr tank (new UserTank(this, 1, turret, 
						 init_x, init_y, bodyfile));

		// add to renderable map
		renderables.insert(std::make_pair(1, tank));
		// add to collidable map
		collidables.insert(std::make_pair(1, tank));
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
    int width, height;
    std::stringstream s;
    worldfile.move_to_first_attribute();
    s << worldfile.get_value().raw();
    s >> width;
    s.str(""); s.clear();
    worldfile.move_to_next_attribute();
    s << worldfile.get_value().raw();
    s >> height;
    s.str(""); s.clear();
    
    // read rest of the map
    // textures
    typedef std::map<int, Texture::sPtr> TexMap;
    TexMap texture_list;
    // vector for tile textures
    std::vector<int> tile_textures;
    // vector for tile tangibility
    std::vector<bool> tile_tangibility;
    // reserve space for all tiles
    tile_textures.reserve(width*height);
    tile_tangibility.reserve(width*height);
    // tile dimensions
    Point td (0, 0);
    // something for ornaments

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
	    int tw, th;
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
	    // do ornament stuff
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
    for (int j = height-1; j >= 0; --j) {
	y = th*j;
	for (int i = 0; i < width; ++i) {
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
