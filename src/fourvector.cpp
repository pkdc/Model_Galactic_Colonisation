#include <iostream>
#include <fstream>
#include "fourvector.h"

// Constructors:
// Default constructor
fourvector::fourvector(): x(0.0), y(0.0), z(0.0), t(0.0) {}

// Cartesian constructor
fourvector::fourvector(double xc, double yc, double zc): x(xc), y(yc), z(zc) {}

fourvector::fourvector(double tc, double xc, double yc, double zc) : t(tc), x(xc), y(yc), z(zc) {}

// Access method for coordinates
double fourvector::get_x() const {
  return x;
}

double fourvector::get_y() const {
  return y;
}

double fourvector::get_z() const {
  return z;
}

// Access method for t
double fourvector::get_t() const {
  return t;
}

// Access method for square of spatial coordinates
double fourvector::square() const{
  return (x*x)+(y*y)+(z*z);	// summing the square of each coordinate
}

// Method to modify coordinates
void fourvector::set_x(double valuex) {
  x = valuex;		// changing the x coord to the valuex inputted
}

void fourvector::set_y(double valuey) {
  y = valuey;		
}

void fourvector::set_z(double valuez) {
  z = valuez;		
}

void fourvector::set_t(double valuet) {
  t = valuet;		
}

// Method to print out contents onto screen
void fourvector::print() const {
  std::cout << x << '\t' << y << '\t' << z << std::endl;
}

// Method to print out spatial coordinates onto files
void fourvector::print(std::ofstream& fout) const {
  fout << x << '\t' << y << '\t' << z << std::endl;
}

// Method to modify the spatial coords
void fourvector::mod_coord(double xc, double yc, double zc) {
  set_x(xc);
  set_y(yc);
  set_z(zc);
}
