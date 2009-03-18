// implementation of Texture class
// George Kettleborough <g.kettleborough@uea.ac.uk>
// 
// PNG reading code is based on:
// <http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures>
// which is available under GFDL license
//
// Raw reading code is from SDL's lecture notes

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>

#include <png.h>

#include "SDL.h"
#include "SDL_opengl.h"

#include "texture.h"

Texture::Texture(const std::string& filename, texture_types type, 
		 const int& width, const int& height) :
    filename (filename),
    type (type),
    tex_id (0),
    width (width),
    height (height),
    bit_depth (0),
    colour_type (0),
    texels (0)
{
    // set type automatically
    if (this->type == automatic) {
	discover_type();
    }

    // open file
    switch (this->type) {
    case png:
	open_png();
	break;
    case raw:
	open_raw();
	break;
    default:
	// fail
	break;
    }

    // check if image was opened
    if (texels) {
	// it was
	make_opengl_tex();
    }
}

Texture::~Texture() 
{
    // destroy image data
    clean_up();
}

GLuint Texture::getTexId() const
{
    return tex_id;
}

GLsizei Texture::getWidth() const
{
    return width;
}

GLsizei Texture::getHeight() const
{
    return height;
}

texture_types Texture::getType() const
{
    return type;
}

int Texture::getBitDepth() const
{
    return bit_depth;
}

int Texture::getColourType() const
{
    return colour_type;
}

void Texture::discover_type() 
{
    // discover the type using the filename
    std::string::size_type idx = filename.rfind('.');
    if (idx != std::string::npos) {
    	// match the extension
    	std::string extension(filename.substr(idx+1));
    	// convert string to upper case
    	std::transform(extension.begin(), 
    		       extension.end(),
    		       extension.begin(),
    		       ::toupper);
    	if (extension == "PNG") {
    	    type = png;
    	    return;
    	}
    	if (extension == "RAW") {
    	    type = raw;
    	    return;
    	}
    }
    // set to raw if there is no extension or it is unmatched
    type = raw;
}

void Texture::open_png() 
{
    // to contain PNG header data
    png_byte header[8];
    
    // open file, the C way... has to be for libpng
    FILE* file = std::fopen(filename.c_str(), "rb");
    if (!file) {
	// file not opened
	std::cerr << "Texture: " << filename << " does not exist." 
		  << std::endl;
	return;
    }
    
    // read header
    fread(header, 1, 8, file);

    // check header
    if (png_sig_cmp(header, 0, 8)) {
	// not a png
	std::cerr << "Texture: " << filename << " is not a PNG." 
		  << std::endl;
	fclose(file);
	return;
    }

    // create structs for libpng
    // image struct
    png_structp png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING,
						  0, 0, 0);
    if (!png_ptr) {
	// error creating struct
	fclose(file);
	return;
    }
    
    // info structs
    png_infop info_ptr = png_create_info_struct (png_ptr);
    if (!info_ptr) {
	png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
	fclose(file);
	return;
    }
    png_infop end_info = png_create_info_struct (png_ptr);
    if (!info_ptr) {
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
	fclose(file);
	return;
    }

    // start reading png
    png_init_io(png_ptr, file);
    // already read the header
    png_set_sig_bytes(png_ptr, 8);
    // read info data
    png_read_info(png_ptr, info_ptr);
    
    // get PNG info
    int ibit_depth, icolour_type;
    png_uint_32 iwidth, iheight;
    png_get_IHDR(png_ptr, info_ptr, &iwidth, &iheight, &ibit_depth, 
		 &icolour_type, 0, 0, 0);
    // set this object's info
    width = iwidth;
    height = iheight;
    bit_depth = ibit_depth;
    colour_type = icolour_type;
    
    // update info struct
    png_read_update_info(png_ptr, info_ptr);
    
    // read image data into array
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    // allocate memory
    texels = new png_byte[rowbytes * height];
    if (!texels) {
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	fclose(file);
	return;
    }
    // array of row pointers
    png_bytep* row_pointers = new png_bytep[height];
    if (!row_pointers) {
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] texels;
	texels = 0;
	fclose(file);
	return;
    }
    // set row pointers
    for (int i = 0; i < height; ++i) {
	row_pointers[height - 1 - i] = texels + i * rowbytes;
    }

    // read png into row pointers
    png_read_image(png_ptr, row_pointers);
    
    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete[] row_pointers;
    row_pointers = 0;
    fclose(file);
}

void Texture::open_raw() 
{
    // open raw file
    FILE* file = std::fopen(filename.c_str(), "rb");
    if(!file) {
	std::cerr << "Texture: " << filename << " does not exist." 
		  << std::endl;
	return;
    }
    texels = new unsigned char[width*height*4];
    for(int i=0; i<height; i++) {
	for(int j=0; j<width; j++) {
	    texels[i*width*4+j*4] = getc(file);
	    texels[i*width*4+j*4+1] = getc(file);
	    texels[i*width*4+j*4+2] = getc(file);
	}
    }
    fclose(file);
    // RGBA colour type
    colour_type = 6;
}

void Texture::clean_up() 
{
    // destroy texel data
    if (texels) {
	delete[] texels;
	texels = 0;
    }
}

void Texture::make_opengl_tex() 
{
    // make OpenGL texture from texels
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    switch (colour_type) {
    case 2:
	// RGB
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0, 
		     GL_RGB, GL_UNSIGNED_BYTE, texels);
	break;
    case 6:
	// RGBA
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0, 
		     GL_RGBA, GL_UNSIGNED_BYTE, texels);
	break;
    default:
	break;
    }    // ***TODO*** Add cases for other PNG colour types

    // set some parameters, these could be options in the next version
    // set texture wrap to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // linear scaling, could support mipmapping in the next version
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
