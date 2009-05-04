/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of cursor class

#include <iostream>
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_opengl.h"

#include "cursor.h"
#include "point.h"
#include "texture.h"

Cursor::Cursor (const std::string& texture_name, const int& rpm) :
    texture (texture_name, Texture::automatic),
    mouse_pos (0,0),
    rpm (rpm),
    heading (0)
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
    glVertex3f(texVert[0].getX(),texVert[0].getY(),0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(texVert[1].getX(),texVert[1].getY(),0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(texVert[2].getX(),texVert[2].getY(),0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(texVert[3].getX(),texVert[3].getY(),0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    
    glEndList();
}

Cursor::~Cursor()
{
    // destruct stuff
}

void Cursor::update()
{
    // update stuff
}
void Cursor::update(SDL_Event& event)
{
    if (event.type == SDL_MOUSEMOTION) {
	mouse_pos.setX(event.motion.x-SDL_GetVideoInfo()->current_w/2);
	mouse_pos.setY(SDL_GetVideoInfo()->current_h/2-event.motion.y);
    }
}

void Cursor::draw()
{
    // update heading
    heading -= (rpm*timer.get_ticks())/(60000.0f/360.0f);
    if (heading > 360) heading = heading - 360;
    if (heading < 0) heading = 360 + heading;

    glPushMatrix();

    // move to correct positon on screen
    glTranslatef(mouse_pos.getX(),mouse_pos.getY(),0);
    // rotate to face correct direction
    glRotatef(heading, 0, 0, 1);

    // call list
    glCallList(drawing_list);
    
    glPopMatrix();

    timer.start();
}
