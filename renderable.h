/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Interface for Renderable objects. Objects must implement this
// interface if they are to be drawn on the screen with OpenGL

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "tools/shared_ptr.h"

class Renderable
{
public:
    virtual void draw() = 0;

    // convenience typedef
    typedef gk::shared_ptr<Renderable> Ptr;
};

#endif // RENDERABLE_H
