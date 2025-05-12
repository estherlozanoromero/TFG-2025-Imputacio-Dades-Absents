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

// Particle Swarm Optimization (PSO) class for data imputation
class PSO {
public:
PSO(int num_particles, 
    int dimensions, 
    int max_iterations,
    double c1, double c2, double w,
    double velocity_ratio,
    int active_attributes,
    Initializer* init,
    const Dataset& dataset
);

    void run();
    double getBestFitness() const;
    const vector<double>& getGlobalBestPosition() const;
    vector<vector<double>> getBestCorrelationMatrix() const;

private:
    int num_particles;
    int dimensions;
    int max_iterations;
    double c1, c2, w, velocity_ratio;
    int active_attributes;

    Initializer* initializer;
    const Dataset& dataset;
    

    vector<Particle> swarm;
    vector<double> global_best_position;
    double global_best_fitness;

    double randomBetween(double min, double max);
};

#endif
