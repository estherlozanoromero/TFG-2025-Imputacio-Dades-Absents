#include "PSO.h"

PSO::PSO(int num_particles, 
        int dimensions, 
        int max_iterations,
        double c1, double c2, double w,
        double velocity_ratio,
        Initializer* init,
        const Dataset& dataset
    ): 
        num_particles(num_particles), 
        dimensions(dimensions),
        max_iterations(max_iterations), 
        c1(c1), c2(c2), w(w),
        velocity_ratio(velocity_ratio),
        initializer(init), 
        dataset(dataset)
{
    global_best_position.resize(dimensions);
    global_best_fitness = numeric_limits<double>::max();

    vector<double> min_value_per_attribute = dataset.getMinAttributes();
    vector<double> max_value_per_attribute = dataset.getMaxAttributes();
    vector<int> missing_values_cols = dataset.getMissingCols();

    for (int i = 0; i < num_particles; ++i) {
        vector<double> position = initializer->initialize(dimensions);
        Particle p(dimensions);
        p.position = position;
        p.best_position = position;
        p.best_fitness = evaluateFitness(p, dataset);

        for (int d = 0; d < dimensions; ++d) {
            double range = max_value_per_attribute[missing_values_cols[d]] - min_value_per_attribute[missing_values_cols[d]];
            p.velocity[d] = randomBetween(-range, range);
        }

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
                double max_value = dataset.getMaxAttributeAt(dataset.getMissingColAt(d));
                double min_value = dataset.getMinAttributeAt(dataset.getMissingColAt(d));
                double r1 = static_cast<double>(rand()) / RAND_MAX;
                double r2 = static_cast<double>(rand()) / RAND_MAX;

                double range = max_value - min_value;
                double max_vel = velocity_ratio * range;

                p.velocity[d] = w * p.velocity[d]
                              + c1 * r1 * (p.best_position[d] - p.position[d])
                              + c2 * r2 * (global_best_position[d] - p.position[d]);

                // Clip velocity
                if (p.velocity[d] > max_vel) p.velocity[d] = max_vel;
                if (p.velocity[d] < -max_vel) p.velocity[d] = -max_vel;

                // Update position
                p.position[d] += p.velocity[d];

                // Clamp position within valid range
                if (p.position[d] < min_value)
                    p.position[d] = min_value;
                if (p.position[d] > max_value)
                    p.position[d] = max_value;
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

const vector<double>& PSO::getGlobalBestPosition() const {
    return global_best_position;
}

vector<vector<double>> PSO::getBestCorrelationMatrix() const {
    Particle best_particle(dimensions);
    best_particle.position = global_best_position;

    vector<vector<double>> data = dataset.getRawData();
    const auto& rows = dataset.getMissingRows();
    const auto& cols = dataset.getMissingCols();

    for (size_t i = 0; i < rows.size(); ++i) {
        data[rows[i]][cols[i]] = global_best_position[i];
    }

    // Calculate mean
    int n_cols = data[0].size();
    int n_rows = data.size();
    vector<double> means(n_cols, 0.0);
    for (int j = 0; j < n_cols; ++j)
        for (int i = 0; i < n_rows; ++i)
            means[j] += data[i][j];
    for (int j = 0; j < n_cols; ++j)
        means[j] /= n_rows;

    // Calculate correlation
    vector<vector<double>> corr(n_cols, vector<double>(n_cols, 1.0));
    Utils utils;
    utils.computeCorrelationMatrix(corr, data, means);

    return corr;
}

double PSO::getBestFitness() const {
    return global_best_fitness;
}

double PSO::randomBetween(double min, double max) {
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}
