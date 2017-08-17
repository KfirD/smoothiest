#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Neuron;
class Connection;

using Neurons = vector<Neuron>;
using Connections = vector<Connection>;

int cantor(int a, int b);

/* Neuron */
class Neuron {
public:
    // Data
    const int id;

    // Methods
    Neuron(int id);
    Neuron(int id, double override_value);
    int get_id() const;
    double evaluate(const Neurons &neurons,
        const Connections &connections,
        const std::unordered_map<int, int> &conn_map,
        const std::vector<double> &input_values) const;
    void add_input(int new_in);
    void add_output(int new_out);
    bool is_overriden() const;
    void set_override_value(double val);
    double get_override_value() const;
    int get_activation_id() const;
    void set_activation_id(int id);
    const std::vector<int> &get_inputs() const;
    std::vector<int> &get_inputs();
    const std::vector<int> &get_outputs() const;
    std::vector<int> &get_outputs();
    void print_connections() const;
private:
    int activation_id;
    double override_value;
    bool override_flag;
    vector<int> inputs;
    vector<int> outputs;

    double evaluateR(const Neurons &neurons,
        const Connections &connections,
        const std::unordered_map<int, int> &conn_map,
        const std::vector<double> &input_values,
        std::unordered_set<int> &visited_neurons) const;
};

std::ostream &operator<<(std::ostream &out, const Neuron &neuron);


class Connection {
public:
    Connection(int innovation_number, int in, int out, double weight);
    int get_innovation_number() const;
    int get_in() const;
    int get_out() const;
    double get_weight() const;

    void set_weight(double new_weight);
private:
    int innovation_number;
    int in;
    int out;
    double weight;
};

std::ostream &operator<<(std::ostream &out, const Connection &con);

#endif
