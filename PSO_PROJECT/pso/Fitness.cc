#include "Fitness.h"

double evaluateFitness(const Particle& p, const Dataset& dataset) {
    Utils utils = Utils();
    const auto& raw = dataset.getRawData();
    const auto& rows = dataset.getMissingRows();
    const auto& cols = dataset.getMissingCols();

    // Copy the data to fill in
    vector<vector<double>> data = raw;

    // Impute the missing values using the particle
    for (size_t i = 0; i < rows.size(); ++i) {
        data[rows[i]][cols[i]] = p.position[i];
    }

    // Calculate column means
    int n_cols = data[0].size();
    int n_rows = data.size();
    vector<double> means(n_cols, 0.0);
    for (int j = 0; j < n_cols; ++j)
        means[j] = utils.calculateMean(data[j]);

    // Compute correlation
    vector<vector<double>> corr(n_cols, vector<double>(n_cols, 1.0));
    utils.computeCorrelationMatrix(corr, data, means);

    for (int i = 0; i < corr.size(); ++i) {
        for (int j = 0; j < corr[i].size(); ++j) {
            if (isnan(corr[i][j])) {
                cerr << "[ERROR] NaN in correlation matrix at (" << i << ", " << j << ")\n";
                return numeric_limits<double>::max();  // Penalitzation
            }
        }
    }

    // Compare to original correlation
    return utils.calculateMSE(corr, dataset.getCorrelationMatrix());
}
