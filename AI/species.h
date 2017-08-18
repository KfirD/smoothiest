
#ifndef SPECIES_H
#define SPECIES_H

#include <iostream>
#include <vector>

#include "network.h"

class Species;

struct Organism {
    int species_name;
    double fitness;
    Network network;

    static const int UnspeciedOrganism = -1;
};

std::ostream &operator<<(std::ostream &out, const Organism &o);

class Species {
public:
    // Data
    int size;

    // Methods
    Species();
    void add(int organism_index);
    std::vector<int> &get_organism_indexes();
    const std::vector<int> &get_organism_indexes() const;
private:
    std::vector<int> organisms;
};

#endif
