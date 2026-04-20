#ifndef ORGANISM_H
#define ORGANISM_H

#include "network.h"

class Organism {
public:
    Organism(int numIn, int numOut);
    Organism(const Network& n);
    Organism(const Organism& other);

    Network& getNetwork() { return network; }
    const Network& getNetwork() const { return network; }

    double getFitness() const { return fitness; }
    void setFitness(double f) { fitness = f; }

    int getSpeciesId() const { return speciesId; }
    void setSpeciesId(int id) { speciesId = id; }

    double getSharedFitness() const { return sharedFitness; }
    void setSharedFitness(double f) { sharedFitness = f; }

    std::vector<double> forward(const std::vector<double>& inputs) {
        return network.forward(inputs);
    }

    int act(const std::vector<double>& state);

private:
    Network network;
    double fitness;
    double sharedFitness;
    int speciesId;
};

#endif
