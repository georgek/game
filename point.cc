/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of point

#include "SDL.h"
#include "SDL_opengl.h"

#include "point.h"

Point::Point (const float& x, const float& y) :
    x(x),
    y(y) 
{
    
}

Point::~Point () 
{
    
}

float Point::getX() const 
{
    return x;
}

float Point::getY() const 
{
    return y;
}

float Point::getDispX() const 
{
    return (SDL_GetVideoInfo()->current_h
	    / static_cast<float>(Point::refh)) * x;
}

float Point::getDispY() const 
{
    return (SDL_GetVideoInfo()->current_h
	    / static_cast<float>(Point::refh)) * y;
}

void Point::setX (const float& x) 
{
    this->x = x;
}

void Point::setY (const float& by) 
{
    this->y = y;
}

int Point::refw = 1280;
int Point::refh = 1024;
