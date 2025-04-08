#include "Initializer.h"
#include <cstdlib>

vector<double> RandomInitializer::initialize(int dim) {
    vector<double> position(dim);
    for (int i = 0; i < dim; ++i) {
        position[i] = static_cast<double>(rand());
    }
    return position;
}


MeanRandomInitializer::MeanRandomInitializer(const vector<double>& means,
                                             const vector<double>& mins,
                                             const vector<double>& maxs,
                                             double ratio)
    : attribute_means(means), min_values(mins), max_values(maxs), random_ratio(ratio) {}

vector<double> MeanRandomInitializer::initialize(int dim) {
    vector<double> position(dim);
    for (int i = 0; i < dim; ++i) {
        double r = static_cast<double>(rand()) / RAND_MAX;
        if (r < random_ratio) {
            position[i] = min_values[i] + r * (max_values[i] - min_values[i]);
        } else {
            position[i] = attribute_means[i];
        }
    }
    return position;
}
