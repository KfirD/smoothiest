
#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <unordered_map>

#include "activation.h"
#include "innovation_tracker.h"
#include "network.h"
#include "utilities.h"

using namespace std;

//Network()
//Notes: constructor
Network::Network(int num_in, int num_out, InnovationTracker *innovation_tracker):
    num_in(num_in),
    num_out(num_out),
    neuron_count(num_in + num_out),
    innovation_tracker(innovation_tracker)
{
    //initialize input and output neurons
    for(int i = 0; i < num_in; i++) {
        Neuron input(i);
        input.set_activation_id(0);
        neurons.push_back(input);
    }
    for(int i = num_in; i < num_in + num_out; i++) {
        Neuron output(i);
        output.set_activation_id(1);
        neurons.push_back(output);
    }

    for(int i = 0; i < num_in; i++) {
        for(int j = num_in; j < num_out+num_in; j++) {
            connect_without_innovation(i * num_in + (j - num_in), i, j, random_p());
        }
    }
}

const Neurons &Network::get_neurons() const { return neurons; }
Neurons &Network::get_neurons() { return neurons; }

const Connections &Network::get_connections() const { return connections; }
Connections &Network::get_connections() { return connections; }

//connect()
//Notes: all connecting handled here
bool Network::connect(int in, int out, double weight) {
    bool connect_debug = false;

    // push connection into connection vector
    connections.push_back(Connection(innovation_tracker->new_innovation(), in, out, weight));

    if(connect_debug) {
      cout << "##########: connect()" << endl;
      cout << "in: " << in << endl;
      cout << "out: " << out << endl;
      cout << "size: " << neurons.size() << endl;
    }

    Neuron &in_neuron = neurons[in];
    Neuron &out_neuron = neurons[out];
    in_neuron.add_output(out);
    out_neuron.add_input(in);

    //TODO: check for redundant connections
    //TODO: check if actually suppose to be circular
    return true;
}

bool Network::connect_without_innovation(int innovation_number, int in, int out, double weight)
{
    bool connect_debug = false;

    // push connection into connection vector
    connections.push_back(Connection(innovation_number, in, out, weight));

    if(connect_debug) {
        cout << "##########: connect()" << endl;
        cout << "in: " << in << endl;
        cout << "out: " << out << endl;
        cout << "size: " << neurons.size() << endl;
    }

    Neuron &in_neuron = neurons[in];
    Neuron &out_neuron = neurons[out];
    in_neuron.add_output(out);
    out_neuron.add_input(in);

    //TODO: check for redundant connections
    //TODO: check if actually suppose to be circular
    return true;
}

bool Network::disconnect(int in, int out)
{
    Neuron &in_neuron = neurons[in];
    Neuron &out_neuron = neurons[out];

    // Update input neuron's outputs
    std::vector<int> &outputs = in_neuron.get_outputs();
    auto it = std::find(outputs.begin(), outputs.end(), out);

    // Check that connection exists
    if (it == outputs.end()) {
        std::cerr << "ERROR: disconnect could not find connection from " << in;
        std::cerr << " to " << out << "." << std::endl;
        return false;
    }
    outputs.erase(it);

    // Update output neuron's inputs
    std::vector<int> &inputs = out_neuron.get_inputs();
    inputs.erase(std::find(inputs.begin(), inputs.end(), in));

    // Update connection list
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        Connection &currentConnection = *it;
        if (currentConnection.get_in() == in && currentConnection.get_out() == out) {
            connections.erase(it);
            return true;
        }
    }

    return false;
}

std::vector<double> Network::evaluate(const std::vector<double> &input_values) const
{
    // Create connection map
    std::unordered_map<int, int> connection_map(connections.size());
    for (int i = 0; i < connections.size(); i++) {
        const Connection &con = connections[i];
        connection_map.insert({cantor(con.get_in(), con.get_out()), i});
    }

    std::vector<double> outputs;
    for (int i = num_in; i < (num_in + num_out); i++) {
        const Neuron &currentNeuron = neurons[i];
        outputs.push_back(currentNeuron.evaluate(neurons, connections, connection_map, input_values));
    }
    return outputs;
}

int Network::get_max_innovation_number() const
{
    int num = 0;
    for (const Connection &c : connections) {
        if (c.get_innovation_number() > num) num = c.get_innovation_number();
    }
    return num;
}

bool Network::is_disjoint_connection(const Connection &con) const
{
    const int other_con_innovation_number = con.get_innovation_number();
    if (other_con_innovation_number < get_max_innovation_number()) {
        for (const Connection &c : connections) {
            // Not disjoint if it's shared
            if (c.get_innovation_number() == other_con_innovation_number) return false;
        }
        // Disjoint if it's not excess and not shared
        return true;
    }

    // Not disjoint if it's excess
    return false;
}

bool Network::is_excess_connection(const Connection &con) const
{
    return con.get_innovation_number() > get_max_innovation_number();
}

bool Network::is_shared_connection(const Connection &con) const
{
    const int other_con_innovation_number = con.get_innovation_number();
    for (const Connection &c : connections) {
        if (c.get_innovation_number() == other_con_innovation_number) return true;
    }
    return false;
}

