
#ifndef SPECIES_H
#define SPECIES_H

#include <iostream>
#include <vector>

#include "network.h"

class Species;

struct Organism {
    int species_index;
    Network network;
};

std::ostream &operator<<(std::ostream &out, const Organism &o);

class Species {
public:
    Species();
    void add(Organism &organism);
    std::vector<Organism *> &get_organisms();

    friend std::ostream &operator<<(std::ostream &out, const Species &s);
private:
    std::vector<Organism *> organisms;
};

#endif
