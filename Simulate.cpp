#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <ctime>
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "Planet.h"
#include "Simulate.h"
#include "Spaceship.h"

using std::vector; using std::istream; using std::ostream; using std::ofstream;
using std::cout; using std::endl; using std::list; using std::map;

// time taken to develop a planet


// global generator
extern gsl_rng* r;

static const double pi = acos(-1);

// (global) time, v, max duration of a journey
static double global_t = 0.0, v = 0.0, safe_duration = 0.0;

// size of global clock tick, diameter, thickness of galaxy (ly), density of habitable planets
// dt should be small compared to time required to colonise/develop a planet/spacecraft
static const double dt = 20, galaxy_rad = 3000, r2 = galaxy_rad*0.59 , r1 = galaxy_rad*0.46, galaxy_thick = galaxy_rad/50, habp_den = 0.0000944;

// development time for the planet, interval between sending of spacecraft
static const double dev_time = 871, interval = 10;

// size of the disc shaped galaxy
static const double size_of_galaxy = pi*(r2*r2-r1*r1)*galaxy_thick;

// estimated number of habitable planets in the galaxy
static const int total_no_of_hbplanets = int(size_of_galaxy*habp_den);

// Output streams
// All habitable planets
ofstream output0 ("/Users/David/Documents/code/C++/ColonisingTheGalaxy/All habitable planets.xls");

// Coords of colonised planets
ofstream output1 ("/Users/David/Documents/code/C++/ColonisingTheGalaxy/Coordinates of colonised planets.xls");

// open and add results (Note: comment the header row for sucessive runs)
ofstream output2 ("/Users/David/Documents/code/C++/ColonisingTheGalaxy/r=1200.txt");

// Distances between planets
ofstream output3 ("/Users/David/Documents/code/C++/ColonisingTheGalaxy/Nearest HbPlanet Distances.xls");


double rand_coord(const double& length) {
    double rc;
    rc = (gsl_rng_uniform(r)-0.5)*2*length;
    return rc;
}

//Generate planet randomly in the Galaxy
vector<planet> generate()	{
    
    // set up the container of habitable planets within the galaxy
    vector<planet> galaxy;
    
    for (int i = 0; i < total_no_of_hbplanets; i++)
    {
        
        // generate again if the coord is outside the GHZ ring
        double x, y;
        do {
            x = rand_coord(galaxy_rad);
            y = rand_coord(galaxy_rad);
        } while ((x*x + y*y)>(r2*r2)||(x*x + y*y)<r1*r1);
        
        double z = rand_coord(galaxy_thick/2);
        
        //Add the newly generated planet to the galaxy
        galaxy.push_back(planet(0, x, y, z));			// assign the generated coord to the planet
        
        //galaxy[i].set_life(lifespan*gsl_rng_uniform(r));	// assign lifespan for each planet, uniform dist. 0 < x < 10 bill
    }
    for(int i = 0; i < sizeof(galaxy); i++)
      galaxy[i].print();
    return galaxy;
}

void record(vector<planet>& galaxy, int index, double t)		// Record time for development to complete, NB it doesn't change develop boolean since not developed yet
{
    galaxy[index].set_t(t);				// store time for development of the civilisation before it can start colonising
    galaxy[index].print(output1);		// record on file
}

void start(vector<planet>& galaxy, int& colon, list<int>& colonised_indices)	// default start at random position
{
    
    int start_pos = int(gsl_rng_uniform(r)*total_no_of_hbplanets);	// pick a random planet within the galaxy to start colonisation
    double dev_t = dt-1;	// okay as long as non-zero and less than dt
    
    record(galaxy, start_pos, dev_t);
    colon++;
    colonised_indices.push_back(start_pos);
    std::cout << "Starting Coordinates : ";
    galaxy[start_pos].print();
}

//void start(int n)	// start with chosen coordinate
//{
//	double dev_t = dt-1;	// okay as long as non-zero and less than dt
//
//	record(n, dev_t);
//	cout << "Starting Coordinates : ";
//	galaxy[n].print();
//}

