objects = main.o timer.o texture.o world.o

CXXFLAGS =  -g -Wall -ansi -pedantic `sdl-config --cflags`

game :	$(objects)
	g++ -g -lGL -lGLU -lpng `sdl-config --cflags --libs` -o game $(objects)

main.o : renderable.h collidable.h controllable.h world.h

timer.o : timer.h

texture.o : texture.h

world.o : renderable.h collidable.h controllable.h world.h

clean :
	rm game $(objects)