int Network::disjoint_count_with(const Network &n) const
{
    int count = 0;

    // Look for disjoints in this
    for (const Connection &c : connections) {
        if (n.is_disjoint_connection(c))
            count++;
    }

    // Look for disjoints in n
    for (const Connection &c : n.connections) {
        if (is_disjoint_connection(c))
            count++;
    }

    return count;
}

int Network::excess_count_with(const Network &n) const
{
    const int max_innovation_number = get_max_innovation_number();
    const int n_max_innovation_number = n.get_max_innovation_number();

    if (n_max_innovation_number == max_innovation_number) return 0;

    const Network *highest_innovation_network = nullptr;
    const Network *lowest_innovation_network = nullptr;

    // Get the network with the highest innovation number
    if (n_max_innovation_number > max_innovation_number) {
        highest_innovation_network = &n;
        lowest_innovation_network = this;
    } else {
        highest_innovation_network = this;
        lowest_innovation_network = &n;
    }

    // Count the excess connections in that network
    int count = 0;
    for (const Connection &c : highest_innovation_network->connections) {
        if (c.get_innovation_number() > lowest_innovation_network->get_max_innovation_number())
            count++;
    }

    return count;
}

double Network::average_weight_difference_with(const Network &n) const
{
    double diff = 0;

    for (const Connection &con1 : connections) {
        for (const Connection &con2 : n.connections) {
            if (con1.get_innovation_number() == con2.get_innovation_number()) {
                diff += std::abs(con1.get_weight() - con2.get_weight());
            }
        }
    }

    return diff;
}

//get_random_neuron();
//Output: Random neuron in the network
Neuron &Network::get_random_neuron() {
    int rand = random_big()%neurons.size();
    return neurons[rand];
}

//get_random_connection();
//Output: Random connection in the network
Connection &Network::get_random_connection() {
    if(connections.size() == 0) {
        cerr << "get_random_connection() called, but no connections" << std::endl;
    }
    int rand = random_big()%connections.size();

    return connections[rand];
}

//get_random_connection();
//Output: Random connection not in the network
std::pair<int, int> Network::get_random_unconnection() {
   int limit = neurons.size();

   for(int i = 0; i < limit; i++) {
      Neuron &A = get_random_neuron();
      Neuron &B = get_random_neuron();
      vector<int> &Ain = A.get_inputs();
      vector<int> &Bin = B.get_inputs();
      if(A.get_id() == B.get_id()) continue;
      bool isAB = find(Bin.begin(), Bin.end(), A.get_id()) != Bin.end();
      if(!isAB) {
         return std::pair<int, int>(A.get_id(), B.get_id());
      }
      bool isBA = find(Ain.begin(), Ain.end(), B.get_id()) != Ain.end();
      if(!isBA) {
         return std::pair<int, int>(B.get_id(), A.get_id());
      }
   }
   return std::pair<int, int>(-1,-1); //The failure connection
}

//add_new_neuron();
int Network::add_new_neuron() { //TODO: add activation function, make actual object
   Neuron new_neuron(neuron_count);
   neurons.push_back(new_neuron);
   neuron_count++;
   return new_neuron.get_id();
}

//mutate()
//Effect: Does one of the following things to the network:
//        - Adds node in between a Connection
//        - Adds new connection between nodes
//        - Changes weight of a Connection
void Network::mutate() {
    //p = probability
    double p_new_node = 0.05;
    double p_new_con  = 0.05;
    double p_change_weight = 0.9;

    bool mutation_debug = false;

    if(random_p() < p_new_node) {
        if(mutation_debug) cout << "Mutation: New Node as added" << std::endl;
        // add a new node
        Connection &c = get_random_connection();
        disconnect(c.get_in(), c.get_out());
        int new_neuron_id = add_new_neuron(); //TODO
        //TODO: should the new connections have the same weight?
        connect(c.get_in(), new_neuron_id, c.get_weight());
        connect(new_neuron_id, c.get_out(), c.get_weight());
    }
    else if(random_p() < p_new_con) {
        //   cout << "Mutation: Nothing Happened\n";
        // add a new Connection TODO

        double default_weight = 0.5; //TODO: what should it be?
        std::pair<int, int> new_con_nodes = get_random_unconnection();
        if(new_con_nodes.first < 0) return;  //if no unconnection found, do nothing
        connect(new_con_nodes.first, new_con_nodes.second, default_weight);
        if(mutation_debug) cout << "Mutation: New Connection Made" << std::endl;
    }

    if (random_p() < p_change_weight) {
        if(mutation_debug) cout << "Mutation: All weights are changed" << std::endl;

        const double weight_mutation_chance = 0.80;

        // Adjust the weights of all connections
        for (Connection &con : connections) {
            double w = con.get_weight();
            if (random_p() < weight_mutation_chance) {
                // Kfir's fancy weight manipulation
                // w = rand < 0.5 ? w*(1/2.0+1/(1.0+w)) : w/(1/2.0+1/(1.0+w));

                w = random_p() < 0.5 ? w * 0.9 : w * 1.1;
                // I don't think this was actually mutating connection weights at all???
                con.set_weight(w);  // Changes the connection weight
            }
        }
    }
}

//printing
std::ostream &operator<<(std::ostream& out, const Network &network) {
    out << "Connections: " << endl;
    for(const Connection &con : network.get_connections()) {
        out << con << endl;
    }

    out << "Neurons: " << endl;
    for(const Neuron &n : network.get_neurons()) {
        out << n << endl;
    }

    return out;
}
