objects = main.o

CXXFLAGS =  -g -Wall -ansi -pedantic `sdl-config --cflags`

game :	$(objects)
	g++ -g -lGL -lGLU -lpng `sdl-config --cflags --libs` -o game $(objects)

main.o : 

texture.o : texture.h

clean :
	rm game $(objects)