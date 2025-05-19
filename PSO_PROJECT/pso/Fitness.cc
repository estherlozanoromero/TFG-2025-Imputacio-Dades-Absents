#include "Fitness.h"

double evaluateFitness(const Particle& p, const Dataset& dataset) {
    Utils utils;
    const auto& raw = dataset.getRawData();
    const auto& rows = dataset.getMissingRows();
    const auto& cols = dataset.getMissingCols();

    // Copy and impute missing values with the particle's proposed solution
    vector<vector<double>> data = raw;
    for (size_t i = 0; i < rows.size(); ++i) {
        int r = dataset.getMissingRowAt(i);
        int c = dataset.getMissingColAt(i);
        data[r][c] = p.position[i];
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

    const auto& ref_corr = dataset.getCorrelationMatrix();

    return utils.calculateMSE(corr, ref_corr);
}
