#include "Particle.h"

Particle::Particle(int dimensions) {
    position.resize(dimensions);           
    velocity.resize(dimensions, 0.0);      
    best_position.resize(dimensions);      
    best_fitness = numeric_limits<double>::max();
}
