
#include "species.h"
#include <algorithm>
#include <cmath>

Species::Species(int id, Organism* representative) 
    : id(id), bestFitness(0.0), gensWithoutImprovement(0) {
    organisms.push_back(representative);
    representative->setSpeciesId(id);
    bestFitness = representative->getFitness();
}

void Species::addOrganism(Organism* org) {
    organisms.push_back(org);
    org->setSpeciesId(id);
    if(org->getFitness() > bestFitness) {
        bestFitness = org->getFitness();
        resetGensWithoutImprovement();
    }
}

Organism* Species::getBestOrganism() const {
    if(organisms.empty()) return nullptr;
    return *std::max_element(organisms.begin(), organisms.end(),
        [](Organism* a, Organism* b) { return a->getFitness() < b->getFitness(); });
}

void Species::computeSharedFitness() {
    double sum = 0.0;
    for(Organism* org : organisms) {
        double adjusted = org->getFitness() / organisms.size();
        org->setSharedFitness(adjusted);
        sum += adjusted;
    }
}

void Species::clear() {
    organisms.clear();
}

double Species::compatibility(const Organism* a, const Organism* b) {
    const Network& na = a->getNetwork();
    const Network& nb = b->getNetwork();
    
    const Connections& ca = na.getConnections();
    const Connections& cb = nb.getConnections();
    
    int maxConn = std::max(ca.size(), cb.size());
    if(maxConn == 0) return 0.0;
    
    int matching = 0;
    int excess = 0;
    double weightDiff = 0.0;
    int matchingCount = 0;
    
    for(const auto& caConn : ca) {
        bool found = false;
        for(const auto& cbConn : cb) {
            if(caConn.get_id() == cbConn.get_id()) {
                found = true;
                weightDiff += std::abs(caConn.get_weight() - cbConn.get_weight());
                matchingCount++;
                break;
            }
        }
        if(found) {
            matching++;
        } else {
            excess++;
        }
    }
    
    for(const auto& cbConn : cb) {
        bool found = false;
        for(const auto& caConn : ca) {
            if(caConn.get_id() == cbConn.get_id()) {
                found = true;
                break;
            }
        }
        if(!found) {
            excess++;
        }
    }
    
    double avgWeightDiff = matchingCount > 0 ? weightDiff / matchingCount : 0.0;
    
    double c1 = 1.0;
    double c2 = 1.0;
    double c3 = 0.4;
    
    double delta = (c1 * excess) / maxConn + (c2 * (maxConn - matching)) / maxConn + c3 * avgWeightDiff;
    
    return delta;
}
