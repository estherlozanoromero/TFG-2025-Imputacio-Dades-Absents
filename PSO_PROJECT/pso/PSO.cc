#include "PSO.h"

using namespace std;

PSO::PSO(int num_particles, int dimensions, int max_iterations,
         double c1, double c2, double w,
         Initializer* init,
         const Dataset& dataset)
    : num_particles(num_particles), dimensions(dimensions),
      max_iterations(max_iterations), c1(c1), c2(c2), w(w),
      initializer(init), dataset(dataset)
{
    global_best_position.resize(dimensions);
    global_best_fitness = numeric_limits<double>::max();

    vector<double> min_value_per_attribute = dataset.getMinAttributes();
    vector<double> max_value_per_attribute = dataset.getMaxAttributes();
    const auto& raw_data = dataset.getRawData();
    const auto& target_corr = dataset.getCorrelationMatrix();

    for (int i = 0; i < num_particles; ++i) {

        
        vector<double> position = initializer->initialize(dimensions);
        Particle p(dimensions);
        p.position = position;
        p.best_position = position;
        p.best_fitness = evaluateFitness(p, dataset);

        for (int d = 0; d < dimensions; ++d) {
            // Random velocity between -1.0 and 1.0
            p.velocity[d] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        }

        swarm.push_back(p);

        if (p.best_fitness < global_best_fitness) {
            global_best_fitness = p.best_fitness;
            global_best_position = p.best_position;
        }
    }
}

void PSO::run() {
    vector<double> min_value_per_attribute = dataset.getMinAttributes();
    vector<double> max_value_per_attribute = dataset.getMaxAttributes();
    const auto& raw_data = dataset.getRawData();
    const auto& target_corr = dataset.getCorrelationMatrix();

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

                if (p.position[d] < min_value_per_attribute[d]) p.position[d] = min_value_per_attribute[d];
                if (p.position[d] > max_value_per_attribute[d]) p.position[d] = max_value_per_attribute[d];
            }

            double fitness = evaluateFitness(p, dataset);
            
            if (i == 0 && (iter == 0 || iter % 10 == 0)) {
                cout << "[DEBUG] Fitness de p0 en iter " << iter << ": " << fitness << endl;
            }

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
