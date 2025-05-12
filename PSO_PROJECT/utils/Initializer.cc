#include "Initializer.h"

// Fills vector with purely random values (not scaled)
vector<double> RandomInitializer::initialize(int dim) {
    vector<double> position(dim);
    for (int i = 0; i < dim; ++i) {
        position[i] = static_cast<double>(rand());
    }
    return position;
}

// Uses min/max from dataset to generate bounded random values
BoundedRandomInitializer::BoundedRandomInitializer(const Dataset& dataset)
    : data(dataset) {}

vector<double> BoundedRandomInitializer::initialize(int dim) {
    vector<double> position(dim);
    vector<int> missing_cols = data.getMissingCols();

    for (int i = 0; i < dim; ++i) {
        int attr = missing_cols[i];
        double min = data.getMinAttributeAt(attr);
        double max = data.getMaxAttributeAt(attr);
        double r = static_cast<double>(rand()) / RAND_MAX;
        position[i] = min + r * (max - min);
    }

    return position;
}

// Uses mean ± noise or random value depending on ratio
MeanRandomInitializer::MeanRandomInitializer(const Dataset& dataset, double ratio)
    : data(dataset), random_ratio(ratio) {}

vector<double> MeanRandomInitializer::initialize(int dim) {
    vector<double> position(dim);
    vector<int> missing_cols = data.getMissingCols();

    for (int i = 0; i < dim; ++i) {
        int attr = missing_cols[i];
        double r = static_cast<double>(rand()) / RAND_MAX;

        double min = data.getMinAttributeAt(attr);
        double max = data.getMaxAttributeAt(attr);
        double mean = data.getMeanAttributeAt(attr);

        if (r < random_ratio) {
            // Random value between min and max
            position[i] = min + r * (max - min);
        } else {
            // Mean ± 5% noise
            double noise = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // [-1,1]
            double epsilon = 0.05 * (max - min);
            position[i] = mean + noise * epsilon;
        }
    }

    return position;
}
