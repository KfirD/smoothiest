
#include <iostream>
#include <cstdlib>

#include "network.h"

//Network()
//Notes: constructor
Network::Network(int num_in, int num_out):
num_in(num_in), num_out(num_out) {
   //initialize input and output neurons
   for(int i = 0; i < num_in; i++) {
      neurons.push_back(Neuron(i));
   }
   for(int i = num_in; i < num_in + num_out; i++) {
      neurons.push_back(Neuron(i));
   }
   neuron_count = num_in + num_out;
}

const Connections& Network::get_connections() const {return connections;}
Connections& Network::get_connections() {return connections;}

//connect()
//Notes: all connecting handled here
bool Network::connect(int in, int out, double weight) {
   connections.push_back(Connection(in,out,weight));
   //TODO: check for redundant connections
   //TODO: check if actually suppose to be circular
   return true;

}

//get_random_neuron();
//Output: Random neuron in the network
Neuron &Network::get_random_neuron() {
   int rand = random()%neurons.size();
   return neurons[rand];
}

//get_random_connection();
//Output: Random connection in the network
Connection &Network::get_random_connection() {
   int rand = random()%connections.size();
   return connections[rand];
}

//get_random_connection();
//Output: Random connection not in the network
/*
Connection Network::get_random_unconnection() {
   return connections[rand];
}*/

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

   double rand = random();
   if(rand <= p_new_node) {
      // add a new node
      Connection &c = get_random_connection();
      c.disable();
      int new_neuron_id = add_new_neuron(); //TODO
      //TODO: should the new connections have the same weight?
      connect(c.get_in(), new_neuron_id, c.get_weight());
      connect(new_neuron_id, c.get_out(), c.get_weight());
   }
   else if(p_new_node < rand && rand <= p_new_node + p_new_con) {
      // add a new Connection TODO
      /*
      double default_weight = 0.5; //TODO: what should it be?
      Connection c = get_random_unconnection();
      connect(c.get_in(), c.get_out(), default_weight);
      */
   }
   else {
      // change weight of a connection
      Connection &c = get_random_connection();
      double rand = random();
      double w = c.get_weight();
      //50-50 chance of increasing or decreasing the weight
      //TODO: figure out correct amount to change weights
      w = rand < 0.5? w*(1/2+1/(1+w)) : w/(1/2+1/(1+w));
      c.set_weight(w);
   }

}

//printing
std::ostream &operator<<(std::ostream& out, const Network &concs) {
    for(Connection con: concs.get_connections()) {
      out << con.get_id() << ": " << con.get_in() << "->";
      out << con.get_out() << std::endl;
    }
    return out;
 }
