#include "pso/PSO.h"
#include "utils/Initializer.h"
#include "utils/Dataset.h"
#include "utils/Utils.h"

#include <ctime>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#ifdef _WIN32
#include <direct.h>  // for _mkdir on Windows
#else
#include <sys/stat.h>  // for mkdir on Unix/Linux
#include <sys/types.h>
#include <unistd.h>
#endif

using namespace std;
using namespace std::chrono;

// Parses command-line arguments into key-value map
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

// Creates a directory if it does not already exist (cross-platform)
void ensureDirectory(const string& dir) {
#ifdef _WIN32
    _mkdir(dir.c_str());  // Windows
#else
    struct stat st = {0};
    if (stat(dir.c_str(), &st) == -1) {
        mkdir(dir.c_str(), 0700);  // Linux/Unix
    }
#endif
}

int main(int argc, char* argv[]) {
    auto args = parseArgs(argc, argv);

    if (args.find("-d") == args.end()) {
        cerr << "Usage: " << argv[0] << " -d <dataset.csv> "
             << "[-p <particles>] [-m <max_iterations>] "
             << "[-c1 <c1>] [-c2 <c2>] [-w <inertia>] "
             << "[-r <ratio_random>] [-i <init_type 0|1|2>] [-a <active_attributes>]" << endl;
        return 1;
    }

    // Seed random generator
    srand(static_cast<unsigned int>(time(0)));

    // Parse parameters with default values
    string dataset_path   = args["-d"];
    int num_particles     = args.count("-p") ? stoi(args["-p"]) : 30;
    int max_iterations    = args.count("-m") ? stoi(args["-m"]) : 50;
    double c1             = args.count("-c1") ? stod(args["-c1"]) : 1.5;
    double c2             = args.count("-c2") ? stod(args["-c2"]) : 1.5;
    double w              = args.count("-w") ? stod(args["-w"]) : 0.7;
    double ratio_random   = args.count("-r") ? stod(args["-r"]) : 0.5;
    int init_type         = args.count("-i") ? stoi(args["-i"]) : 2;
    int active_attributes = args.count("-a") ? stoi(args["-a"]) : 0;

    // Extract filename base (remove path and extension)
    string base_name = dataset_path.substr(dataset_path.find_last_of("/\\") + 1);
    string base_no_ext = base_name.substr(0, base_name.find_last_of('.'));

    // Encode tuning parameters into a suffix for output filenames
    string suffix = "_p" + to_string(num_particles) +
                    "_m" + to_string(max_iterations) +
                    "_c1" + to_string(static_cast<int>(c1 * 10)) +
                    "_c2" + to_string(static_cast<int>(c2 * 10)) +
                    "_w" + to_string(static_cast<int>(w * 10)) +
                    "_r" + to_string(static_cast<int>(ratio_random * 10)) +
                    "_i" + to_string(init_type);

    // Ensure required output directories exist
    ensureDirectory("dataset_out");
    ensureDirectory("output_correlations");
    ensureDirectory("results");

    // Define output file paths
    string out_csv          = "dataset_out/" + base_no_ext + suffix + "_imputed.csv";
    string out_corr_orig    = "output_correlations/" + base_no_ext + suffix + "_corr_original.csv";
    string out_corr_imputed = "output_correlations/" + base_no_ext + suffix + "_corr_imputed.csv";
    string result_summary   = "results/summary.csv";

    // Start measuring execution time
    auto start_time = high_resolution_clock::now();

    // Load and process dataset
    Dataset dataset(dataset_path);
    dataset.load();

    int missing = dataset.numMissingValues();
    const auto& clean_data = dataset.getCleanData();
    int dimensions = missing;

    cout << "[INFO] Missing values found: " << missing << endl;
    cout << "[INFO] Rows with no missing values: " << clean_data.size() << endl;
    cout << "[INFO] Correlation matrix ["
         << dataset.getCorrelationMatrix().size() << "x"
         << dataset.getCorrelationMatrix().size() << "] computed." << endl;

    // Select initializer based on flag
    Initializer* initializer = nullptr;
    switch (init_type) {
        case 0:
            cout << "[INFO] Using RandomInitializer\n";
            initializer = new RandomInitializer();
            break;
        case 1:
            cout << "[INFO] Using BoundedRandomInitializer\n";
            initializer = new BoundedRandomInitializer(dataset);
            break;
        case 2:
            cout << "[INFO] Using MeanRandomInitializer (ratio = " << ratio_random << ")\n";
            initializer = new MeanRandomInitializer(dataset, ratio_random);
            break;
        default:
            cerr << "[ERROR] Invalid initializer type. Use 0, 1 or 2.\n";
            return 1;
    }

    // Run Particle Swarm Optimization
    PSO pso(num_particles, dimensions, max_iterations, c1, c2, w, initializer, dataset, active_attributes);
    pso.run();

    // Stop measuring time
    auto end_time = high_resolution_clock::now();
    double duration_sec = duration_cast<duration<double>>(end_time - start_time).count();

    // Export final imputed dataset
    dataset.exportImputedData(pso.getGlobalBestPosition(), out_csv);

    // Export correlation matrices
    dataset.exportCorrelationMatrix(dataset.getCorrelationMatrix(), out_corr_orig);
    dataset.exportCorrelationMatrix(pso.getBestCorrelationMatrix(), out_corr_imputed);

    // Save summary statistics to CSV
    double final_fitness = pso.getBestFitness();
    ofstream summary(result_summary, ios::app);
    if (summary.is_open()) {
        summary << base_no_ext << "," << suffix << ","
                << final_fitness << "," << duration_sec << "\n";
        summary.close();
    } else {
        cerr << "[ERROR] Could not write to results/summary.csv\n";
    }

    delete initializer;
    return 0;
}
