#include "Dataset.h"

Dataset::Dataset(const string& filepath) : filepath(filepath) {}

void Dataset::load() {
    ifstream file(filepath);
    string line;
    Utils utils;

    bool first_line = true;
    vector<vector<string>> raw_lines;

    // Paso 1: leer todas las líneas y determinar el máximo de columnas
    int expected_columns = 0;
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

        if (row.size() > expected_columns) {
            expected_columns = row.size();
        }

        raw_lines.push_back(row);
    }

    // Paso 2: procesar todas las líneas
    int row_index = 0;
    for (const auto& string_row : raw_lines) {
        vector<double> parsed_row;
        bool has_missing = false;

        for (int col_index = 0; col_index < expected_columns; ++col_index) {
            if (col_index >= string_row.size() || isMissingValue(string_row[col_index])) {
                parsed_row.push_back(numeric_limits<double>::quiet_NaN());
                missing_row_indices.push_back(row_index);
                missing_col_indices.push_back(col_index);
                has_missing = true;
            } else {
                try {
                    parsed_row.push_back(stod(string_row[col_index]));
                } catch (...) {
                    cerr << "[ERROR] Invalid numeric value at row " << row_index << ", col " << col_index << endl;
                    exit(1);
                }
            }
        }

        raw_data.push_back(parsed_row);
        if (!has_missing) {
            clean_data.push_back(parsed_row);
        }

        row_index++;
    }
    
    if (!clean_data.empty()) {
        int cols = clean_data[0].size();
        min_attributes.resize(cols);
        max_attributes.resize(cols);
        mean_attributes.resize(cols);

        for (int i = 0; i < cols; ++i) {
            min_attributes[i] = utils.getMinValue(clean_data[i]);
            max_attributes[i] = utils.getMaxValue(clean_data[i]);
            mean_attributes[i] = utils.calculateMean(clean_data[i]);
        }

        correlation_matrix = vector<vector<double>>(cols, vector<double>(cols, 1.0));
        utils.computeCorrelationMatrix(correlation_matrix, clean_data, mean_attributes);
    } else {
        cerr << "[ERROR] No valid data." << endl;
        exit(1);
    }
}

int Dataset::numMissingValues() {
    return missing_row_indices.size();
}

vector<vector<double>> Dataset::getCleanData() const {
    return clean_data;
}

vector<vector<double>> Dataset::getRawData() const { 
    return raw_data; 
}

vector<vector<double>> Dataset::getCorrelationMatrix() const {
    return correlation_matrix;
}

double Dataset::getMinAttributeAt(int pos) const {
    return min_attributes[pos];
}

double Dataset::getMaxAttributeAt(int pos) const {
    return max_attributes[pos];
}

double Dataset::getMeanAttributeAt(int pos) const {
    return mean_attributes[pos];
}

vector<double> Dataset::getMinAttributes() const {
    return min_attributes;
}

vector<double> Dataset::getMaxAttributes() const {
    return max_attributes;
}

vector<double> Dataset::getMeansAttributes() const {
    return mean_attributes;
}

vector<int> Dataset::getMissingRows() const { 
    return missing_row_indices; 
}
    
vector<int> Dataset::getMissingCols() const { 
    return missing_col_indices;
}

bool Dataset::isMissingValue(const string& val) const {
    string v = val;
    transform(v.begin(), v.end(), v.begin(), ::tolower);
    return v.empty() || v == "nan" || v == "null" || v == "n/a" || v == "na";
}







