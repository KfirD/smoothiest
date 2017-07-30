#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <iostream>
#include <vector>

using namespace std;

class Neuron;
class Connection;

using Neurons = vector<Neuron>;
using Connections = vector<Connection>;

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
   double evaluate(const Neurons &neurons) const;
   void add_input(int new_in);
   void add_output(int new_out);
private:
   double override_value;
   bool override_flag;
   vector<int> inputs;
   vector<int> outputs;
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

#endif
