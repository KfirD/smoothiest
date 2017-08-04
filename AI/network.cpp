
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unordered_map>

#include "activation.h"
#include "network.h"
#include "utilities.h"

using namespace std;

//Network()
//Notes: constructor
Network::Network(int num_in, int num_out):
    num_in(num_in), num_out(num_out)
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
   neuron_count = num_in + num_out;

   for(int i = 0; i < num_in; i++) {
      for(int j = num_in; j < num_out+num_in; j++) {
         connect(i,j,random_p());
      }
   }
}

const Neurons &Network::get_neurons() const { return neurons; }
Neurons &Network::get_neurons() { return neurons; }

const Connections &Network::get_connections() const { return connections; }
Connections &Network::get_connections() { return connections; }

const std::unordered_map<int, int> &Network::get_connection_map() const { return connection_map; }
std::unordered_map<int, int> &Network::get_connection_map() { return connection_map; }

//connect()
//Notes: all connecting handled here
bool Network::connect(int in, int out, double weight) {
   bool connect_debug = false;

    // push connection into connection vector
    connections.push_back(Connection(in, out, weight));

    const Connection &new_connection = connections.back();    // get new connection

    connection_map.insert({new_connection.get_id(), connections.size() - 1});

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

   std::vector<int> &outputs = in_neuron.get_outputs();
   outputs.erase(outputs.begin(),
      std::find(outputs.begin(), outputs.end(), out));

   std::vector<int> &inputs = out_neuron.get_inputs();
   inputs.erase(inputs.begin(),
      std::find(inputs.begin(), inputs.end(), in));
}

void Network::set_input_neurons(const std::vector<double> &inputs)
{
    if (num_in != inputs.size()) {
        cerr << "ERROR: inputs does not match Network's num_in." << endl;
    }

    for (int i = 0; i < num_in; i++) {
        Neuron &currentNeuron = neurons[i];
        currentNeuron.set_override_value(inputs[i]);
    }
}

std::vector<double> Network::evaluate(const std::vector<double> &inputs)
{
    set_input_neurons(inputs);

    //std::cout << "######## Network::evaluate()" << endl;

    std::vector<double> outputs;
    for (int i = num_in; i < (num_in + num_out); i++) {
        Neuron &currentNeuron = neurons[i];
        outputs.push_back(currentNeuron.evaluate(neurons, connections, connection_map));
    }
    return outputs;
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
      cerr << "get_random_connection() called, but no connections";
   }
   int rand = random_big()%connections.size();
   Connection &c = connections[rand];
   return c;
}

//get_random_connection();
//Output: Random connection not in the network
Connection Network::get_random_unconnection() {
   int limit = neurons.size();

   for(int i = 0; i < limit; i++) {
      Neuron &A = get_random_neuron();
      Neuron &B = get_random_neuron();
      vector<int> &Ain = A.get_inputs();
      vector<int> &Bin = B.get_inputs();
      if(A.get_id() == B.get_id()) continue;
      bool isAB = find(Bin.begin(), Bin.end(), A.get_id()) != Bin.end();
      if(!isAB) {
         return Connection(A.get_id(), B.get_id(), random_p());
      }
      bool isBA = find(Ain.begin(), Ain.end(), B.get_id()) != Ain.end();
      if(!isBA) {
         return Connection(B.get_id(), A.get_id(), random_p());
      }
   }
   return Connection(-1,-1,0); //The failure connection
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
   double p_new_node = 0.33;
   double p_new_con  = 0.33;
   double p_change_weight = 0.33;

   bool mutation_debug = false;

   double rand = random_p();
   if(rand <= p_new_node) {
      if(mutation_debug) cout << "Mutation: New Node as added\n";
      // add a new node
      Connection c(get_random_connection());
      c.disable();
      disconnect(c.get_in(), c.get_out());
      int new_neuron_id = add_new_neuron(); //TODO
      //TODO: should the new connections have the same weight?
      connect(c.get_in(), new_neuron_id, random_p());
      connect(new_neuron_id, c.get_out(), random_p());
   }
   else if(p_new_node < rand && rand <= p_new_node + p_new_con) {
    //   cout << "Mutation: Nothing Happened\n";
      // add a new Connection TODO

      double default_weight = 0.5; //TODO: what should it be?
      Connection c = get_random_unconnection();
      if(c.get_in() < 0) return;  //if no unconnection found, do nothing
      connect(c.get_in(), c.get_out(), c.get_weight());
      if(mutation_debug) cout << "Mutation: New Connection Made\n";
   }
   else {
      if(mutation_debug) cout << "Mutation: Weight was changed\n";
      // change weight of a connection
      Connection &c = get_random_connection();
      double rand = random_p();
      double w = c.get_weight();
      //50-50 chance of increasing or decreasing the weight
      //TODO: figure out correct amount to change weights
      w = rand < 0.5? w*(1/2.0+1/(1.0+w)) : w/(1/2.0+1/(1.0+w));
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
