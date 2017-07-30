
#include <iostream>

#include "network.h"

Network::Network(int num_in, int num_out):
num_in(num_in), num_out(num_out) {}

const Connections& Network::get_connections() const {return connections;}
Connections& Network::get_connections() {return connections;}

//all connecting handled here
//by convention a connection goes from low to high id
bool Network::connect(int in, int out, double weight) {
   connections.push_back(Connection(in,out,weight));
   //TODO: check for redundant connections
   //TODO: check if actually suppose to be circular
   return true;

}

void Network::mutate() {}

std::ostream &operator<<(std::ostream& out, const Network &concs) {
    for(Connection con: concs.get_connections()) {
      out << con.get_id() << ": " << con.get_in() << "->";
      out << con.get_out() << std::endl;
    }
    return out;
 }
