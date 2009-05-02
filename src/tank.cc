/* CMPC2G04 Game Coursework
 * George Kettleborough <g.kettleborough@uea.ac.uk>
 * UEA registration number: 3316173
 */

// implementation of Tank class

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include <libxml++/libxml++.h>
#include <libxml++/parsers/textreader.h>

#include "bullet.h"
#include "pnpoly.h"
#include "tank.h"
#include "transform.h"
#include "world.h"

const double PI = 4.0*std::atan(1.0);

Tank::Tank(World* world, const int& layer, const Turret::Ptr& turret, 
	   const std::string& texturename, 
	   const Point& init_pos,
	   const int& engine_force, const int& mass, const int& rpm) :
    world(world),
    turret(turret),
    texture(texturename, Texture::automatic),
    worldpos(init_pos),
    radius(83),
    top_speed(180),
    rpm(rpm),
    curr_velocity(0),
    target_velocity(0),
    curr_rpm(0),
    heading(0),
    xf(0),
    yf(1),
    mouse_x(0),
    mouse_y(0),
    turret_rpm(25),
    curr_turret_rpm(0),
    layer(layer)
{
    // calculate vertices
    int w = texture.getWidth();
    int h = texture.getHeight();
    vertices.push_back(Point(w/2,h/2));
    vertices.push_back(Point(w/2,-h/2));
    vertices.push_back(Point(-w/2,-h/2));
    vertices.push_back(Point(-w/2,h/2));

    makeDrawingList();
}

Tank::Tank(World* world, const int& layer, const Turret::Ptr& turret,
	   const Point& init_pos,
	   const std::string& inputfile) :
    world(world),
    turret(turret),
    worldpos(init_pos),
    curr_velocity(0),
    target_velocity(0),
    curr_rpm(0),
    heading(0),
    xf(0),
    yf(1),
    mouse_x(0),
    mouse_y(0),
    turret_rpm(25),
    curr_turret_rpm(0),
    layer(layer) 
{
    // parse input file
    try {
	parseInputFile(inputfile);
    }
    catch (const std::exception& e) {
	std::cout << "XML Exception in file " << inputfile 
		  << ": " << e.what() << std::endl;
    }

    makeDrawingList();
}

Tank::~Tank() 
{
    
}

void Tank::draw() 
{
    // find screen position
    Point screenpos =  worldpos - world->getOffset();

    // (re)start timer
    timer.start();

    glPushMatrix();

    // move to correct positon on screen
    glTranslatef(screenpos.getDispX(),screenpos.getDispY(),0);
    // rotate to face correct direction
    glRotatef(heading, 0, 0, 1);
    
    // call list
    glCallList(drawing_list);
    
    glPopMatrix();
}

bool Tank::isCollidedR(const Point& centre,
		       const float& radius) const 
{
    // distance between points
    if (worldpos % centre < (this->radius + radius)) {
	// collided
	return true;
    }
    return false;
}

bool Tank::isCollidedV(const std::vector<Point>& vertices) const 
{
    // work out current vertices
    std::vector<Point> curr_vertices;
    // transform the vertices using the transform functor
    std::transform(this->vertices.begin(), this->vertices.end(),
		   std::back_inserter(curr_vertices),
		   Transform(worldpos, heading));

    // check that none of the other vertices are in this polygon
    std::vector<Point>::const_iterator pos;
    for (pos = vertices.begin(); pos < vertices.end(); ++pos) {
	if (wrf::pnpoly(curr_vertices, *pos)) {
	    return true;
	}
    }
    // now check that none of these vertices are in the other polygon
    for (pos = curr_vertices.begin(); pos < curr_vertices.end(); ++pos) {
	if (wrf::pnpoly(vertices, *pos)) {
	    return true;
	}
    }
    return false;
}

Point Tank::getWorldPos() const
{
    return worldpos;
}

