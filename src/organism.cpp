#include "organism.h"
#include <algorithm>

Organism::Organism(int numIn, int numOut) 
    : network(numIn, numOut), fitness(0.0), sharedFitness(0.0), speciesId(-1) {}

Organism::Organism(const Network& n) 
    : network(n), fitness(0.0), sharedFitness(0.0), speciesId(-1) {}

Organism::Organism(const Organism& other)
    : network(other.network), fitness(other.fitness), 
      sharedFitness(other.sharedFitness), speciesId(other.speciesId) {}

int Organism::act(const std::vector<double>& state) {
    std::vector<double> outputs = forward(state);
    if(outputs.size() < 2) return 0;
    return outputs[0] > outputs[1] ? 0 : 1;
}
