#include "Initializer.h"


vector<double> RandomInitializer::initialize(int dim) {
    vector<double> position(dim);
    for (int i = 0; i < dim; ++i) {
        position[i] = static_cast<double>(rand());
    }
    return position;
}


MeanRandomInitializer::MeanRandomInitializer(const Dataset& dataset,
                                             double ratio)
    : data(dataset), random_ratio(ratio) {}

vector<double> MeanRandomInitializer::initialize(int size) {
    vector<double> position(size);
    vector<int> missing_values_cols = data.getMissingCols();
    for (int i = 0; i < size; ++i) {
        double r = static_cast<double>(rand()) / RAND_MAX;
        if (r < random_ratio) {
            position[i] = data.getMinAttributeAt(missing_values_cols[i]) + r * (data.getMaxAttributeAt(missing_values_cols[i]) - data.getMinAttributeAt(missing_values_cols[i]));
        } else {
            position[i] = data.getMeanAttributeAt(missing_values_cols[i]);
        }
    }
    return position;
}
