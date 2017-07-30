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
    const int id;
    int activation_id;

    // Methods
   Neuron(int id);
   Neuron(int id, double override_value);
   int get_id() const;
   double evaluate() const;
private:
   double override_value;
   bool override_flag;
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
