#ifndef FITNESS_H
#define FITNESS_H

#include "Particle.h"
#include "../utils/Dataset.h"
#include "../utils/Utils.h"

#include <vector>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

// Evaluates the fitness of a particle based on the MSE of correlation matrices.
// If active_attributes > 0, it only compares the top-left submatrix.
double evaluateFitness(const Particle& p, const Dataset& dataset);

#endif
