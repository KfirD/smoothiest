#include <iostream>

#include "network.h"
#include "network_utils.h"

using namespace std;

int main() {

   //input
   Neurons inputs = {Neuron(0), Neuron(2)};
   Neurons outputs = {Neuron(1), Neuron(4)};

   Neuron keiths_only_neuron(1);
   cout << keiths_only_neuron.get_id();

   Network n(inputs, outputs);
   n.connect(inputs.front(), outputs.front(), 0);
   cout << n << "\n";

   return 0;
}
