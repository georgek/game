/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Implementation of Transform functor

#include <cmath>

#include "point.h"
#include "transform.h"

const double PI = 4.0*std::atan(1.0);

Transform::Transform (const Point& translation, const float& heading) :
    translation(translation),
    heading(heading)
{
    
}

Transform::~Transform ()
{
    
}

Point Transform::operator() (const Point& vertex) const
{
    Point new_vertex;
    
    // rotate
    new_vertex.setX(vertex.getX() * std::cos((heading) * PI/180.0)
		    - vertex.getY() * std::sin((heading) * PI/180.0));
    new_vertex.setY(vertex.getX() * std::sin((heading) * PI/180.0)
		    + vertex.getY() * std::cos((heading) * PI/180.0));

    // translate them
    new_vertex += translation;

    return new_vertex;
}
