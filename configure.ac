# initialise autoconf
AC_INIT
AC_CONFIG_SRCDIR([README])

# name of program and version
AM_INIT_AUTOMAKE(game,0.1)

# tools needed: make and a C++ compiler
AC_PROG_MAKE_SET
AC_PROG_CXX

# include libraries using pkginfo
PKG_CHECK_MODULES(DEPS, libpng >= 1.2 libxml++-2.6 >= 2.6)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

# Use the macro SDL provides to check the installed version of the SDL
# development environment.  Abort the configuration process if the
# minimum version we require isn't available.
SDL_VERSION=1.0.8
AM_PATH_SDL($SDL_VERSION,
            :,
	    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
# Add the SDL preprocessor flags and libraries to the build process
DEPS_CFLAGS="$DEPS_CFLAGS $SDL_CFLAGS"
DEPS_LIBS="$DEPS_LIBS $SDL_LIBS"

# add OpenGL (could add check for OpenGL here)
DEPS_LIBS="$DEPS_LIBS -lGL -lGLU"

# output Makefiles
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
