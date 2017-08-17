
#include <iostream>
#include <vector>

#include "species.h"

std::ostream &operator<<(std::ostream &out, const Organism &o)
{
    out << "Species: " << o.species_index;
    return out;
}

Species::Species()
{

}

void Species::add(Organism &organism)
{
    organisms.push_back(&organism);
}

std::vector<Organism *> &Species::get_organisms()
{
    return organisms;
}

std::ostream &operator<<(std::ostream &out, const Species &s)
{
    out << "Organism count: " << s.organisms.size() << std::endl;
    for (const Organism *o : s.organisms) {
        out << o << std::endl;
    }
    return out;
}
