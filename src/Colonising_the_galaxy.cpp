#include <iostream>
#include <vector>
// GSL - GNU Scientific Library
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "Planet.h"
#include "Simulate.h"


// global generator
gsl_rng* r;

int main() {
    
    const gsl_rng_type* T;
    
    // reads environmental variable GSL_RNG_TYPE and GSL_RNG_SEED from the cmd line and
    // set the corresponding library variables gsl_rng_default and gsl_rng_default_seed
    gsl_rng_env_setup();
    
    // type of the rng
    T = gsl_rng_default;
    
    // Create an instance of a rng of type T
    r = gsl_rng_alloc(T);
    
    std::cout << "generator type : " << gsl_rng_name(r) << std::endl;
    std::cout << "seed = " << gsl_rng_default_seed << std::endl;
    std::cout << "first value = " << gsl_rng_get(r) << "\n" << std::endl;
    
    // Initialize the galaxy with randomly distributed habitable planets
    std::vector<planet> galaxy = generate();
    
    // record all the habitable planets into file
	write_planets(galaxy);
    
    // choose spaceship
    interstellar_travel();
    
    // print parameters inputted
	print_info();

    
    double total_t = 0.0; // used to take average from multiple simulations
    const int sample = 1;	// number of samples
    
	for (int run = 0; run < sample; run++)	// loop colonisation algorithm, repeated with different starting positions
	{
		simulation(galaxy, run);
        tiktok(total_t, run, sample);
	}
    
    // free all the memory associated with the rng
    gsl_rng_free(r);
	return 0;
}

