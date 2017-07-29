#include <iostream>

#include "network.h"
#include "network_utils.h"

using namespace std;

int main() {
   //input
   Neurons inputs = {Neuron(0), Neuron(2)};
   Neurons outputs = {Neuron(1), Neuron(4)};

   Network n(inputs, outputs);
   n.connect(inputs.front(), outputs.front(), 0);
   cout << n << "\n";

   /*Connection edge(one, two, 0.5);

   cout << edge.get_id() << "\n";
   cout << edge.get_weight() << "\n";
   cout << edge.get_src().get_id() << "\n";
   cout << edge.get_dst().get_id() << "\n";*/


   return 0;
}
