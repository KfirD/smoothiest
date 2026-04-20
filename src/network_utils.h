#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

class Neuron;
class Connection;

using Neurons = vector<Neuron>;
using Connections = vector<Connection>;

class Neuron {
public:
    int id;
    int activation_id;

    Neuron(int nid);
    Neuron(int nid, double override_value);
    int get_id() const { return id; }
    double get_activation() const { 
        if(override_flag) return override_value;
        return activation;
    }
    void set_override_value(double val) { override_value = val; override_flag = true; }
    void clear_override() { override_flag = false; }
    void add_input_value(double val) { input_sum += val; }
    void reset_activation() { input_sum = 0.0; activation = 0.0; }
    void restore_override() { override_flag = true; }
    void compute_activation();
    
    const vector<int>& get_inputs() const { return inputs; }
    const vector<int>& get_outputs() const { return outputs; }
    void add_input(int new_in) { inputs.push_back(new_in); }
    void add_output(int new_out) { outputs.push_back(new_out); }

private:
    double override_value;
    bool override_flag;
    double input_sum;
    double activation;
    vector<int> inputs;
    vector<int> outputs;
};

class Connection {
public:
    int id;
    int in;
    int out;
    bool enabled;
    double weight;
    int innovation;

    Connection(int nin, int nout, double w, int inn);
    int get_id() const { return id; }
    int get_in() const { return in; }
    int get_out() const { return out; }
    double get_weight() const { return weight; }
    int get_innovation() const { return innovation; }

    void set_weight(double new_weight) { weight = new_weight; }
    void enable() { enabled = true; }
    void disable() { enabled = false; }
};

int cantor(int x, int y);

#endif
