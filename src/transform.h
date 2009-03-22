/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// This function object is for transforming a set of vertices, the
// object is constructed using the translation and rotation used for
// the transformation.  It is designed to be used with the STL
// algorithms and can be used with STL function adapters

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <functional>

class Transform : public std::unary_function<Point, Point>
{
public:
    Transform(const Point& translation, const float& heading);
    virtual ~Transform();

    Point operator() (const Point& vertex) const;
    
private:
    Point translation;
    float heading;
};

#endif // TRANSFORM_H
