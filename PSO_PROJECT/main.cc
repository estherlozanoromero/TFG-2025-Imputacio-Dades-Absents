#include "pso/PSO.h"
#include "utils/Initializer.h"
#include "utils/Dataset.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <map>

using namespace std;

// Simple arg parser: maps flags to values
map<string, string> parseArgs(int argc, char* argv[]) {
    map<string, string> args;
    for (int i = 1; i < argc - 1; ++i) {
        string key = argv[i];
        if (key[0] == '-' && i + 1 < argc) {
            args[key] = argv[i + 1];
            ++i;
        }
    }
    return args;
}

int main(int argc, char* argv[]) {
    auto args = parseArgs(argc, argv);

    if (args.find("-d") == args.end()) {
        cerr << "Usage: " << argv[0] << " -d <dataset.csv> [-p <particles>] [-m <max_iterations>] [-c1 <c1>] [-c2 <c2>] [-w <inertia>] [-r <ratio_random>] [-i <init_type> 1|2]" << endl;
        return 1;
    }

    srand(static_cast<unsigned int>(time(0)));

    string dataset_path = args["-d"];
    int num_particles   = args.count("-p") ? stoi(args["-p"]) : 30;
    int max_iterations  = args.count("-m") ? stoi(args["-m"]) : 100;
    double c1           = args.count("-c1") ? stod(args["-c1"]) : 1.5;
    double c2           = args.count("-c2") ? stod(args["-c2"]) : 1.5;
    double w            = args.count("-w") ? stod(args["-w"]) : 0.7;
    double ratio_random = args.count("-r") ? stod(args["-r"]) : 1.0;
    int init_type       = args.count("-i") ? stoi(args["-i"]) : 2; // default to MeanRandomInitializer

    // Load dataset
    Dataset dataset(dataset_path);
    dataset.load();

    int missing = dataset.countMissingValues();
    const auto& clean_data = dataset.getCleanData();
    int dimensions = clean_data[0].size();

    cout << "[INFO] Missing values found: " << missing << endl;
    cout << "[INFO] Rows with no missing values: " << clean_data.size() << endl;
    cout << "[INFO] Correlation matrix [" << dimensions << "x" << dimensions << "] computed.\n";

    // Calculate means, min and max per attribute
    vector<double> means(dimensions, 0.0);
    for (const auto& row : clean_data)
        for (int j = 0; j < dimensions; ++j)
            means[j] += row[j];
    for (int j = 0; j < dimensions; ++j)
        means[j] /= clean_data.size();

    const vector<double> min_values = dataset.getMinAttributes();
    const vector<double> max_values = dataset.getMaxAttributes();

    // Choose initializer
    Initializer* initializer = nullptr;
    if (init_type == 1) {
        cout << "[INFO] Using RandomInitializer\n";
        initializer = new RandomInitializer();
    } else {
        cout << "[INFO] Using MeanRandomInitializer\n";
        initializer = new MeanRandomInitializer(means, min_values, max_values, ratio_random);
    }

    // Create and run PSO
    PSO pso(num_particles, missing, max_iterations, c1, c2, w, initializer, dataset);
    pso.run();

    delete initializer;
    return 0;
}
