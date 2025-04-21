#ifndef DATASET_H
#define DATASET_H

#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class Dataset {
public:
    Dataset(const string& filepath);
    
    void load();
    void computeCorrelationMatrix();
    int numMissingValues();

    vector<vector<double>> getCleanData() const;
    vector<vector<double>> getRawData() const;
    vector<vector<double>> getCorrelationMatrix() const;

    double getMinAttributeAt(int pos) const;
    double getMaxAttributeAt(int pos) const;
    double getMeanAttributeAt(int pos) const;

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

    bool isMissingValue(const string& val) const;
};

#endif
