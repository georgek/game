/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of Tank class

#include <cmath>
#include <iostream>
#include <string>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

#include "tank.h"
#include "world.h"

const double PI = 4.0*std::atan(1.0);

Tank::Tank(World* world, const int& layer, const Turret::Ptr& turret, 
	   const std::string& texturename, 
	   const int& init_x, const int& init_y,
	   const int& engine_force, const int& mass, const int& rpm) :
    world(world),
    turret(turret),
    texture(texturename, Texture::automatic),
    worldpos(init_x, init_y),
    engine_force(engine_force),
    mass(mass),
    rpm(rpm),
    velocity(0),
    friction(0),
    curr_rpm(0),
    heading(0),
    xf(0),
    yf(1),
    mouse_x(0),
    mouse_y(0),
    turret_rpm(25),
    curr_turret_rpm(0),
    layer(layer)
{
    // calculate vertices
    int w = texture.getWidth();
    int h = texture.getHeight();
    vertices.push_back(Point(w/2,h/2));
    vertices.push_back(Point(w/2,-h/2));
    vertices.push_back(Point(-w/2,-h/2));
    vertices.push_back(Point(-w/2,h/2));

    makeDrawingList();
}

Tank::Tank(World* world, const int& layer, const Turret::Ptr& turret,
	   const int& init_x, const int& init_y,
	   const std::string& inputfile) :
    world(world),
    turret(turret),
    worldpos(init_x, init_y),
    velocity(0),
    friction(0),
    curr_rpm(0),
    heading(0),
    xf(0),
    yf(1),
    mouse_x(0),
    mouse_y(0),
    turret_rpm(25),
    curr_turret_rpm(0),
    layer(layer) 
{
    // parse input file
    try {
	parseInputFile(inputfile);
    }
    catch (const std::exception& e) {
	std::cout << "XML Exception in file " << inputfile 
		  << ": " << e.what() << std::endl;
    }

    makeDrawingList();
}

Tank::~Tank() 
{
    
}

void Tank::draw() 
{
    // find screen position
    Point screenpos (worldpos.getX() - world->getXOffset(),
    		     worldpos.getY() - world->getYOffset());
    // (re)start timer
    timer.start();

    glPushMatrix();

    // move to correct positon on screen
    glTranslatef(screenpos.getDispX(),screenpos.getDispY(),0);
    // rotate to face correct direction
    glRotatef(heading, 0, 0, 1);
    
    // call list
    glCallList(drawing_list);
    
    glPopMatrix();
}

bool Tank::isCollidedR(const float& centre_x,
		       const float& centre_y,
		       const float& radius) const 
{
    return false;
}

bool Tank::isCollidedV(const std::vector<Point>& vertices) const 
{
    return false;
}

void Tank::makeDrawingList() 
{
    // calculate texture vertices
    std::vector<Point> texVert;
    int w = texture.getWidth();
    int h = texture.getHeight();
    texVert.push_back(Point(w/2,h/2));
    texVert.push_back(Point(w/2,-h/2));
    texVert.push_back(Point(-w/2,-h/2));
    texVert.push_back(Point(-w/2,h/2));

    // make new drawing list
    drawing_list = glGenLists(1);
    glNewList(drawing_list, GL_COMPILE);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glBindTexture(GL_TEXTURE_2D, texture.getTexId());

    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(texVert[0].getDispX(),texVert[0].getDispY(),0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(texVert[1].getDispX(),texVert[1].getDispY(),0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(texVert[2].getDispX(),texVert[2].getDispY(),0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(texVert[3].getDispX(),texVert[3].getDispY(),0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    
    glEndList();
}

void Tank::parseInputFile(const std::string& filename) 
{
    std::stringstream s;
    // open file
    xmlpp::TextReader inputfile(filename.c_str());
    // parse it
    while (inputfile.read()) {
	if (inputfile.get_node_type() == xmlpp::TextReader::Element) {
	    if (inputfile.get_name() == "tank") {
		// get attributes
		inputfile.move_to_first_attribute();
		s << inputfile.get_value();
		s >> engine_force;
		s.str(""); s.clear();
		inputfile.move_to_next_attribute();
		s << inputfile.get_value();
		s >> mass;
		s.str(""); s.clear();
		inputfile.move_to_next_attribute();
		s << inputfile.get_value();
		s >> rpm;
		s.str(""); s.clear();
	    }
	    // switch for element type
	    else if (inputfile.get_name() == "texture") {
		// set texture
		inputfile.move_to_first_attribute();
		texture = Texture(inputfile.get_value().raw(), 
				  Texture::automatic);
	    }
	    else if (inputfile.get_name() == "vertices") {
		// read each vertex
		int x, y;
		while (inputfile.read()) {
		    if (inputfile.get_node_type() == 
			xmlpp::TextReader::EndElement) {
			// end of vertex list
			break;
		    }
		    else if (inputfile.get_name() == "v") {
			inputfile.move_to_first_attribute();
			s << inputfile.get_value();
			s >> x;
			s.str(""); s.clear();
			inputfile.move_to_next_attribute();
			s << inputfile.get_value();
			s >> y;
			s.str(""); s.clear();
			// insert into vector
			vertices.push_back(Point(x,y));
		    }
		}
	    }
	    else if (inputfile.get_name() == "radius") {
		// store radius
		inputfile.move_to_first_attribute();
		s << inputfile.get_value();
		s >> radius;
		s.str(""); s.clear();
	    }
	}
    }
}
