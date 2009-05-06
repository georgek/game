/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of ornament class

#include <algorithm>
#include <string>
#include <vector>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

#include "ornament.h"
#include "pnpoly.h"
#include "point.h"
#include "transform.h"

Ornament::Ornament (World* world, const Point& pos,
                    const std::string& filename, float rotation) :
    world (world),
    worldpos (pos),
    heading (rotation)
{
    // parse input file
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
	    else if (inputfile.get_name() == "radius") {
		// store radius
		inputfile.move_to_first_attribute();
		s << inputfile.get_value();
		s >> radius;
		s.str(""); s.clear();
	    }
	}
    }

    // make drawing list
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

Ornament::~Ornament() 
{
    // destruct
}

void Ornament::draw()
{
    // find screen position
    Point screenpos =  worldpos - world->getOffset();

    glPushMatrix();

    // move to correct positon on screen
    glTranslatef(screenpos.getDispX(),screenpos.getDispY(),0);
    // rotate to face correct direction
    glRotatef(heading, 0, 0, 1);
    
    // call list
    glCallList(drawing_list);
    
    glPopMatrix();
}

bool Ornament::isCollidedR(const Point& centre,
                           const float& radius) const
{
    // distance between points
    if (worldpos % centre < (this->radius + radius)) {
	// collided
	return true;
    }
    return false;
}
bool Ornament::isCollidedV(const std::vector<Point>& vertices) const
{
    // work out current vertices
    std::vector<Point> curr_vertices;
    // transform the vertices using the transform functor
    std::transform(this->vertices.begin(), this->vertices.end(),
		   std::back_inserter(curr_vertices),
		   Transform(worldpos, heading));

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

