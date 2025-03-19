#ifndef PSO_H
#define PSO_H

#include "Particle.h"
#include <vector>

class PSO {
public:
    PSO(int num_particles, int dimensions, int max_iterations,
        double c1, double c2, double w,
        double lower_bound, double upper_bound);

    void run();

private:
    int num_particles;
    int dimensions;
    int max_iterations;
    double c1, c2, w;
    double lower_bound, upper_bound;

    std::vector<Particle> swarm;
    std::vector<double> global_best_position;
    double global_best_fitness;
};

#endif // PSO_H
