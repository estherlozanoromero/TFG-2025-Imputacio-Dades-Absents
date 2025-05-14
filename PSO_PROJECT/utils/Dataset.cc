#include "Dataset.h"
#include "Utils.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>  
#include <unistd.h>    

Dataset::Dataset(const string& filepath) : filepath(filepath) {}

void Dataset::load() {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "[ERROR] Could not open file: " << filepath << endl;
        exit(1);
    }

    string line;
    Utils utils;
    bool first_line = true;
    vector<vector<string>> raw_lines;
    int expected_columns = 0;

    // Step 1: Read and determine maximum number of columns
    while (getline(file, line)) {
        if (first_line) {
            first_line = false; // Skip header
            continue;
        }

        stringstream ss(line);
        string value;
        vector<string> row;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }

        expected_columns = max(expected_columns, static_cast<int>(row.size()));
        raw_lines.push_back(row);
    }

    // Step 2: Parse rows
    int row_index = 0;
    for (const auto& string_row : raw_lines) {
        vector<double> parsed_row(expected_columns, numeric_limits<double>::quiet_NaN());
        bool has_missing = false;

        for (int col = 0; col < expected_columns; ++col) {
            if (col >= static_cast<int>(string_row.size()) || isMissingValue(string_row[col])) {
                missing_row_indices.push_back(row_index);
                missing_col_indices.push_back(col);
                has_missing = true;
            } else {
                try {
                    parsed_row[col] = stod(string_row[col]);
                } catch (...) {
                    cerr << "[ERROR] Invalid numeric value at row " << row_index << ", col " << col << endl;
                    exit(1);
                }
            }
        }

        raw_data.push_back(parsed_row);
        if (!has_missing) clean_data.push_back(parsed_row);
        row_index++;
    }

    if (clean_data.empty()) {
        cerr << "[ERROR] No valid data to compute statistics." << endl;
        exit(1);
    }

    // Step 3: Compute column stats
    int cols = clean_data[0].size();
    int rows = clean_data.size();

    min_attributes.assign(cols, numeric_limits<double>::max());
    max_attributes.assign(cols, numeric_limits<double>::lowest());
    mean_attributes.assign(cols, 0.0);

    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            double val = clean_data[i][j];
            min_attributes[j] = min(min_attributes[j], val);
            max_attributes[j] = max(max_attributes[j], val);
            mean_attributes[j] += val;
        }
        mean_attributes[j] /= rows;
    }

    // Step 4: Correlation matrix
    correlation_matrix = vector<vector<double>>(cols, vector<double>(cols, 1.0));
    utils.computeCorrelationMatrix(correlation_matrix, clean_data, mean_attributes);
}

void Dataset::exportImputedData(const vector<double>& imputed_values, const string& output_path) {
    if (imputed_values.size() != missing_row_indices.size()) {
        cerr << "[ERROR] Mismatch between missing values and imputations." << endl;
        return;
    }

    // Reconstruct completed dataset
    vector<vector<double>> completed_data = raw_data;
    for (size_t i = 0; i < missing_row_indices.size(); ++i) {
        int row = missing_row_indices[i];
        int col = missing_col_indices[i];
        completed_data[row][col] = imputed_values[i];
    }

    // Write the CSV
    ofstream out(output_path);
    if (!out.is_open()) {
        cerr << "[ERROR] Cannot open output file: " << output_path << endl;
        return;
    }

    for (const auto& row : completed_data) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (isnan(row[i]))
                out << "nan";
            else
                out << row[i];
            if (i < row.size() - 1) out << ",";
        }
        out << "\n";
    }

    out.close();
    cout << "[INFO] Imputed dataset saved to: " << output_path << endl;
}

void Dataset::exportCorrelationMatrix(const vector<vector<double>>& matrix, const string& out_path) {
    // Create output folder if needed
    struct stat st = {0};
    size_t slash_pos = out_path.find_last_of("/\\");
    string directory = (slash_pos != string::npos) ? out_path.substr(0, slash_pos) : ".";

    if (stat(directory.c_str(), &st) == -1) {
    #ifdef _WIN32
        _mkdir(directory.c_str());
    #else
        mkdir(directory.c_str(), 0700);
    #endif
    }

    // Write matrix to file
    ofstream out(out_path);
    if (!out.is_open()) {
        cerr << "[ERROR] Cannot write correlation matrix to: " << out_path << endl;
        return;
    }

    for (const auto& row : matrix) {
        for (size_t i = 0; i < row.size(); ++i) {
            out << (std::isnan(row[i]) ? "nan" : to_string(row[i]));
            if (i < row.size() - 1) out << ",";
        }
        out << "\n";
    }

    out.close();
    cout << "[INFO] Correlation matrix saved to: " << out_path << endl;
}


// Getters and helpers
int Dataset::numMissingValues() {
    return missing_row_indices.size();
}

vector<vector<double>> Dataset::getCleanData() const { return clean_data; }
vector<vector<double>> Dataset::getRawData() const { return raw_data; }
vector<vector<double>> Dataset::getCorrelationMatrix() const { return correlation_matrix; }

double Dataset::getMinAttributeAt(int pos) const { return min_attributes[pos]; }
double Dataset::getMaxAttributeAt(int pos) const { return max_attributes[pos]; }
double Dataset::getMeanAttributeAt(int pos) const { return mean_attributes[pos]; }
int Dataset::getMissingRowAt(int pos) const { return missing_row_indices[pos]; }
int Dataset::getMissingColAt(int pos) const { return missing_col_indices[pos]; }

vector<double> Dataset::getMinAttributes() const { return min_attributes; }
vector<double> Dataset::getMaxAttributes() const { return max_attributes; }
vector<double> Dataset::getMeansAttributes() const { return mean_attributes; }

vector<int> Dataset::getMissingRows() const { return missing_row_indices; }
vector<int> Dataset::getMissingCols() const { return missing_col_indices; }

bool Dataset::isMissingValue(const string& val) const {
    string v = val;
    transform(v.begin(), v.end(), v.begin(), ::tolower);
    return v.empty() || v == "nan" || v == "null" || v == "n/a" || v == "na";
}
