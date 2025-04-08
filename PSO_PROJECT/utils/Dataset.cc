#include "Dataset.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>

Dataset::Dataset(const string& filepath) : filepath(filepath) {}

void Dataset::load() {
    ifstream file(filepath);
    string line;

    bool first_line = true; 
    while (getline(file, line)) {
        if (first_line) {
            first_line = false;
            continue;
        }

        stringstream ss(line);
        string value;
        vector<string> row;

        while (getline(ss, value, ',')) {
            row.push_back(value);
        }

        raw_data.push_back(row);
        if (!hasMissing(row)) {
            vector<double> clean_row;
            try {
                for (const auto& val : row)
                    clean_row.push_back(stod(val));
                clean_data.push_back(clean_row);
            } catch (const invalid_argument& e) {
                cerr << "[ERROR] Invalid value in dataset" << endl;
                exit(1);
            }
        }
    }

    if (!clean_data.empty()) {
        int cols = clean_data[0].size();
        min_attributes = vector<double>(cols, numeric_limits<double>::max());
        max_attributes = vector<double>(cols, numeric_limits<double>::lowest());

        for (const auto& row : clean_data) {
            for (int i = 0; i < cols; ++i) {
                if (row[i] < min_attributes[i]) min_attributes[i] = row[i];
                if (row[i] > max_attributes[i]) max_attributes[i] = row[i];
            }
        }
    } else {
        cerr << "[ERROR] No valid rows found for min/max calculation." << endl;
        exit(1);
    }
}


bool Dataset::hasMissing(const vector<string>& row) const {
    for (const auto& val : row) {
        if (val.empty() || val == "nan") return true;
    }
    return false;
}

int Dataset::countMissingValues() const {
    int count = 0;
    for (const auto& row : raw_data) {
        for (const auto& val : row) {
            if (val.empty() || val == "nan")
                count++;
        }
    }
    return count;
}

vector<vector<double>> Dataset::getCleanData() const {
    return clean_data;
}

vector<vector<string>> Dataset::getRawData() const { 
    return raw_data; 
}

 double Dataset::getMinAttributeAt(int pos) const {
    return min_attributes[pos];
}

double Dataset::getMaxAttributeAt(int pos) const {
    return max_attributes[pos];
}

vector<double> Dataset::getMinAttributes() const {
    return min_attributes;
}

vector<double> Dataset::getMaxAttributes() const {
    return max_attributes;
}

vector<vector<double>> Dataset::computeCorrelationMatrix() const {
    int cols = clean_data[0].size();
    int rows = clean_data.size();

    vector<vector<double>> corr(cols, vector<double>(cols, 0.0));

    // Calculate mean's
    vector<double> mean(cols, 0.0);
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i)
            mean[j] += clean_data[i][j];
        mean[j] /= rows;
    }

    // Calculate correlation
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < cols; ++j) {
            double num = 0.0, denom_i = 0.0, denom_j = 0.0;
            for (int k = 0; k < rows; ++k) {
                double xi = clean_data[k][i] - mean[i];
                double xj = clean_data[k][j] - mean[j];
                num += xi * xj;
                denom_i += xi * xi;
                denom_j += xj * xj;
            }
            corr[i][j] = num / sqrt(denom_i * denom_j + 1e-8); // skip divide by 0
        }
    }

    return corr;
}
