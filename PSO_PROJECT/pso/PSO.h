#ifndef PSO_H
#define PSO_H

#include "Particle.h"
#include "Fitness.h"
#include "../utils/Initializer.h"
#include "../utils/Dataset.h"

#include <iostream>
#include <cstdlib>
#include <limits>
#include <vector>

using namespace std;

class PSO {
public:
    PSO(int num_particles, int dimensions, int max_iterations,
        double c1, double c2, double w,
        Initializer* init,
        const Dataset& dataset);

    void run();

private:
    int num_particles;
    int dimensions;
    int max_iterations;
    double c1, c2, w;
    
    Initializer* initializer;
    const Dataset& dataset; 

    vector<Particle> swarm;
    vector<double> global_best_position;
    double global_best_fitness;
    double randomBetween(double min, double max);
};

#endif
