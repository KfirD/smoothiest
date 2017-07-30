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
   Connection(int in, int out, double weight);
   int get_id() const;
   bool enabled;
   int get_in() const;
   int get_out() const;
   double get_weight() const;

private:
   const int id;
   const int in;
   const int out;
   double weight;
};


using Neurons = vector<Neuron>;
using Connections = vector<Connection>;


#endif
