#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

using namespace std;

class Utils {
public:
    Utils();
    
    double getMinValue(const vector<double>& values);
    double getMaxValue(const vector<double>& values);
    double calculateMean(const vector<double>& values);
    double calculateMSE(const vector<vector<double>>& a, const vector<vector<double>>& b);
    void computeCorrelationMatrix(vector<vector<double>>& correlation_matrix, const vector<vector<double>>& data, const vector<double>& mean_attributes);
    void printMatrix(const vector<vector<double>>& matrix, int precision = 4, int max_rows = -1, int max_cols = -1);
};

#endif
