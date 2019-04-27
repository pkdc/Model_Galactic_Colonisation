//
//  Spaceship.cpp
//  Model_Galactic_Colonisation
//
//  Created by David Choy on 06/01/2015.
//  Copyright (c) 2015 pkdc. All rights reserved.
//

//#include "Spaceship.h"
//#include "fourvector.h"
#pragma once

// default Constructor
spaceship::spaceship() : v(0.0), safe_dur(0.0) {}

spaceship::spaceship(double vc, double sd) : v(vc), safe_dur(sd) {}

// access methods
double spaceship::get_v() const { return v;}

double spaceship::get_safe_dur() const { return safe_dur;}

// modifiers
void spaceship::set_v(double vc) {
    
    // ensure velocity is positive and is smaller than the speed of light
    if (vc >= 0 && vc < 1) { v = vc;}
}

void spaceship::set_safe_dur(double sd) {
    if (sd >= 0) { safe_dur = sd;}
}

// acquire safe duration of flight
double spaceship::get_safe_dist() const { return v*safe_dur;}
