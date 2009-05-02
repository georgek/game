/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// this class represents a point in the world at the reference
// resolution, the coordinates can be converted to display coordinates
// for the actual resolution

#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point 
{
public:
    Point (const float& x = 0, const float& y = 0);
    ~Point ();

    // named constructors
    static Point screen (const float& x = 0, const float& y = 0);
    static Point polarD (const float& radius = 0, const float& angle = 0);
    static Point polarR (const float& radius = 0, const float& angle = 0);
    
    float getX() const;
    float getY() const;

    float getDispX() const;
    float getDispY() const;

    void setX (const float& x);
    void setY (const float& y);

    // operators
    bool operator== (const Point& other) const;
    bool operator!= (const Point& other) const;
    Point& operator+= (const Point& rhs);
    Point& operator-= (const Point& rhs);
    const Point operator+ (const Point& other) const;
    const Point operator- (const Point& other) const;
    // these operators have special behaviour
    // calculates the euclidean distance between the points
    float operator% (const Point& other) const;
    // calculates angle of line from this point to other point
    float operator/ (const Point& other) const;

    // for printing to standard output
    friend std::ostream& operator<< (std::ostream& out, 
                                     const Point& point);

    // reference resolution
    static int refw;
    static int refh;

private:
    // coordinates
    float x, y;
};

#endif // POINT_H
