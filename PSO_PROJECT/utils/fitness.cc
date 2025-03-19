#include "fitness.h"

double evaluateFitness(const std::vector<double>& position) {
    double fitness = 0.0;
    for (double val : position) {
        fitness += val * val; // TEMPORAL FITNESS (DUMMY)
    }
    return fitness;
}
