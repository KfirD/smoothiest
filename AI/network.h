#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "network_utils.h"

class Network {
public:
    Network(int num_in, int num_out);
    bool connect(int in, int out, double weight);
    bool disconnect(int in, int out);
    int add_new_neuron();
    void mutate();
    std::vector<double> evaluate(const std::vector<double> &inputs);

    const Neurons &get_neurons() const;
    Neurons &get_neurons();
    const Connections &get_connections() const;
    Connections &get_connections();
    Connection &get_random_connection();
    Neuron &get_random_neuron();
    Connection get_random_unconnection();
private:
    // Data
    int num_in;
    int num_out;
    int neuron_count;
    Neurons neurons;
    Connections connections;

    // Methods
    void set_input_neurons(const std::vector<double> &inputs);
};

std::ostream &operator<<(std::ostream &out, const Network &concs);

#endif
