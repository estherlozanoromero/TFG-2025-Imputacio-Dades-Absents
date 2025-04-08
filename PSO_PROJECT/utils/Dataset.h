#ifndef DATASET_H
#define DATASET_H

#include <vector>
#include <string>

using namespace std;

class Dataset {
public:
    Dataset(const string& filepath);
    
    void load();
    vector<vector<double>> getCleanData() const;
    vector<vector<double>> computeCorrelationMatrix() const;
    int countMissingValues() const;
    vector<vector<string>> getRawData() const;
    double getMinAttributeAt(int pos) const;
    double getMaxAttributeAt(int pos) const;
    vector<double> getMinAttributes() const;
    vector<double> getMaxAttributes() const;
    
private:
    string filepath;
    vector<vector<string>> raw_data;
    vector<vector<double>> clean_data;
    vector<double> min_attributes;
    vector<double> max_attributes;
    
    bool hasMissing(const vector<string>& row) const;
    double correlation(int col1, int col2) const;
};

#endif
