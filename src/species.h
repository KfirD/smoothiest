#ifndef SPECIES_H
#define SPECIES_H

#include <vector>
#include <memory>
#include "organism.h"

class Species {
public:
    Species(int id, Organism* representative);
    
    int getId() const { return id; }
    void addOrganism(Organism* org);
    const std::vector<Organism*>& getOrganisms() const { return organisms; }
    
    double getBestFitness() const { return bestFitness; }
    void setBestFitness(double f) { bestFitness = f; }
    
    int getGenerationsWithoutImprovement() const { return gensWithoutImprovement; }
    void incrementGensWithoutImprovement() { gensWithoutImprovement++; }
    void resetGensWithoutImprovement() { gensWithoutImprovement = 0; }

    Organism* getBestOrganism() const;

    void computeSharedFitness();
    void clear();

    static double compatibility(const Organism* a, const Organism* b);

private:
    int id;
    std::vector<Organism*> organisms;
    double bestFitness;
    int gensWithoutImprovement;
};

#endif
