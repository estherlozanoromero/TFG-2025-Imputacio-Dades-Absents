#include "Utils.h"

Utils::Utils() {}

double Utils::getMinValue(const vector<double>& values) {
    if (values.empty()) {
        return numeric_limits<double>::quiet_NaN();
    }

    double min = numeric_limits<double>::max();

    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i] < min) {
            min = values[i];
        }
    }

    return min;
}

double Utils::getMaxValue(const vector<double>& values) {
    if (values.empty()) {
        return numeric_limits<double>::quiet_NaN();
    }

    double max = numeric_limits<double>::lowest();

    for (size_t i = 0; i < values.size(); ++i) {
        if (values[i] > max) {
            max = values[i];
        }
    }

    return max;
}

double Utils::calculateMean(const vector<double>& values) {
    if (values.empty()) {
        return numeric_limits<double>::quiet_NaN();
    }
    
    double sum = 0.0;
    int size = values.size();
    for(int i = 0; i < size; ++i) {
        sum += values[i];
    }
    return sum/size;
}

double Utils::calculateMSE(const vector<vector<double>>& a, const vector<vector<double>>& b) {
    if (a.size() != b.size() || a[0].size() != b[0].size()) {
        cerr << "[ERROR] Correlation matrix sizes do not match." << endl;
        exit(1);
    }

    double mse = 0.0;
    int n = a.size();

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            mse += pow(a[i][j] - b[i][j], 2);

    return mse / (n * n);
}

void Utils::computeCorrelationMatrix(vector<vector<double>>& correlation_matrix, const vector<vector<double>>& data, const vector<double>& mean_attributes) {
    
    int cols = data[0].size();
    int rows = data.size();
    
    // Calculate correlation
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
            correlation_matrix[i][j] = num / sqrt(denom_i * denom_j + 1e-8); // skip divide by 0
        }
    }
}

void Utils::printMatrix(const vector<vector<double>>& matrix, int precision, int max_rows, int max_cols) {
    int rows = matrix.size();
    if (rows == 0) {
        cout << "[INFO] Empty matrix.\n";
        return;
    }

    int cols = matrix[0].size();
    int display_rows = (max_rows > 0 && max_rows < rows) ? max_rows : rows;
    int display_cols = (max_cols > 0 && max_cols < cols) ? max_cols : cols;

    cout << fixed << setprecision(precision);

    for (int i = 0; i < display_rows; ++i) {
        for (int j = 0; j < display_cols; ++j) {
            if (std::isnan(matrix[i][j])) {
                cout << "NaN\t";
            } else {
                cout << matrix[i][j] << "\t";
            }
        }
        if (display_cols < cols)
            cout << "...";
        cout << endl;
    }
    if (display_rows < rows)
        cout << "...\n";
}





