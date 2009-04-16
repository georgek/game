/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implements turret class

#include <algorithm>
#include <iostream>

#include "SDL.h"
#include "SDL_opengl.h"

#include "pnpoly.h"
#include "texture.h"
#include "transform.h"
#include "turret.h"
#include "world.h"

Turret::Turret(World* world, const std::string& texturename,
	       const Point& init_pos, const int& layer) :
    world (world),
    worldpos (init_pos),
    curr_angle (0),
    radius(0),
    texture (texturename, Texture::automatic),
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

Turret::Turret(World* world, const Point& init_pos,
	       const std::string& filename, const int& layer) :
    world(world),
    worldpos(init_pos),
    curr_angle(0),
    layer(layer)
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
    Point screenpos =  worldpos - world->getOffset();
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

bool Turret::isCollidedR(const Point& centre,
			 const float& radius) const 
{
    // distance between points
    if ((worldpos + real_offset) % centre < (this->radius + radius)) {
	// collided
	return true;
    }
    return false;
}

bool Turret::isCollidedV(const std::vector<Point>& vertices) const 
{
    // work out current vertices
    std::vector<Point> curr_vertices;
    // transform the vertices using the transform functor
    std::transform(this->vertices.begin(), this->vertices.end(),
		   std::back_inserter(curr_vertices),
		   Transform(worldpos+real_offset, curr_angle));

    // check that none of the other vertices are in this polygon
    std::vector<Point>::const_iterator pos;
    for (pos = vertices.begin(); pos < vertices.end(); ++pos) {
	if (wrf::pnpoly(curr_vertices, *pos)) {
	    return true;
	}
    }
    // now check that none of these vertices are in the other polygon
    for (pos = curr_vertices.begin(); pos < curr_vertices.end(); ++pos) {
	if (wrf::pnpoly(vertices, *pos)) {
	    return true;
	}
    }
    return false;
}

void Turret::setWorldPos (const Point& pos) 
{
    worldpos = pos;
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
    // transform offset
    Transform offset_trans(Point(0,0), curr_angle);
    real_offset = offset_trans(offset);
}

void Turret::incTurretAngle (const float& theta) 
{
    curr_angle += theta;
    if (curr_angle > 360) curr_angle = curr_angle - 360;
    if (curr_angle < 0) curr_angle = 360 + curr_angle;
    // transform offset
    Transform offset_trans(Point(0,0), curr_angle);
    real_offset = offset_trans(offset);
}

bool Turret::isCollided (const Point& new_worldpos, 
			 const float& angle_inc) const
{
    // work out new vertices
    std::vector<Point> new_vertices;
    // transform the vertices using the transform functor
    std::transform(vertices.begin(), vertices.end(), // source
                   std::back_inserter(new_vertices), // destination
                   Transform(new_worldpos+real_offset, 
                             curr_angle+angle_inc));

    // check for collisions
    if (world->isCollided(new_worldpos+real_offset, radius, new_vertices,
                          layer, this)) {
        // it has collided
        return true;
    }
    return false;
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
		// transform offset
		Transform offset_trans(Point(0,0), curr_angle);
		real_offset = offset_trans(offset);
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