void colonise(vector<planet>& galaxy, planet& p, int& colon, int& no_of_retire, list<int>& colonised_indices)		// carry out colonisation algorithm for planet p (to colonise nearest planet from planet p)
{
    // create a spaceship by dynamic allocation
    spaceship* sp = new spaceship(v, safe_duration);
    
    // distance from nearest neighbour, has to be smaller than safe_dist
    double shortest_dist = sp->get_safe_dist();
    
    // index of the nearest neighbour
    int nearest = 0;
    
    // boolean to flag whether a colonisable neighbour is found, and to determine whether current planet can retire
    bool found = false;
    
    // find nearest planet
    // search in whole galaxy for the first search of each planet
    if (p.get_col_count() == 0) {
        for (int n = 0; n < total_no_of_hbplanets ; n++)
        {
            // see if t = 0 (not colonised)
            if (int(galaxy[n].get_t()) == 0)
            {
                double dist = p.get_dist(galaxy[n]);	// evaluate distance between that planet and the current planet
                if (dist <= shortest_dist)
                {
                    
                    p.nearby_planets[n] = dist;
                    
                    // store the nearest distance and its index in the array
                    found = true;
                    nearest = n;
                    shortest_dist = dist;
                }
            }
        }
    }
    
    //search from the vector of nearby planet for subsequent search
    else {
        for(map<int, double>::iterator iter = p.nearby_planets.begin(); iter != p.nearby_planets.end(); iter++) {
            if(galaxy[iter->first].get_t() == 0 && iter->second < shortest_dist) {
                found = true;
                nearest = iter->first;
                shortest_dist = iter->second;
            }
        }
        p.nearby_planets.erase(nearest);
    }
    
    // neighbour exists and found
    if (found)
    {
        record(galaxy, nearest, global_t + shortest_dist/(sp->get_v()) + dev_time);		// time for travelling + est. time taken for the planet to reach type I status
        colon++;
        p.inc_col_count();
        colonised_indices.push_back(nearest);
        output3 << std::setprecision(2) << shortest_dist << endl;
    }
    
    // reachable neighbours of this planet have all been colonised, this planet will retire
    else
    {
        p.set_ret(true);
        no_of_retire++;
    }
    
    //
    delete sp;
}

void interstellar_travel()
{
    char type;
    cout << "Please select Propulsion and Life support method:" << endl;
    cout << "A for Fusion World ship" << endl;
    cout << "B for Antimatter annihilation World ship" << endl;
    cout << "C for Fusion Sleepers' ship" << endl;
    cout << "D for Antimatter annihilation Sleepers' ship"<< endl;
    cout << "E for Fusion Embryo ship" << endl;
    cout << "F for Antimatter annihilation Embryo ship" << endl;
    cout << "G for Laser-pushed lightsail Embryo ship" << endl;
    std::cin >> type;
    switch (type)
    {
        case 'A':
        case 'a':
        {
            safe_duration = 75;
            v=0.005;
            break;
        }
        case 'B':
        case 'b':
        {
            safe_duration = 75;
            v=0.05;
            break;
        }
        case 'C':
        case 'c':
        {
            safe_duration = 200;
            v=0.03;
            break;
        }
        case 'D':
        case 'd':
        {
            safe_duration = 200;
            v=0.3;
            break;
        }
        case 'E':
        case 'e':
        {
            safe_duration = 1000;
            v=0.1;
            break;
        }
        case 'F':
        case 'f':
        {
            safe_duration = 1000;
            v=0.9;
            break;
        }
        case 'G':
        case 'g':
        {
            safe_duration = 1000;
            v=0.3;
            break;
        }
        default:
        {
            cout<<"Please try again"<<endl;
            interstellar_travel();
            break;
        }
    }
}

