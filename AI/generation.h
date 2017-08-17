
#ifndef GENERATION_H
#define GENERATION_H

#include <iostream>
#include <vector>

#include "innovation_tracker.h"
#include "network.h"
#include "species.h"

class Generation {
public:
    Generation(int input_count, int output_count, int pop_size);
    Generation create_new_generation() const;
    bool are_same_species(const Network &n1, const Network &n2) const;

    friend std::ostream &operator<<(std::ostream &out, const Generation &gen);
private:
    int input_count;
    int output_count;
    int population_size;
    std::vector<Species> species;
    InnovationTracker innovation_tracker;
};

#endif
