#include "Particle.h"
#include <cstdlib>
#include <limits>

Particle::Particle(int dimensions, double lower_bound, double upper_bound) {
    position.resize(dimensions);
    velocity.resize(dimensions);
    best_position.resize(dimensions);
    best_fitness = std::numeric_limits<double>::max();

    for (int i = 0; i < dimensions; ++i) {
        position[i] = lower_bound + static_cast<double>(rand()) / RAND_MAX * (upper_bound - lower_bound);
        velocity[i] = 0.0;
        best_position[i] = position[i];
    }
}