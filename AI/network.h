#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "innovation_tracker.h"
#include "network_utils.h"

class Network {
public:
    Network(int num_in, int num_out, InnovationTracker *innovation_tracker);
    bool connect(int in, int out, double weight);
    bool connect_without_innovation(int innovation_number, int in, int out, double weight);
    bool disconnect(int in, int out);
    int add_new_neuron();
    void mutate();
    std::vector<double> evaluate(const std::vector<double> &input_values) const;
    int get_max_innovation_number() const;
    bool is_disjoint_connection(const Connection &c) const;
    bool is_excess_connection(const Connection &c) const;
    bool is_shared_connection(const Connection &c) const;
    int disjoint_count_with(const Network &n) const;
    int excess_count_with(const Network &n) const;
    double average_weight_difference_with(const Network &n) const;

    const Neurons &get_neurons() const;
    Neurons &get_neurons();
    const Connections &get_connections() const;
    Connections &get_connections();
    Connection &get_random_connection();
    Neuron &get_random_neuron();
    std::pair<int, int> get_random_unconnection();
private:
    // Data
    int num_in;
    int num_out;
    int neuron_count;
    Neurons neurons;
    Connections connections;
    InnovationTracker *innovation_tracker;
};

std::ostream &operator<<(std::ostream &out, const Network &concs);

#endif
