#include "fourvector.h"
#include "Planet.h"
#include <math.h>


// Default constructor
planet::planet() : fourvector(), developed(false), retired(false), col_count(0) { }

// Constructor specifying the position
planet::planet(double xc, double yc, double zc) : fourvector(xc, yc, zc), developed(false), retired(false), col_count(0) { }

// Constructor specifying the time and position
planet::planet(double tc, double xc, double yc, double zc) : fourvector(tc, xc, yc, zc), developed(false), retired(false), col_count(0) { }

// Access method for status
bool planet::get_dev() const{ return developed;}

bool planet::get_ret() const{ return retired;}

int planet::get_col_count() const { return col_count;};

// Modifiers
void planet::set_ret(bool ret) { retired = ret;}

void planet::set_dev(bool dev) { developed = dev;}

void planet::inc_col_count() { col_count++;}

// acquire distance between two planets
double planet::get_dist(const planet& p) const {
  return sqrt((get_x()-p.get_x())*(get_x()-p.get_x())+(get_y()-p.get_y())*(get_y()-p.get_y())+(get_z()-p.get_z())*(get_z()-p.get_z()));
}

