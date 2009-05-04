/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// this draws a custom cursor where the mouse is positioned

#ifndef CURSOR_H
#define CURSOR_H

#include <string>

#include "SDL.h"
#include "SDL_opengl.h"

#include "controllable.h"
#include "point.h"
#include "renderable.h"
#include "texture.h"
#include "timer.h"

class Cursor : public Controllable, public Renderable
{
public:
    Cursor (const std::string& texture_name, const int& rpm = 45);
    virtual ~Cursor();
    
    virtual void update();
    virtual void update(SDL_Event& event);

    virtual void draw();
    
private:
    Texture texture;
    Point mouse_pos;
    GLuint drawing_list;
    int rpm;
    float heading;

    Timer timer;
};

#endif // CURSOR_H
