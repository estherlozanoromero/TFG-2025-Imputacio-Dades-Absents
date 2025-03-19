#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

class Particle {
public:
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> best_position;
    double best_fitness;

    Particle(int dimensions, double lower_bound, double upper_bound);
};

#endif // PARTICLE_H