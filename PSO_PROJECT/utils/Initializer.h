#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "Dataset.h"
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

// Abstract base class for all particle initializers
class Initializer {
public:
    virtual vector<double> initialize(int dim) = 0;
    virtual ~Initializer() {}
};

// Generates random values in [0, RAND_MAX] (not recommended for real use)
class RandomInitializer : public Initializer {
public:
    vector<double> initialize(int dim) override;
};

// Generates random values within min/max per attribute from the dataset
class BoundedRandomInitializer : public Initializer {
public:
    BoundedRandomInitializer(const Dataset& dataset);
    vector<double> initialize(int dim) override;

private:
    const Dataset& data;
};

// Mixes mean values with small noise and random values, based on ratio
class MeanRandomInitializer : public Initializer {
public:
    MeanRandomInitializer(const Dataset& dataset, double ratio);
    vector<double> initialize(int dim) override;

private:
    const Dataset& data;
    double random_ratio;
};

#endif // INITIALIZER_H
