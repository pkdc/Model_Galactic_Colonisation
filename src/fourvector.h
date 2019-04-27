#pragma once

#include <fstream>

class fourvector {

public:
  // Constructors:
  // Default constructor
  fourvector();

  // Cartesian constructor
  fourvector(double, double, double);

  fourvector(double, double, double, double);

  // Access method for coordinates
  double get_x() const;

  double get_y() const;

  double get_z() const;

  double get_t() const;

  // Access method for square of spatial coordinates
  double square() const;

  // Method to modify coordinates
  void set_x(double);

  void set_y(double);

  void set_z(double);

  void set_t(double);

  // Method to print out contents onto screen
  void print() const;

  // Method to print out spatial coordinates onto files
  void print(std::ofstream&) const;

  // Method to modify the spatial coords
  void mod_coord(double, double, double);
    
    
private:
    
  double x, y, z, t;

};