void print_info()
{
    cout << "Size of galaxy : " << size_of_galaxy << " cubic light years" << endl;
    cout << "Number of habitable planets : " << total_no_of_hbplanets << endl << endl;
    //interstellar_travel();
    //cout << "(Constant) velocity of : " << v << "c | Safe duration: " << safe_duration << "years | Safe distance: "<< v*safe_duration << "ly" <<endl;
    
    output2 << "Size of galaxy" << '\t' << "v" << '\t' << "Safe duration" << '\t' << "P_f" << '\t' << "Time" << '\t' << "No. of hp" << '\t' << "No. of cp"  << '\t' << "Run_t" << endl;
    output3 << "Distances between hbplanets" << endl;
}

void print_results(int colon)	// print results on screen
{
    cout << "Time elapsed : " << global_t << " years" << endl
    << "No. of colonised planets : " << colon << endl;
}


// record all the habitable planets into file
void write_planets(const vector<planet>& galaxy) {
    for (vector<planet>::const_iterator it = galaxy.begin(); it != galaxy.end(); it++)
        it->print(output0);
}

void write_results(int colon)
{
    output2 << size_of_galaxy /*<< '\t' << v << '\t' << safe_duration */<< '\t' << global_t <<'\t' << total_no_of_hbplanets << '\t' << colon << '\t' << clock()/CLOCKS_PER_SEC << endl;
}

void reset(vector<planet>& galaxy, int& colon, int& no_of_retire, int& run)	// reset the parameters for colonisation beginning on another planet
{
    if(run > 1){
        for (int i = 0; i < total_no_of_hbplanets; i++)
        {
            galaxy[i].set_dev(false);
            galaxy[i].set_ret(false);
            galaxy[i].set_t(0);
        }
        colon = 0;
        no_of_retire = 0;
        global_t = 0;
    }
}

void simulation(vector<planet>& galaxy, int run)
{
    // no. of colonised planets, no. of colonised but retired planets
    int colon = 0, no_of_retire = 0;
    
    reset(galaxy, colon, no_of_retire, run);		// reset the parameters

    // doubly-linked list storing the indices of colonised planets
    list<int> colonised_indices;
    
    start(galaxy, colon, colonised_indices);		// Develop on a random planet
    
    // loop algorithm for colonisation
    cout << endl << "*****Colonising*****" << endl;
    
    // Stop simulation if all colonised planets have retired or all planets in the galaxy are colonised
    while ((no_of_retire < colon)&&(colon < total_no_of_hbplanets))
    {
        typedef list<int>::const_iterator con_iter;
        typedef list<int>::iterator iter;
        
        for (con_iter it = colonised_indices.begin(); it != colonised_indices.end(); it++) {
            
            vector<planet>::iterator mother_planet = galaxy.begin()+(*it); //*it exceeded the galaxy vector??
            
            // Check if the planet is developed
            if (mother_planet->get_dev())	{
                
                // Check if the planet has retired
                if(!mother_planet->get_ret()) {
                
                // and prevents overcounting when all planets are already colonised in the middle of loop
                    if (colon < total_no_of_hbplanets)
                            colonise(galaxy, *mother_planet, colon, no_of_retire, colonised_indices);
                }
                
                else {
                    // erase the retired planet from the list
                    colonised_indices.erase(it);
                }
                // to account for time needed before the planet can send another ship
                // development time depends on current Ep_rate and E req. for next journey
                mother_planet->set_dev(false);
                mother_planet->set_t(global_t + interval);
            }
            
            // the case where the planet is colonised but not developed yet
            else {
                if (mother_planet->get_t() < global_t)	// check if development has completed
                    mother_planet->set_dev(true);			// change developed to true
            }
        }
        global_t += dt;		// tick the global clock
    }
    
    // generate results
    cout << endl << endl << "*****Results*****" << endl;
    cout << "Time taken to run the program : " << clock()/CLOCKS_PER_SEC << " seconds" << endl;
    print_results(colon);	// on screen
    write_results(colon);	// on file
    
    colonised_indices.clear();		// clear the vector storing indices of colonised planets
}

void tiktok(double& total_t, const int i, const int sample){
    total_t += global_t;
    if(i == sample-1) output2 << total_t/sample;
}
