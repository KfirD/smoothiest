
#ifndef GENERATION_H
#define GENERATION_H

#include <iostream>
#include <vector>

#include "innovation_tracker.h"
#include "network.h"
#include "species.h"

class Generation {
public:
    Generation(int input_count, int output_count, int pop_size, double (*_feedback)(vector<double>&, vector<double>&));
    Generation create_new_generation();
    bool are_same_species(const Network &n1, const Network &n2) const;

    friend std::ostream &operator<<(std::ostream &out, const Generation &gen);
private:
    // Data
    int input_count;
    int output_count;
    int population_size;
    int species_counter;
    double (*fitness_function)(vector<double>&, vector<double>&);
    std::vector<Organism> organisms;
    std::unordered_map<int, Species> species;
    InnovationTracker innovation_tracker;

    // Methods
    void speciate_organisms();
    void resize_species();
    void populate_species();
    vector<double> generate_random_input() const;
    Network breed(const Organism &organism1, const Organism &organism2);
};

#endif
