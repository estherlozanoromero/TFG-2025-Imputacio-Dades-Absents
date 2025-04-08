#ifndef FITNESS_H
#define FITNESS_H

#include <vector>
#include <string>

using namespace std;

double evaluateFitness(const vector<double>& position,
                       const vector<vector<string>>& raw_data,
                       const vector<vector<double>>& target_corr,
                       const vector<double>& lower,
                       const vector<double>& upper);

#endif
