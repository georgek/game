/* This class loads a texture from an image file, it can then create
 * an OpenGL texture for it.
 * Supported file types so far are:
 * PNG
 * Raw (untested)
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <string>

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
    Texture(const std::string& filename, 
	    texture_types type, 
	    const int& width = 0, 
	    const int& height = 0);
    ~Texture();
    
    GLuint		getTexId() const;
    GLsizei		getWidth() const;
    GLsizei		getHeight() const;
    texture_types	getType() const;
    int			getBitDepth() const;
    // color type follows the PNG specification
    // only two colour types are supported at the moment:
    // 2: RGB
    // 6: RGBA
    int			getColourType() const;

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
