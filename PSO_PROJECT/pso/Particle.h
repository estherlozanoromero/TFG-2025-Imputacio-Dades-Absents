#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include <limits>

using namespace std;

class Particle {
public:
    vector<double> position;
    vector<double> velocity;
    vector<double> best_position;
    double best_fitness;

    Particle(int dimensions);
};

#endif // PARTICLE_H