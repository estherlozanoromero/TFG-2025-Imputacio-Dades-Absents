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

double evaluateFitness(const Particle& p, const Dataset& dataset);

#endif
