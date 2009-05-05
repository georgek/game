/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of health bar

#include <iostream>

#include "SDL.h"
#include "SDL_opengl.h"

#include "healthbar.h"

HealthBar::HealthBar (const Point& init_pos, const int& init_health,
                      const int& init_reload) :
    screenpos (init_pos),
    health (init_health),
    reload (init_reload)
{
    // construct
}

HealthBar::~HealthBar()
{
    // destruct
}

void HealthBar::draw()
{
    // offset position
    Point pos = screenpos + Point(0, -60);

    // container
    if (reload == 100) {
        glColor4f(0, 0, 0, 0.4);
        glBegin(GL_QUADS);
        glVertex3f(pos.getDispX()-22, pos.getDispY()+4, 0);
        glVertex3f(pos.getDispX()+22, pos.getDispY()+4, 0);
        glVertex3f(pos.getDispX()+22, pos.getDispY()-4, 0);
        glVertex3f(pos.getDispX()-22, pos.getDispY()-4, 0);
        glEnd();
    }
    else {
        glColor4f(0, 0, 0, 0.4);
        glBegin(GL_QUADS);
        glVertex3f(pos.getDispX()-22, pos.getDispY()+4, 0);
        glVertex3f(pos.getDispX()+22, pos.getDispY()+4, 0);
        glVertex3f(pos.getDispX()+22, pos.getDispY()-6, 0);
        glVertex3f(pos.getDispX()-22, pos.getDispY()-6, 0);
        glEnd();
    }

    // health bar
    // colours
    float r = 1.0f-health/200;
    float g = health/100;

    glColor4f(r, g, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(pos.getDispX()-19, pos.getDispY()+1, 0);
    glVertex3f(pos.getDispX()+38*health/100-19, pos.getDispY()+1, 0);
    glVertex3f(pos.getDispX()+38*health/100-19, pos.getDispY()-1, 0);
    glVertex3f(pos.getDispX()-19, pos.getDispY()-1, 0);
    glEnd();

    // reload bar
    if (reload != 100) {
        glColor4f(0.2, 0.6, 0.8, 1);
        glBegin(GL_QUADS);
        glVertex3f(pos.getDispX()-19, pos.getDispY()-3, 0);
        glVertex3f(pos.getDispX()+38*reload/100-19, pos.getDispY()-3, 0);
        glVertex3f(pos.getDispX()+38*reload/100-19, pos.getDispY()-4, 0);
        glVertex3f(pos.getDispX()-19, pos.getDispY()-4, 0);
        glEnd();
    }
}

void HealthBar::update(const Point& pos, const int& health, const int& reload)
{
    screenpos = pos;
    this->health = health;
    this->reload = reload;
}

