#include "Utils.h"

Utils::Utils() {}

double Utils::getMinValue(const std::vector<double>& values) {
    if (values.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    double min_val = std::numeric_limits<double>::max();
    for (double val : values) {
        if (val < min_val) min_val = val;
    }
    return min_val;
}

double Utils::getMaxValue(const std::vector<double>& values) {
    if (values.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    double max_val = std::numeric_limits<double>::lowest();
    for (double val : values) {
        if (val > max_val) max_val = val;
    }
    return max_val;
}

double Utils::calculateMean(const std::vector<double>& values) {
    if (values.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    double sum = 0.0;
    for (double val : values) {
        sum += val;
    }
    return sum / values.size();
}

double Utils::calculateMSE(const std::vector<std::vector<double>>& a,
                           const std::vector<std::vector<double>>& b) {
    if (a.size() != b.size() || a[0].size() != b[0].size()) {
        std::cerr << "[ERROR] Correlation matrix sizes do not match." << std::endl;
        exit(1);
    }

    double mse = 0.0;
    int n = a.size();

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            mse += std::pow(a[i][j] - b[i][j], 2);

    return mse / (n * n);
}

void Utils::computeCorrelationMatrix(std::vector<std::vector<double>>& correlation_matrix,
                                     const std::vector<std::vector<double>>& data,
                                     const std::vector<double>& mean_attributes) {
    int rows = data.size();
    int cols = data[0].size();

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < cols; ++j) {
            double num = 0.0, denom_i = 0.0, denom_j = 0.0;

            for (int k = 0; k < rows; ++k) {
                double xi = data[k][i] - mean_attributes[i];
                double xj = data[k][j] - mean_attributes[j];
                num += xi * xj;
                denom_i += xi * xi;
                denom_j += xj * xj;
            }

            double denominator = std::sqrt(denom_i * denom_j);
            if (denominator < 1e-8) {
                correlation_matrix[i][j] = std::numeric_limits<double>::quiet_NaN();
                std::cerr << "[ERROR] NaN in correlation matrix at (" << i << ", " << j << ") - zero or invalid denominator" << std::endl;
            } else {
                correlation_matrix[i][j] = num / denominator;
            }
        }
    }
}

void Utils::printMatrix(const std::vector<std::vector<double>>& matrix,
                        int precision, int max_rows, int max_cols) {
    int rows = matrix.size();
    if (rows == 0) {
        std::cout << "[INFO] Empty matrix.\n";
        return;
    }

    int cols = matrix[0].size();
    int display_rows = (max_rows > 0 && max_rows < rows) ? max_rows : rows;
    int display_cols = (max_cols > 0 && max_cols < cols) ? max_cols : cols;

    std::cout << std::fixed << std::setprecision(precision);

    for (int i = 0; i < display_rows; ++i) {
        for (int j = 0; j < display_cols; ++j) {
            if (std::isnan(matrix[i][j])) {
                std::cout << "NaN\t";
            } else {
                std::cout << matrix[i][j] << "\t";
            }
        }
        if (display_cols < cols)
            std::cout << "...";
        std::cout << std::endl;
    }

    if (display_rows < rows)
        std::cout << "...\n";
}
