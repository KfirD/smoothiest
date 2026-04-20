#include "neat_evaluator.h"

int evaluateOrganism(Organism* org, NEATEvaluator* env, int maxSteps) {
    env->reset();
    int steps = 0;
    while(!env->isDone() && steps < maxSteps) {
        std::vector<double> state = env->getState();
        int action = org->act(state);
        env->step(action);
        steps++;
    }
    return steps;
}
