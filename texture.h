/* This class loads a texture from an image file, it can then create
 * an OpenGL texture for it.
 * Supported file types so far are:
 * PNG
 * Raw (untested)
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <memory>

#include <png.h>

#include "SDL.h"
#include "SDL_opengl.h"

// file types
enum texture_types {
    automatic,
    png,
    raw
};

class Texture 
{
public:
    // filename should be a path to a file
    // if the file type is set to automatic, the constructor will
    // attempt to find the type from the filename extension
    // width and height is required for some file types ie. raw for
    // PNG these values will be ignored (the values from the file will
    // be used)
    Texture(std::string filename, 
	    texture_types type, 
	    int width = 0, 
	    int height = 0);
    ~Texture();
    
    GLuint		getTexId();
    GLsizei		getWidth();
    GLsizei		getHeight();
    texture_types	getType();
    int			getBitDepth();
    // color type follows the PNG specification
    // only two colour types are supported at the moment:
    // 2: RGB
    // 6: RGBA
    int			getColourType();

    typedef std::auto_ptr<Texture> Ptr;
    
private:
    std::string		filename;
    texture_types	type;

    GLuint	tex_id;
    GLsizei	width;
    GLsizei	height;
    int		bit_depth;
    int		colour_type;
    GLubyte*	texels;
    
    void	discover_type();
    void	open_png();
    void	open_raw();
    void	clean_up();
    void	make_opengl_tex();
};

#endif // TEXTURE_H
