
#ifndef TRAINER_H
#define TRAINER_H

#include "population.h"

class Trainer {
public:
    Trainer();
    void train(int generations);
private:
    Population pop;
};

#endif
