#include "pso/PSO.h"
#include <ctime>
#include <stdlib.h>

int main() {
    srand(time(0));

    // Config params
    int num_particles = 30;
    int dimensions = 20;
    int max_iterations = 100;
    double c1 = 1.5, c2 = 1.5, w = 0.7;
    double lower = 0.0, upper = 100.0;

    PSO pso(num_particles, dimensions, max_iterations, c1, c2, w, lower, upper);
    pso.run();

    return 0;
}
