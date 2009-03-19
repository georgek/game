bin_PROGRAMS = game

game_SOURCES = main.cc collidable.h controllable.h point.h point.cc \
	renderable.h tank.h tank.cc texture.h texture.cc timer.h timer.cc \
	turret.h turret.cc usertank.h usertank.cc world.h world.cc

EXTRA_DIST = tools/shared_ptr.h

AM_CXXFLAGS = -g -Wall -ansi -pedantic

INCLUDES = $(DEPS_CFLAGS)
LIBS = $(DEPS_LIBS)
