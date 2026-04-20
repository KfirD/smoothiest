#ifndef NEAT_EVALUATOR_H
#define NEAT_EVALUATOR_H

#include <vector>
#include "organism.h"

class NEATEvaluator {
public:
    virtual ~NEATEvaluator() = default;
    
    virtual std::vector<double> getState() const = 0;
    virtual void step(int action) = 0;
    virtual bool isDone() const = 0;
    virtual void reset() = 0;
    virtual int getSteps() const = 0;
    
    virtual const char* getName() const = 0;
    virtual int getNumInputs() const = 0;
    virtual int getNumOutputs() const = 0;
};

int evaluateOrganism(Organism* org, NEATEvaluator* env, int maxSteps);

#endif
