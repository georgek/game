/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

#include <iostream>

#include <cstdlib>
#include <unistd.h>

int main (int argc, char* argv[]) 
{
    // parse arguments
    int width = 640;
    int height = 480;

    // do not let getopt print errors to stderr
    opterr = 0;
    
    int c;
    while ((c = getopt(argc, argv, "w:h:")) != -1) {
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
    
    std::cout << "Width = " << width << " Height = " << height << std::endl;
    
    for (int i = optind; i < argc; ++i) {
	std::cout << "Non-option argument: " << argv[i] << std::endl;
    }

    // end of argument parsing

    std::cout << "Lul it started." << std::endl;
    
    
    return 0;
    
}
