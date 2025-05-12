#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <string>

using namespace std;

class Utils;  // Forward declaration

class Dataset {
public:
    explicit Dataset(const string& filepath);

    // Loads data from the CSV file and processes it
    void load();

    // Export data
    void exportImputedData(const vector<double>& imputed_values, const string& output_path);

    // Export Correlations Matrix
    void exportCorrelationMatrix(const vector<vector<double>>& matrix, const string& out_path);

    // Returns number of missing values in the dataset
    int numMissingValues();

    // Getters
    vector<vector<double>> getCleanData() const;
    vector<vector<double>> getRawData() const;
    vector<vector<double>> getCorrelationMatrix() const;

    double getMinAttributeAt(int pos) const;
    double getMaxAttributeAt(int pos) const;
    double getMeanAttributeAt(int pos) const;
    int getMissingRowAt(int pos) const;
    int getMissingColAt(int pos) const;

    vector<double> getMinAttributes() const;
    vector<double> getMaxAttributes() const;
    vector<double> getMeansAttributes() const;

    vector<int> getMissingRows() const;
    vector<int> getMissingCols() const;

private:
    string filepath;
    vector<vector<double>> raw_data;
    vector<vector<double>> clean_data;
    vector<vector<double>> correlation_matrix;

    vector<double> min_attributes;
    vector<double> max_attributes;
    vector<double> mean_attributes;

    vector<int> missing_row_indices;
    vector<int> missing_col_indices;

    // Helper to detect missing tokens like "", "nan", etc.
    bool isMissingValue(const string& val) const;
};

#endif
