bin_PROGRAMS = game

game_SOURCES = main.cc collidable.h controllable.h point.h point.cc	\
	renderable.h tank.h tank.cc texture.h texture.cc timer.h	\
	timer.cc turret.h turret.cc usertank.h usertank.cc world.h	\
	world.cc pnpoly.h pnpoly.cc transform.h transform.cc		\
	friendlytank.h friendlytank.cc aitank.h aitank.cc bullet.h	\
	bullet.cc sprite.h sprite.cc cursor.h cursor.cc healthbar.h	\
	healthbar.cc ornament.h ornament.cc enemytank.h enemytank.cc

EXTRA_DIST = tools/shared_ptr.h

AM_CXXFLAGS = -g -Wall -ansi -pedantic -std=c++11

INCLUDES = $(DEPS_CFLAGS)
LIBS = $(DEPS_LIBS)
