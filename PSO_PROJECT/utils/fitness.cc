#include "Fitness.h"
#include <cmath>
#include <iostream>

// Helper: convert string to double or return 0
double safe_stod(const string& s) {
    try {
        return stod(s);
    } catch (...) {
        return 0.0;
    }
}

double evaluateFitness(const vector<double>& position,
                       const vector<vector<string>>& raw_data,
                       const vector<vector<double>>& target_corr,
                       const vector<double>& lower,
                       const vector<double>& upper) {
    
    vector<vector<double>> data;
    int pos_idx = 0;
    double penalty = 0.0;

    for (const auto& row : raw_data) {
        vector<double> filled_row;
        for (const auto& val : row) {
            if (val.empty() || val == "nan") {
                if (pos_idx >= position.size()) {
                    cerr << "[ERROR] Particle dimension is smaller than number of missing values.\n";
                    exit(1);
                }

                double value = position[pos_idx];
                // Penalize if value is out of expected bounds
                if (value < lower[pos_idx] || value > upper[pos_idx]) {
                    penalty += 100.0; // Optional: tune this factor
                }

                filled_row.push_back(value);
                pos_idx++;
            } else {
                filled_row.push_back(safe_stod(val));
            }
        }
        data.push_back(filled_row);
    }

    int n = data[0].size();  // num columns
    int m = data.size();     // num rows
    vector<vector<double>> corr(n, vector<double>(n, 0.0));

    // Compute column means
    vector<double> mean(n, 0.0);
    for (const auto& row : data)
        for (int j = 0; j < n; ++j)
            mean[j] += row[j];
    for (int j = 0; j < n; ++j)
        mean[j] /= m;

    // Compute correlation matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double num = 0.0, denom_i = 0.0, denom_j = 0.0;
            for (int k = 0; k < m; ++k) {
                double xi = data[k][i] - mean[i];
                double xj = data[k][j] - mean[j];
                num += xi * xj;
                denom_i += xi * xi;
                denom_j += xj * xj;
            }
            corr[i][j] = num / sqrt(denom_i * denom_j + 1e-8);
        }
    }

    // Compute MSE between correlation matrices
    double mse = 0.0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            mse += pow(corr[i][j] - target_corr[i][j], 2);

    return mse / (n * n) + penalty;
}
