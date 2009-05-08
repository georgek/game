/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implements sprite class

#include <string>
#include <vector>

#include "point.h"
#include "sprite.h"
#include "texture.h"
#include "world.h"

Sprite::Sprite(World* world, const std::string& texturename,
               const Point& worldpos, const int& frames, const int& fps,
               const int& loops, const float& rotation) :
    world (world),
    texture (texturename, Texture::automatic),
    worldpos (worldpos),
    frames (frames),
    fps (fps),
    loops (loops),
    rotation (rotation),
    running (false),
    curr_frame (0),
    loops_done (0)
{
    // construct
}

Sprite::~Sprite()
{
    // destruct
}

void Sprite::draw()
{
    if (!running) {
        // not running yet
        return;
    }
    
    // find screen position
    Point screenpos =  worldpos - world->getOffset();

    if (!timer.is_started()) {
        // start timer
        timer.start();
    }
    else {
        // if enough time has passed, switch frame
        if (timer.get_ticks() > 1000/fps) {
            // switch frame
            curr_frame = curr_frame + timer.get_ticks() / (1000/fps);
            if (curr_frame >= frames) {
                curr_frame = curr_frame%frames;
                ++loops_done;
            }
            // restart timer
            timer.start();
        }
    }

    if (loops != 0  && loops_done >= loops) {
        // enough loops done, delete self
        world->remRenderable(me);
        return;
    }

    glPushMatrix();

    // move to correct positon on screen
    glTranslatef(screenpos.getDispX(),screenpos.getDispY(),0);
    // rotate
    glRotatef(rotation, 0, 0, 1);
    
    // calculate texture vertices
    std::vector<Point> texVert;
    int w = texture.getWidth()/frames;
    int h = texture.getHeight();
    texVert.push_back(Point(w/2,h/2));
    texVert.push_back(Point(w/2,-h/2));
    texVert.push_back(Point(-w/2,-h/2));
    texVert.push_back(Point(-w/2,h/2));

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glBindTexture(GL_TEXTURE_2D, texture.getTexId());

    glBegin(GL_QUADS);
    glTexCoord2f((curr_frame+1.0f)/frames, 1.0);
    glVertex3f(texVert[0].getDispX(),texVert[0].getDispY(),0);
    glTexCoord2f((curr_frame+1.0f)/frames, 0.0);
    glVertex3f(texVert[1].getDispX(),texVert[1].getDispY(),0);
    glTexCoord2f((curr_frame+0.0f)/frames, 0.0);
    glVertex3f(texVert[2].getDispX(),texVert[2].getDispY(),0);
    glTexCoord2f((curr_frame+0.0f)/frames, 1.0);
    glVertex3f(texVert[3].getDispX(),texVert[3].getDispY(),0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
}

void Sprite::run(World::RendMap::iterator you)
{
    me = you;
    running = true;
}

