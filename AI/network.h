#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>

#include "network_utils.h"

using namespace std;

class Network {
public:
   Network(Neurons& input, Neurons& output);
   Connections& get_connections();
   bool connect(Neuron &src, Neuron &dst, double weight);

private:
   Neurons inner_neurons;
   Connections connections;

   Neurons& input;
   Neurons& output;

};

std::ostream &operator<<(std::ostream &out, Network &coord);

#endif
