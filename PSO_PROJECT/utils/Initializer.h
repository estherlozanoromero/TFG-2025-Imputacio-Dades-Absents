#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <Dataset.h>

#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

class Initializer {
public:
    virtual vector<double> initialize(int dim) = 0;
    virtual ~Initializer() {}
};

class RandomInitializer : public Initializer {
public:
    vector<double> initialize(int dim) override;
};

class MeanRandomInitializer : public Initializer {
    public:
        MeanRandomInitializer(const Dataset& data,
                              double ratio);
    
        vector<double> initialize(int size) override;
    
    private:
        Dataset data;
        double random_ratio;
};

#endif // INITIALIZER_HH