void Tank::makeDrawingList() 
{
    // calculate texture vertices
    std::vector<Point> texVert;
    int w = texture.getWidth();
    int h = texture.getHeight();
    texVert.push_back(Point(w/2,h/2));
    texVert.push_back(Point(w/2,-h/2));
    texVert.push_back(Point(-w/2,-h/2));
    texVert.push_back(Point(-w/2,h/2));

    // make new drawing list
    drawing_list = glGenLists(1);
    glNewList(drawing_list, GL_COMPILE);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glBindTexture(GL_TEXTURE_2D, texture.getTexId());

    glBegin(GL_QUADS);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(texVert[0].getDispX(),texVert[0].getDispY(),0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(texVert[1].getDispX(),texVert[1].getDispY(),0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(texVert[2].getDispX(),texVert[2].getDispY(),0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(texVert[3].getDispX(),texVert[3].getDispY(),0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    
    glEndList();
}

void Tank::parseInputFile(const std::string& filename) 
{
    std::stringstream s;
    // open file
    xmlpp::TextReader inputfile(filename.c_str());
    // parse it
    while (inputfile.read()) {
	if (inputfile.get_node_type() == xmlpp::TextReader::Element) {
	    if (inputfile.get_name() == "tank") {
		// get attributes
		inputfile.move_to_first_attribute();
		s << inputfile.get_value();
		s >> accn;
		s.str(""); s.clear();
		inputfile.move_to_next_attribute();
		s << inputfile.get_value();
		s >> dccn;
		s.str(""); s.clear();
		inputfile.move_to_next_attribute();
		s << inputfile.get_value();
		s >> top_speed;
		s.str(""); s.clear();
		inputfile.move_to_next_attribute();
		s << inputfile.get_value();
		s >> rpm;
		s.str(""); s.clear();
	    }
	    // switch for element type
	    else if (inputfile.get_name() == "texture") {
		// set texture
		inputfile.move_to_first_attribute();
		texture = Texture(inputfile.get_value().raw(), 
				  Texture::automatic);
	    }
	    else if (inputfile.get_name() == "vertices") {
		// read each vertex
		int x, y;
		while (inputfile.read()) {
		    if (inputfile.get_node_type() == 
			xmlpp::TextReader::EndElement) {
			// end of vertex list
			break;
		    }
		    else if (inputfile.get_name() == "v") {
			inputfile.move_to_first_attribute();
			s << inputfile.get_value();
			s >> x;
			s.str(""); s.clear();
			inputfile.move_to_next_attribute();
			s << inputfile.get_value();
			s >> y;
			s.str(""); s.clear();
			// insert into vector
			vertices.push_back(Point(x,y));
		    }
		}
	    }
	    else if (inputfile.get_name() == "radius") {
		// store radius
		inputfile.move_to_first_attribute();
		s << inputfile.get_value();
		s >> radius;
		s.str(""); s.clear();
	    }
	}
    }
}

void Tank::fire()
{
    // find point where mouse is
    int vh = SDL_GetVideoInfo()->current_h;
    Point destination = Point::screen (mouse_x, vh - mouse_y);

    destination = destination + world->getOffset();
    destination.setX(destination.getX() - Point::refh/2);
    destination.setY(destination.getY() - Point::refh/2);

    // adjust for real turret position
    float radius = worldpos % destination;

    destination = worldpos +
        Point::polarD (radius, turret->getTurretAngle());
    
    // fire a bullet
    Bullet::Ptr bullet (new Bullet (world, "textures/bullet.png",
                                    worldpos, destination,
                                    115,
                                    1500, 100));
    World::RendMap::iterator bullpos = world->addRenderable (bullet, 3);
    bullet->run(bullpos);
}

void Tank::move()
{
    float ticks = timer.get_ticks();
    // apply acceleration
    if (curr_velocity < target_velocity && target_velocity > 0) {
	// accelerate forwards
	curr_velocity += accn*ticks/1000.0;
	if (curr_velocity > target_velocity) curr_velocity = target_velocity;
    }
    else if (curr_velocity > target_velocity && target_velocity < 0) {
	// accelerate backwards
	curr_velocity -= accn*ticks/1000.0;
	if (curr_velocity < target_velocity) curr_velocity = target_velocity;
    }
    else if (curr_velocity > target_velocity && target_velocity == 0) {
	// deccelerate
	curr_velocity -= dccn*ticks/1000.0;
	if (curr_velocity < target_velocity) curr_velocity = target_velocity;
    }
    else if (curr_velocity < target_velocity && target_velocity == 0) {
	// deccelerate
	curr_velocity += dccn*ticks/1000.0;
	if (curr_velocity > target_velocity) curr_velocity = target_velocity;
    }

    // move according to current velocities
    Point new_worldpos;
    new_worldpos.setX(worldpos.getX() + curr_velocity*xf*ticks/1000.0);
    new_worldpos.setY(worldpos.getY() + curr_velocity*yf*ticks/1000.0);

    if (new_worldpos == worldpos) {
	// nothing do so return
	return;
    }

    // check for collisions, first check turret
    if (turret->isCollided(new_worldpos, 0)) {
	return;
    }

    // work out new vertices
    std::vector<Point> new_vertices;
    // transform the vertices using the transform functor
    std::transform(vertices.begin(), vertices.end(), // source
                   std::back_inserter(new_vertices), // destination
                   Transform(new_worldpos, heading));

    // now check collisions with tank
    if (world->isCollided(new_worldpos, radius, new_vertices, layer, this)) {
        // it has collided
        curr_velocity = 0;
        return;
    }

    // no collision, set new world coords
    worldpos = new_worldpos;
}

void Tank::rotate() 
{
    // rotate tank according to current rpm
    float new_heading = heading + 
	(curr_rpm*timer.get_ticks())/(60000.0f/360.0f);
    if (new_heading > 360) new_heading = new_heading - 360;
    if (new_heading < 0) new_heading = 360 + new_heading;

    if (new_heading == heading) {
	// nothing to do
	return;
    }

    // work out new vertices
    std::vector<Point> new_vertices;
    // transform the vertices using the transform functor
    std::transform(vertices.begin(), vertices.end(), // source
                   std::back_inserter(new_vertices), // destination
                   Transform(worldpos, new_heading));

    // check for collisions
    if (world->isCollided(worldpos, radius, new_vertices, layer, this)) {
        // it has collided
        return;
    }

    // no collision, set new rotation
    heading = new_heading;

    if (curr_rpm) {
	// calculate new x and y factors for movement
	xf = std::cos((heading+90) * PI/180.0);
	yf = std::sin((heading+90) * PI/180.0);
    }
}

void Tank::rotate_turret()
{
    // rotate turret
    // get current resolution
    int vw = SDL_GetVideoInfo()->current_w;
    int vh = SDL_GetVideoInfo()->current_h;
    
    // convert opengl coords to sdl coords
    Point screenpos = worldpos - world->getOffset();
    float sdlxOffset = screenpos.getDispX() + vw/2;
    float sdlyOffset = vh/2 - screenpos.getDispY();
    
    // angle between tank and mouse
    float dx = sdlxOffset - mouse_x;
    float dy = sdlyOffset - mouse_y;
    float theta;

    // y=atan(x) gives only -PI/2 < y < PI/2 so the answer must be
    // adjusted, also dx = 0 is a special case since we cannot divide
    // by zero
    if (dx == 0) {
        theta = dy>0 ? 0 : PI;
    }
    else if (dx>0) {
        theta = -std::atan(dy/dx)+PI/2;
    }
    else { /* dx<0 */
        theta = -std::atan(dy/dx)+3*PI/2;
    }

    // convert to degrees
    theta = theta*180/PI;

    if (theta == turret->getTurretAngle()) {
        // nothing to do
        return;
    }

    // find out how far the turret can move in one tick
    float tick_angle = (turret_rpm*timer.get_ticks())/(60000.0f/360.0f);

    // difference between current turret direction and mouse direction
    float diff = theta - turret->getTurretAngle();

    // now set turret rpm
    if (std::fabs(diff) < tick_angle) {
	curr_turret_rpm = 0;
	// check for collisions
	if (!turret->isCollided(worldpos, theta-turret->getTurretAngle())) {
	    // not collided, set angle exactly to mouse
	    turret->setTurretAngle(theta);
	}
	// return because there is nothing else to do
	return;
    }
    else if (diff >= -180 && diff < 180) {
	curr_turret_rpm = (diff > 0 ? turret_rpm : -turret_rpm) + curr_rpm;
    }
    else {
	curr_turret_rpm = (diff > 0 ? -turret_rpm : turret_rpm) + curr_rpm;
    }
    
    // find change in angle based on current rpm
    float dtheta = (curr_turret_rpm*timer.get_ticks())/(60000.0f/360.0f);

    // check for collisions
    if (turret->isCollided(worldpos, dtheta)) {
	// collided
	return;
    }
    // not collided, set new turret angle
    turret->incTurretAngle(dtheta);
}
