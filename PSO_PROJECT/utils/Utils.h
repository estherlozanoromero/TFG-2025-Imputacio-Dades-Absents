#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

// Utility class to encapsulate statistical and helper functions
class Utils {
public:
    Utils();

    // Returns the minimum value from a vector
    double getMinValue(const std::vector<double>& values);

    // Returns the maximum value from a vector
    double getMaxValue(const std::vector<double>& values);

    // Calculates the arithmetic mean of a vector
    double calculateMean(const std::vector<double>& values);

    // Calculates the Mean Squared Error (MSE) between two matrices
    double calculateMSE(const std::vector<std::vector<double>>& a,
                        const std::vector<std::vector<double>>& b);

    // Computes Pearson correlation matrix given the data and precomputed means
    void computeCorrelationMatrix(std::vector<std::vector<double>>& correlation_matrix,
                                  const std::vector<std::vector<double>>& data,
                                  const std::vector<double>& mean_attributes);

    // Nicely prints a 2D matrix with optional formatting controls
    void printMatrix(const std::vector<std::vector<double>>& matrix,
                     int precision = 4,
                     int max_rows = -1,
                     int max_cols = -1);
};

#endif
