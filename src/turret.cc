/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implements turret class

#include <iostream>

#include "SDL.h"
#include "SDL_opengl.h"

#include "texture.h"
#include "turret.h"
#include "world.h"

Turret::Turret(World* world, const std::string& texturename,
	       const int& init_x, const int& init_y) :
    world (world),
    worldpos (init_x, init_y),
    curr_angle (0),
    radius(0),
    offset (0,0),
    texture (texturename, Texture::automatic)
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

Turret::Turret(World* world, const int& init_x, const int& init_y,
	       const std::string& filename) :
    world(world),
    worldpos(init_x, init_y),
    curr_angle(0)
{
    // read input file
    try {
	parseInputFile(filename);
    }
    catch (const std::exception& e) {
	std::cout << "XML Exception in file " << filename 
		  << ": " << e.what() << std::endl;
    }

    makeDrawingList();
}

Turret::~Turret () 
{
    
}

void Turret::draw() 
{
    // find screen position
    Point screenpos (worldpos.getX() - world->getXOffset(),
		     worldpos.getY() - world->getYOffset());
    // (re)start timer
    timer.start();
    
    // draw turret
    glPushMatrix();
    // move to correct screen position
    glTranslatef(screenpos.getDispX(), screenpos.getDispY(), 0);

    // rotate turret
    glRotatef(curr_angle, 0, 0, 1);

    // align properly on tank
    glTranslatef(offset.getDispX(), offset.getDispY(), 0);
    
    // call list
    glCallList(drawing_list);
    
    glPopMatrix();
}

bool Turret::isCollidedR(const float& centre_x, const float& centre_y,
			 const float& radius) const 
{
    return false;
}

bool Turret::isCollidedV(const std::vector<Point>& vertices) const 
{
    return false;
}

void Turret::setWorldX (const float& x) 
{
    worldpos.setX(x);
}

void Turret::setWorldY (const float& y) 
{
    worldpos.setY(y);
}

float Turret::getTurretAngle () const
{
    return curr_angle;
}

void Turret::setTurretAngle (const float& theta) 
{
    curr_angle = theta;
    if (curr_angle > 360) curr_angle = curr_angle - 360;
    if (curr_angle < 0) curr_angle = 360 + curr_angle;
}

void Turret::incTurretAngle (const float& theta) 
{
    curr_angle += theta;
    if (curr_angle > 360) curr_angle = curr_angle - 360;
    if (curr_angle < 0) curr_angle = 360 + curr_angle;
}

void Turret::makeDrawingList() 
{
    // calculate texture vertices
    int w = texture.getWidth();
    int h = texture.getHeight();
    std::vector<Point> texVert;
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

/*  this stuff draws the collision zone
    glBegin(GL_QUADS);
    glColor4f(1,0,0,1);
    glVertex3f(vertices[0].getDispX(),vertices[0].getDispY(),0);
    glVertex3f(vertices[1].getDispX(),vertices[1].getDispY(),0);
    glVertex3f(vertices[6].getDispX(),vertices[6].getDispY(),0);
    glVertex3f(vertices[7].getDispX(),vertices[7].getDispY(),0);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f(1,0,0,1);
    glVertex3f(vertices[1].getDispX(),vertices[1].getDispY(),0);
    glVertex3f(vertices[2].getDispX(),vertices[2].getDispY(),0);
    glVertex3f(vertices[3].getDispX(),vertices[3].getDispY(),0);
    glVertex3f(vertices[4].getDispX(),vertices[4].getDispY(),0);
    glVertex3f(vertices[5].getDispX(),vertices[5].getDispY(),0);
    glVertex3f(vertices[6].getDispX(),vertices[6].getDispY(),0);
    glEnd();*/
    
    glEndList();
}

void Turret::parseInputFile(const std::string& filename) 
{
    std::stringstream s;
    // open file
    xmlpp::TextReader inputfile(filename.c_str());
    // parse it
    while (inputfile.read()) {
	if (inputfile.get_node_type() == xmlpp::TextReader::Element) {
	    // switch for element type
	    if (inputfile.get_name() == "texture") {
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
	    else if (inputfile.get_name() == "offset") {
		// store offset
		int x, y;
		inputfile.move_to_first_attribute();
		s << inputfile.get_value();
		s >> x;
		s.str(""); s.clear();
		inputfile.move_to_next_attribute();
		s << inputfile.get_value();
		s >> y;
		s.str(""); s.clear();
		offset = Point(x,y);
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
