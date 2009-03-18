objects = main.o timer.o texture.o point.o world.o turret.o tank.o usertank.o

CXXFLAGS =  -g -Wall -ansi -pedantic `sdl-config --cflags`

game :	$(objects)
	g++ -g -lGL -lGLU -lpng `sdl-config --cflags --libs` -o game $(objects)

main.o : renderable.h collidable.h controllable.h world.h

timer.o : timer.h

texture.o : texture.h

point.o : point.h

world.o : renderable.h collidable.h controllable.h world.h point.h

turret.o : renderable.h collidable.h turret.h texture.h point.h

tank.o : renderable.h collidable.h turret.h texture.h point.h

usertank.o : renderable.h collidable.h controllable.h turret.h texture.h tank.h point.h

clean :
	rm game $(objects)