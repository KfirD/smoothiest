
#include <iostream>
#include <vector>

#include "species.h"

std::ostream &operator<<(std::ostream &out, const Organism &o)
{
    out << "Species: " << o.species_name;
    return out;
}

Species::Species() {}

void Species::add(int organism_index) { organisms.push_back(organism_index); }

std::vector<int> &Species::get_organism_indexes() { return organisms; }
const std::vector<int> &Species::get_organism_indexes() const { return organisms; }
