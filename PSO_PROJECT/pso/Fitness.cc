#include "Fitness.h"

double evaluateFitness(const Particle& p, const Dataset& dataset, int active_attributes) {
    Utils utils;
    const auto& raw = dataset.getRawData();
    const auto& rows = dataset.getMissingRows();
    const auto& cols = dataset.getMissingCols();

    // Copy and impute missing values with the particle's proposed solution
    vector<vector<double>> data = raw;
    for (size_t i = 0; i < rows.size(); ++i) {
        data[rows[i]][cols[i]] = p.position[i];
    }

    int n_cols = data[0].size();
    int n_rows = data.size();

    // Compute mean per column
    vector<double> means(n_cols, 0.0);
    for (int j = 0; j < n_cols; ++j) {
        for (int i = 0; i < n_rows; ++i) {
            means[j] += data[i][j];
        }
        means[j] /= n_rows;
    }

    // Compute correlation matrix of the imputed dataset
    vector<vector<double>> corr(n_cols, vector<double>(n_cols, 1.0));
    utils.computeCorrelationMatrix(corr, data, means);

    // Check for NaNs in correlation matrix
    for (int i = 0; i < n_cols; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            if (isnan(corr[i][j])) {
                cerr << "[ERROR] NaN in correlation matrix at (" << i << ", " << j << ")\n";
                return numeric_limits<double>::max(); // Penalize invalid solution
            }
        }
    }

    const auto& ref_corr = dataset.getCorrelationMatrix();

    // Compare full matrix or top-left submatrix if attribute limit is set
    if (active_attributes > 0 && active_attributes < n_cols) {
        vector<vector<double>> corr_sub(active_attributes, vector<double>(active_attributes));
        vector<vector<double>> ref_sub(active_attributes, vector<double>(active_attributes));

        for (int i = 0; i < active_attributes; ++i)
            for (int j = 0; j < active_attributes; ++j) {
                corr_sub[i][j] = corr[i][j];
                ref_sub[i][j] = ref_corr[i][j];
            }

        return utils.calculateMSE(corr_sub, ref_sub);
    }

    return utils.calculateMSE(corr, ref_corr);
}
