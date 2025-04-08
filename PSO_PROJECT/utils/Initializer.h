#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <vector>

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
        MeanRandomInitializer(const vector<double>& means,
                              const vector<double>& mins,
                              const vector<double>& maxs,
                              double ratio);
    
        vector<double> initialize(int dim) override;
    
    private:
        vector<double> attribute_means;
        vector<double> min_values;
        vector<double> max_values;
        double random_ratio;
};

#endif // INITIALIZER_HH
