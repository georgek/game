/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// this class represents a point in the world at the reference
// resolution, the coordinates can be converted to display coordinates
// for the actual resolution

#ifndef POINT_H
#define POINT_H

class Point 
{
public:
    Point (const float& x = 0, const float& y = 0);
    ~Point ();
    
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
    // this operator has special behaviour, it calculates the
    // euclidean distance between the points
    float operator% (const Point& other) const;
    
    // reference resolution
    static int refw;
    static int refh;

private:
    // coordinates
    float x, y;
};

#endif // POINT_H
