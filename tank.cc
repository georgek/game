/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of Tank class

#include <string>

#include "tank.h"
#include "world.h"

Tank::Tank(World* world, const Turret::Ptr& turret, 
	   const std::string& texturename, 
	   const int& init_x, const int& init_y,
	   const int& engine_force, const int& mass, const int& rpm) :
    world(world),
    turret(turret),
    texture(texturename, automatic),
    worldpos(init_x, init_y),
    engine_force(engine_force),
    mass(mass),
    rpm(rpm) 
{
    // calculate vertices
    int w = texture.getWidth();
    int h = texture.getHeight();
    vertices.push_back(Point(w/2,h/2));
    vertices.push_back(Point(w/2,-h/2));
    vertices.push_back(Point(-w/2,-h/2));
    vertices.push_back(Point(-w/2,h/2));

    // make new drawing list
    drawing_list = glGenLists(1);
    glNewList(drawing_list, GL_COMPILE);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glBindTexture(GL_TEXTURE_2D, texture.getTexId());

    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(vertices[0].getDispX(),vertices[0].getDispY(),0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(vertices[1].getDispX(),vertices[1].getDispY(),0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(vertices[2].getDispX(),vertices[2].getDispY(),0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(vertices[3].getDispX(),vertices[3].getDispY(),0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    
    glEndList();
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
    
    // draw turret
    glLoadIdentity();
    
    glPushMatrix();
    // move to correct screen position
    glTranslatef(screenpos.getDispX(), screenpos.getDispY(), 0);
    
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

