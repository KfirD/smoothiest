#ifndef POPULATION_H
#define POPULATION_H

#include <list>

#include "generation.h"
#include "innovation_tracker.h"
#include "network.h"

using Networks = std::vector<Network>;
using Rank = std::pair<Network, double>;

class Population {
public:
    Population(int num_inputs, int num_outputs, int size,
        double (*_feedback)(vector<double>&, vector<double>&));

    std::vector<Rank> ranks;

    int get_size() const;
    void run_generation();
    double get_average_fitness() const;
    void reset_fitnesses();
    void evaluate_fitness(int num_times);
    void replace_inferior_population(double percentage);
    Network &get_best_network();
    vector<double> generate_random_input() const;
private:
    // Data
    const int num_inputs;
    const int num_outputs;
    const int pop_size;
    double (*feedback)(vector<double>&, vector<double>&);
    Generation current_generation;
    InnovationTracker innovation_tracker;

    // Methods
    Network &get_random_network();
};

Network breed(const Rank &rank1, const Rank &rank2);

#endif
