//
//  Spaceship.h
//  Model_Galactic_Colonisation
//
//  Created by David Choy on 06/01/2015.
//  Copyright (c) 2015 pkdc. All rights reserved.
//

//#ifndef __Model_Galactic_Colonisation__Spaceship__
//#define __Model_Galactic_Colonisation__Spaceship__
#pragma once

#include <stdio.h>


class spaceship {
    
public:
    
    // default Constructor
    spaceship();
    
    spaceship(double, double);
    
    // access methods
    double get_v() const;
    
    double get_safe_dur() const;
    
    // modifiers
    void set_v(double);
    
    void set_safe_dur(double);
    
    // acquire safe duration of flight
    double get_safe_dist() const;
    
    // number of colonised planets
    //static int colon;
    
    
private:
    
    double v;
    double safe_dur;
    
};

#endif /* defined(__Model_Galactic_Colonisation__Spaceship__) */
