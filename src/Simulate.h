#pragma once

#include <stdio.h>
#include <vector>

double rand_coord(const double&);
std::vector<planet> generate();
void record(std::vector<planet>& galaxy, int index, double t);
void start(std::vector<planet>&, int&, std::vector<int>&);
void interstellar_travel();
void print_info();
void print_results(int);
void write_planets(const std::vector<planet>&);
void write_results(int);
void colonise(std::vector<planet>&, planet&, int&, int&);
void simulation(std::vector<planet>&, int);
void tiktok(double&, const int, const int);

