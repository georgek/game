/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// Interface for Controllable objects. Objects must implement this
// interface if they are to be controlled by the user with SDL events

#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

class Controllable 
{
public:
    virtual void update() = 0;
};

#endif // CONTROLLABLE_H
