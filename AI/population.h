
#ifndef POPULATION_H
#define POPULATION_H

#include <list>

#include "generation.h"
#include "innovation_tracker.h"
#include "network.h"
#include "species.h"

using Networks = std::vector<Network>;
using Rank = std::pair<Network, double>;

class Population {
public:
    Population(int num_inputs, int num_outputs, int size,
        double (*_feedback)(vector<double>&, vector<double>&));

    void run_generation();
    Organism &get_best_organism();
private:
    // Data
    const int num_inputs;
    const int num_outputs;
    const int pop_size;
    double (*feedback)(vector<double>&, vector<double>&);
    Generation current_generation;
    InnovationTracker innovation_tracker;
};

#endif
