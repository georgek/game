/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of point

#include <cmath>
#include <iostream>

#include "SDL.h"
#include "SDL_opengl.h"

#include "point.h"

const double PI = 4.0*std::atan(1.0);

Point::Point (const float& x, const float& y) :
    x(x),
    y(y) 
{
    
}

Point::~Point () 
{
    
}

Point Point::screen (const float& x, const float& y)
{
    // convert from screen coords
    // convert width back
    float AR = Point::refw / Point::refh;
    float middle_width = SDL_GetVideoInfo()->current_h * AR;
    float real_x = x - (SDL_GetVideoInfo()->current_w - middle_width)/2;
    real_x = (static_cast<float>(Point::refh)
              / SDL_GetVideoInfo()->current_h) * real_x;
    // height convert is trivial
    float real_y = (static_cast<float>(Point::refh)
                    / SDL_GetVideoInfo()->current_h) * y;
    return Point (real_x, real_y);
}

Point Point::polarD (const float& radius, const float& angle)
{
    float angleR = (angle * PI/180) + PI/2;
    return Point (radius*std::cos(angleR), radius*std::sin(angleR));
}

Point Point::polarR (const float& radius, const float& angle)
{
    return Point (radius*std::cos(angle+PI/2), radius*std::sin(angle+PI/2));
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

float Point::operator/ (const Point& other) const
{
    float dx = other.x - x;
    float dy = other.y - y;
    float theta;
    // y=atan(x) gives only -PI/2 < y < PI/2 so the answer must be
    // adjusted, also dx = 0 is a special case since we cannot divide
    // by zero
    if (dx == 0) {
        theta = dy>0 ? PI : 0;
    }
    else if (dx>0) {
        theta = std::atan(dy/dx)+3*PI/2;
    }
    else { /* dx<0 */
        theta = std::atan(dy/dx)+PI/2;
    }

    return theta * 180/PI;
}

std::ostream& operator<< (std::ostream& out, const Point& point)
{
    out << "(";
    out << point.x;
    out << ", ";
    out << point.y;
    out << ")";
    
    return out;
}

// statics
int Point::refw = 1280;
int Point::refh = 1024;
