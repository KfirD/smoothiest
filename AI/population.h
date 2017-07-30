
#ifndef POPULATION_H
#define POPULATION_H

#include "network.h"

Network breed(const Network &network1, const Network &network2);
void mutate(Network &network);

#endif
