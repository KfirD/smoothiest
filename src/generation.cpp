#include "generation.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cmath>
#include <cstdlib>

Generation::Generation(int pSize, int nIn, int nOut)
    : generationNumber(0), popSize(pSize), 
      numInputs(nIn), numOutputs(nOut),
      bestFitness(0.0), averageFitness(0.0), bestOrganism(nullptr) {
    
    for(int i = 0; i < popSize; i++) {
        organisms.push_back(std::unique_ptr<Organism>(new Organism(numInputs, numOutputs)));
    }
}

void Generation::evaluateAll() {
    for(auto& org : organisms) {
        if(evaluateFunc) {
            evaluateFunc(org.get());
        }
    }
    computeFitness();
    speciate();
}

void Generation::computeFitness() {
    double total = 0.0;
    bestFitness = 0.0;
    bestOrganism = nullptr;
    
    for(auto& org : organisms) {
        double f = org->getFitness();
        total += f;
        if(f > bestFitness) {
            bestFitness = f;
            bestOrganism = org.get();
        }
    }
    
    averageFitness = total / organisms.size();
}

void Generation::speciate() {
    for(auto& sp : species) {
        sp->clear();
    }
    
    int speciesCounter = 0;
    
    for(auto& org : organisms) {
        bool placed = false;
        
        for(auto& sp : species) {
            if(!sp->getOrganisms().empty()) {
                Organism* rep = sp->getOrganisms().front();
                double dist = Species::compatibility(org.get(), rep);
                
                if(dist < COMPATIBILITY_THRESHOLD) {
                    sp->addOrganism(org.get());
                    placed = true;
                    break;
                }
            }
        }
        
        if(!placed) {
            std::unique_ptr<Species> newSpecies(new Species(speciesCounter++, org.get()));
            species.push_back(std::move(newSpecies));
        }
    }
    
    for(auto& sp : species) {
        sp->computeSharedFitness();
        
        if(sp->getOrganisms().size() > 5) {
            Organism* champ = sp->getBestOrganism();
            if(champ->getFitness() > sp->getBestFitness()) {
                sp->resetGensWithoutImprovement();
            } else {
                sp->incrementGensWithoutImprovement();
            }
        }
    }
}

void Generation::evolve() {
    std::vector<std::unique_ptr<Organism>> newOrganisms;
    
    std::vector<Organism*> allElites;
    for(auto& sp : species) {
        auto& orgs = sp->getOrganisms();
        if(!orgs.empty() && sp->getGenerationsWithoutImprovement() < Generation::STAGNATION_THRESHOLD) {
            std::vector<Organism*> orgVec = orgs;
            std::sort(orgVec.begin(), orgVec.end(), 
                [](Organism* a, Organism* b) { return a->getFitness() > b->getFitness(); });
            
            int eliteCount = std::max(1, (int)(orgVec.size() * ELITE_PERCENTAGE));
            for(int i = 0; i < eliteCount && i < (int)orgVec.size(); i++) {
                allElites.push_back(orgVec[i]);
            }
            
            if(orgs.size() > 5) {
                Organism* champ = sp->getBestOrganism();
                newOrganisms.push_back(std::unique_ptr<Organism>(new Organism(champ->getNetwork())));
            }
        }
    }
    
    for(Organism* elite : allElites) {
        newOrganisms.push_back(std::unique_ptr<Organism>(new Organism(elite->getNetwork())));
    }
    
    while((int)newOrganisms.size() < popSize) {
        double totalFitness = 0.0;
        for(auto& sp : species) {
            if(sp->getGenerationsWithoutImprovement() >= STAGNATION_THRESHOLD) continue;
            for(Organism* org : sp->getOrganisms()) {
                totalFitness += org->getSharedFitness();
            }
        }
        
        if(totalFitness <= 0) {
            newOrganisms.push_back(std::unique_ptr<Organism>(new Organism(numInputs, numOutputs)));
            continue;
        }
        
        double roll = (rand() / (double)RAND_MAX) * totalFitness;
        Organism* parent1 = nullptr;
        
        for(auto& sp : species) {
            if(sp->getGenerationsWithoutImprovement() >= STAGNATION_THRESHOLD) continue;
            for(Organism* org : sp->getOrganisms()) {
                roll -= org->getSharedFitness();
                if(roll <= 0) {
                    parent1 = org;
                    break;
                }
            }
            if(parent1) break;
        }
        
        if(!parent1) {
            if(!allElites.empty()) {
                parent1 = allElites[rand() % allElites.size()];
            } else {
                newOrganisms.push_back(std::unique_ptr<Organism>(new Organism(numInputs, numOutputs)));
                continue;
            }
        }
        
        Organism* parent2 = nullptr;
        bool interSpecies = (rand() / (double)RAND_MAX) < INTER_SPECIES_MATE_RATE;
        
        if(interSpecies && allElites.size() > 1) {
            int attempts = 0;
            while(attempts < 10) {
                Organism* candidate = allElites[rand() % allElites.size()];
                if(candidate->getSpeciesId() != parent1->getSpeciesId()) {
                    parent2 = candidate;
                    break;
                }
                attempts++;
            }
        }
        
        std::unique_ptr<Network> childNet;
        if(parent2) {
            childNet.reset(new Network(Network::crossover(parent1->getNetwork(), parent2->getNetwork())));
        } else {
            childNet.reset(new Network(parent1->getNetwork()));
        }
        
        childNet->mutate(0.03, 0.05, 0.80);
        newOrganisms.push_back(std::unique_ptr<Organism>(new Organism(*childNet)));
    }
    
    organisms = std::move(newOrganisms);
    generationNumber++;
    
    bestFitness = 0.0;
    averageFitness = 0.0;
    bestOrganism = nullptr;
}
