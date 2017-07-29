#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <iostream>
#include <vector>

using namespace std;

/*
class Trigger_Function {
   int id;
};
*/

class Neuron {
public:
   Neuron(int id);
   int get_id() const;
private:
   int id;
   //Trigger_Function trigger;
};


class Connection {
public:
   Connection(Neuron &src, Neuron &dst, double weight);
   int get_id() const;
   Neuron& get_src();
   Neuron& get_dst();
   double get_weight();

private:
   int id;
   Neuron& src;
   Neuron& dst;
   double weight;
};


using Neurons = vector<Neuron>;
using Connections = vector<Connection>;


#endif
