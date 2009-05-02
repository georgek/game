/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implements bullet class

#include <cmath>

#include "SDL.h"
#include "SDL_opengl.h"

#include "bullet.h"
#include "world.h"

const double PI = 4.0*std::atan(1.0);

Bullet::Bullet (World* world, const std::string& texturename,
                const Point& init_pos, const Point& destination,
                const float& start_radius,
                const int& speed, const int& damage) :
    world (world),
    worldpos (init_pos),
    destination (destination),
    start_radius (start_radius),
    texture (texturename, Texture::automatic),
    damage (damage),
    speed (speed)
{
    heading = init_pos / destination;
    xf = std::cos((heading+90) * PI/180.0);
    yf = std::sin((heading+90) * PI/180.0);

    // move to start radius position (at end of turret)
    worldpos.setX(worldpos.getX() + start_radius * xf);
    worldpos.setY(worldpos.getY() + start_radius * yf);

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

Bullet::~Bullet ()
{
    // destruct
}


void Bullet::draw()
{
    if (!running) {
        // not running yet
        return;
    }
    
    float ticks = timer.get_ticks();
    // move bullet towards destination
    Point new_worldpos;
    new_worldpos.setX(worldpos.getX() + speed*xf*ticks/1000.0);
    new_worldpos.setY(worldpos.getY() + speed*yf*ticks/1000.0);
    // check if it's at the destination
    if (new_worldpos == destination
        || std::abs((new_worldpos / destination) - heading) > 1) {
        // it's at or past the destination
        worldpos = destination;
        // make an explosion event here
        // remove self from world
        world->remRenderable(this_bullet);
        return;
    }
    else {
        worldpos = new_worldpos;
    }
    
    // find screen position
    Point screenpos =  worldpos - world->getOffset();

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

void Bullet::run(World::RendMap::iterator thisBullet)
{
    // accept iterator to itself and run
    this_bullet = thisBullet;
    running = true;
}
