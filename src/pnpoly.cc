// pnpoly function finds if a point is in a polygon using the Jordan
// Curve Theorem. More info about this file is here:
// <http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html>
//
// This has been modified by me to use C++ standard library containers
// instead of arrays and my Point class
// 
// Copyright (c) 1970-2003, Wm. Randolph Franklin
// 
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
//    1. Redistributions of source code must retain the above
//    copyright notice, this list of conditions and the following
//    disclaimers.
//
//    2. Redistributions in binary form must reproduce the above
//    copyright notice in the documentation and/or other materials
//    provided with the distribution.
//
//    3. The name of W. Randolph Franklin may not be used to endorse
//    or promote products derived from this Software without specific
//    prior written permission.
//    
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <vector>

#include "pnpoly.h"
#include "point.h"

namespace wrf {

bool pnpoly(const PV& vertices, const Point& point)
{
    PV::const_iterator i, j;
    bool c = false;
    for (i = vertices.begin(), j = vertices.end()-1; 
	 i < vertices.end(); j = i++) {
	if ( ((i->getY()>point.getY()) != (j->getY()>point.getY())) &&
	     (point.getX() < (j->getX()-i->getX()) * (point.getY()-i->getY()) 
	      / (j->getY()-i->getY()) + i->getX()) )
	    c = !c;
    }
    return c;
}

}
