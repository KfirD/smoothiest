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

/* Neuron */
class Neuron {
public:
    // Data
    int activation_id;

    // Methods
   Neuron(int id);
   int get_id() const;
   void add_input(int new_in);
   void add_output(int new_out);
private:
   int id;
   vector<int> inputs;
   vector<int> outputs;
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

   void set_weight(double new_weight);
   void enable();
   void disable();

private:
   const int id;
   const int in;
   const int out;
   double weight;
};


using Neurons = vector<Neuron>;
using Connections = vector<Connection>;


#endif
