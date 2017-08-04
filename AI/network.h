#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "network_utils.h"

using namespace std;

class Network {
public:
    Network(int num_in, int num_out);
    bool connect(int in, int out, double weight);
    int add_new_neuron();
    void mutate();
    std::vector<double> evaluate(const std::vector<double> &inputs);

    const Neurons &get_neurons() const;
    Neurons &get_neurons();
    const Connections &get_connections() const;
    Connections &get_connections();
    const std::unordered_map<int, int> &get_connection_map() const;
    std::unordered_map<int, int> &get_connection_map();
    Connection &get_random_connection();
    Neuron &get_random_neuron();
    Connection get_random_unconnection();
private:
    // Data
    const int num_in;
    const int num_out;
    int neuron_count;
    Neurons neurons;
    Connections connections;
    std::unordered_map<int, int> connection_map;

    // Methods
    void set_input_neurons(const std::vector<double> &inputs);
};

std::ostream &operator<<(std::ostream &out, const Network &concs);

#endif
