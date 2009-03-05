objects = main.o

CXXFLAGS =  -g -Wall -ansi -pedantic `sdl-config --cflags`

tut36 :	$(objects)
		g++ -g -lGL -lGLU `sdl-config --cflags --libs` -o game $(objects)

main.o : 

clean :
	rm game $(objects)