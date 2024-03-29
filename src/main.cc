/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include <cstdlib>
#include <unistd.h>

#include "SDL.h"
#include "SDL_opengl.h"

#include "cursor.h"
#include "pnpoly.h"
#include "point.h"
#include "timer.h"
#include "transform.h"
#include "world.h"

// turns the fps cap on or off
const bool FPS_CAP = true;

bool init(const int& width, const int& height);
bool initGL(const int& width, const int& height);
void clean_up();

int main (int argc, char* argv[]) 
{
    // default args
    int width = 640;
    int height = 480;
    int fps = 90;

    // do not let getopt print errors to stderr
    opterr = 0;
    
    char c;
    while ((c = getopt(argc, argv, "w:h:c:")) != -1) {
	switch (c) {
	case 'w':
	    if (std::atoi(optarg) != 0) {
		width = std::atoi(optarg);
	    }
	    else {
		std::cerr << "Invalid width: " << optarg << std::endl;
	    }
	    break;
	    
	case 'h':
	    if (std::atoi(optarg) != 0) {
		height = std::atoi(optarg);
	    }
	    else {
		std::cerr << "Invalid height: " << optarg << std::endl;
	    }
	    break;

	case 'c':
	    if (std::atoi(optarg) != 0) {
		fps = std::atoi(optarg);
	    }
	    else {
		std::cerr << "Invalid fps: " << optarg << std::endl;
	    }
	    break;
	    
	case '?':
	    if (optopt == 'w' || optopt == 'h') {
		std::cerr << "Option -" << static_cast<char>(optopt) 
			  << " requires an argument." << std::endl;
	    }
	    else {
		std::cerr << "Unknown option -" << static_cast<char>(optopt) 
			  << std::endl;
	    }
	    break;
	    
	default:
	    break;
	}
    }

    // get worldfile
    std::string aworldfile;
    if (optind < argc) {
	aworldfile = argv[optind++];
	std::cout << "Worldfile: " << aworldfile << std::endl;
    }

    // print out remaining redundant arguments
    for (int i = optind; i < argc; ++i) {
	std::cout << "Non-option argument: " << argv[i] << std::endl;
    }

    // end of argument parsing

    // initialise random seed
    std::srand(std::time(0));

    // initialise SDL and OpenGL
    if (!init(width, height)) {
	return 1;
    }

    // make a cursor
    Cursor cursor ("textures/crosshair.png", 10);

    // make world
    std::unique_ptr<World> world;
    try{
	world = std::unique_ptr<World> (new World (aworldfile));
    }
    // catch exception from constructor
    catch (const std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
	// exit
	clean_up();
	return 1;
    }

    // prepare for main loop
    // SDL event structure
    SDL_Event event;
    // timer for fpscap
    Timer fpsCap;
    // timer for fps count
    Timer fpsCount;
    int frames = 0;
    // start fps timer
    fpsCount.start();

    // main loop
    bool quit = false;
    while (!quit) {
        // check game status
        if (world->getGameStatus()) {
            switch (world->getGameStatus()) {
            case 1:
                // success
                std::cout << "You win!" << std::endl;
                break;
            case 2:
                // fail
                std::cout << "Fail. Game over." << std::endl;
                break;
            }
            break;
        }
	// start fpscap timer
	fpsCap.start();
	
	// process any events in queue
	while (SDL_PollEvent(&event)) {
	    if (event.type == SDL_QUIT
		|| (event.type == SDL_KEYUP
		    && event.key.keysym.sym == SDLK_ESCAPE)
		) {
		quit = true;
	    }
	    else {
		// send event to world
		world->update(event);

                // send event to cursor
                cursor.update(event);

                // clean up user events...  I cried when I wrote this,
                // it's terrible, in future I will not use SDL events
                // for this sort of thing
                if (event.type == SDL_USEREVENT) {
                    switch (event.user.code) {
                    case 1:     // damage, delete int and Point
                        if (event.user.data1) {
                            delete static_cast<int*>(event.user.data1);
                        }
                        if (event.user.data2) {
                            delete static_cast<Point*>(event.user.data2);
                        }
                        break;
                    case 2:     // user position, delete Point
                        if (event.user.data1) {
                            delete static_cast<Point*>(event.user.data1);
                        }
                        break;
                    case 3:     // enemy dead, delete int
                        if (event.user.data1) {
                            delete static_cast<int*>(event.user.data1);
                        }
                        break;
                    }
                }
	    }
	}
	// update objects
	world->update();

	// draw world
	world->draw();

        // draw cursor
        cursor.draw();

        // swap buffers
        SDL_GL_SwapBuffers();

	// cap frame rate
	if (FPS_CAP && fpsCap.get_ticks() < 1000 / fps) {
	    SDL_Delay ((1000 / fps) - fpsCap.get_ticks());
	}

	++frames;

	// show actual frame rate
	int secs = 5;
	if (fpsCount.get_ticks() > secs*1000) {
	    std::cout << frames << " frames in " << secs << " seconds ("
		      << (frames*1000.0)/fpsCount.get_ticks() << "fps)" 
		      << std::endl;
	    frames = 0;
	    fpsCount.start();
	}

	// show any error
	GLenum errcode;
	if ((errcode = glGetError()) != GL_NO_ERROR) {
	    std::cout << "OpenGL error: " << gluErrorString(errcode) 
		      << std::endl;
	}
    }

    // shut down SDL
    clean_up();

    // exit
    return 0;
}

// initialises SDL
bool init(const int& width, const int& height)
{
    // initialise SDL timer and video subsystems
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
	return false;
    }

    std::cout << "SDL initialised." << std::endl;

    // some OpenGL settings
    // make sure double buffering is enabled
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // create window
    if (SDL_SetVideoMode(width, height, 32, SDL_OPENGL) == NULL) {
	return false;
    }

    // initialise OpenGL
    if (!initGL(width, height)) {
	return false;
    }

    // set window title
    SDL_WM_SetCaption("CMPC2G04 Game", NULL);

    // hide mouse cursor
    SDL_ShowCursor(0);

    return true;
}

// initialise OpenGL
bool initGL(const int& width, const int& height)
{
    // set clear color, red so it stands out (it should never be seen)
    glClearColor(0.4, 0, 0, 0);
    
    // set viewport
    glViewport(0, 0, width, height);

    // set projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-width/2, width/2, -height/2, height/2);

    // initialise modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_COLOR_MATERIAL);

    // turn up texture quality
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    // if there was any errors
    if(glGetError() != GL_NO_ERROR)
    {
        return false;
    }

    // everything initialised
    std::cout << "OpenGL initialised." << std::endl;

    // print some numbers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    return true;
}

void clean_up()
{
    std::cout << "Shutting down SDL." << std::endl;
    SDL_Quit();
}
