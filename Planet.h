//#ifndef PLANET_H	// Prevents the class from being re-defined
//#define PLANET_H
#pragma once

#include "fourvector.h"
#include <map>

class planet : public fourvector {

public:
    // Default constructor
    planet();
    
    // Constructor specifying the position
    planet(double, double, double);
    
    // Constructor specifying the time and position
    planet(double, double, double, double);

    // Access method for status
    bool get_dev() const;

    bool get_ret() const;
    
    int get_col_count() const;

    // Modifiers
    void set_ret(bool);

    void set_dev(bool);
    
    void inc_col_count();

    // acquire distance between two planets
    double get_dist(const planet&) const;
    
    // map of nearby planets
    std::map<int, double> nearby_planets;
    
private:
    // N.B. Data members include x, y, z, t inherited from fourvector specifying the positions and time of completing development
    
    // Status of development: Ready to send ships
    // retirement: Have all of its neighbours been colonised?
    bool developed, retired;
    
    // count the number of colonisation undertaken from this planet
    int col_count;

};

#endif // PLANET_H
