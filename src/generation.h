#ifndef GENERATION_H
#define GENERATION_H

#include <vector>
#include <memory>
#include "organism.h"
#include "species.h"

class Generation {
public:
    Generation(int popSize, int numInputs, int numOutputs);
    
    void evaluateAll();
    void evolve();
    
    int getGenerationNumber() const { return generationNumber; }
    double getBestFitness() const { return bestFitness; }
    double getAverageFitness() const { return averageFitness; }
    
    Organism* getBestOrganism() const { return bestOrganism; }
    const std::vector<std::unique_ptr<Organism>>& getOrganisms() const { return organisms; }
    
    void setEvaluateFunction(std::function<void(Organism*)> evalFunc) {
        evaluateFunc = evalFunc;
    }

private:
    void speciate();
    void reproduce();
    
public:
    void computeFitness();
    
    int generationNumber;
    int popSize;
    int numInputs;
    int numOutputs;
    
    std::vector<std::unique_ptr<Organism>> organisms;
    std::vector<std::unique_ptr<Species>> species;
    
    double bestFitness;
    double averageFitness;
    Organism* bestOrganism;
    
    std::function<void(Organism*)> evaluateFunc;
    
    static constexpr double COMPATIBILITY_THRESHOLD = 2.0;
    static constexpr double ELITE_PERCENTAGE = 0.40;
    static constexpr int STAGNATION_THRESHOLD = 15;
    static constexpr double INTER_SPECIES_MATE_RATE = 0.001;
};

#endif
