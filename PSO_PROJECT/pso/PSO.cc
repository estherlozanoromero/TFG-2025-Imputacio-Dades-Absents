#include "PSO.h"
#include "../utils/fitness.h"
#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

PSO::PSO(int num_particles, int dimensions, int max_iterations,
         double c1, double c2, double w,
         double lower_bound, double upper_bound)
    : num_particles(num_particles), dimensions(dimensions),
      max_iterations(max_iterations), c1(c1), c2(c2), w(w),
      lower_bound(lower_bound), upper_bound(upper_bound) {

    global_best_position.resize(dimensions);
    global_best_fitness = numeric_limits<double>::max();

    for (int i = 0; i < num_particles; ++i) {
        Particle p(dimensions, lower_bound, upper_bound);
        p.best_fitness = evaluateFitness(p.position);
        p.best_position = p.position;
        swarm.push_back(p);

        if (p.best_fitness < global_best_fitness) {
            global_best_fitness = p.best_fitness;
            global_best_position = p.best_position;
        }
    }
}

void PSO::run() {
    for (int iter = 0; iter < max_iterations; ++iter) {
        for (int i = 0; i < num_particles; ++i) {
            Particle& p = swarm[i];

            for (int d = 0; d < dimensions; ++d) {
                double r1 = static_cast<double>(rand()) / RAND_MAX;
                double r2 = static_cast<double>(rand()) / RAND_MAX;

                p.velocity[d] = w * p.velocity[d]
                              + c1 * r1 * (p.best_position[d] - p.position[d])
                              + c2 * r2 * (global_best_position[d] - p.position[d]);

                p.position[d] += p.velocity[d];

                if (p.position[d] < lower_bound) p.position[d] = lower_bound;
                if (p.position[d] > upper_bound) p.position[d] = upper_bound;
            }

            double fitness = evaluateFitness(p.position);
            if (fitness < p.best_fitness) {
                p.best_fitness = fitness;
                p.best_position = p.position;
            }

            if (fitness < global_best_fitness) {
                global_best_fitness = fitness;
                global_best_position = p.position;
            }
        }

        cout << "Iteration " << iter + 1 << " - Global Best Fitness: " << global_best_fitness << endl;
    }
}
