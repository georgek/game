/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of point

#include <cmath>

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

void Point::setY (const float& y) 
{
    this->y = y;
}

// operators
bool Point::operator== (const Point& other) const
{
    return (x == other.x && y == other.y);
}

bool Point::operator!= (const Point& other) const
{
    return !(*this == other);
}

Point& Point::operator+= (const Point& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Point& Point::operator-= (const Point& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

const Point Point::operator+ (const Point& other) const
{
    return Point(*this) += other;
}

const Point Point::operator- (const Point& other) const
{
    return Point(*this) -= other;
}

float Point::operator% (const Point& other) const
{
    return std::sqrt(std::pow(x - other.x, 2) +
		     std::pow(y - other.y, 2));
}

// statics
int Point::refw = 1280;
int Point::refh = 1024;
