
#include <iostream>

#include "network.h"

Network::Network(int num_in, int num_out):
num_in(num_in), num_out(num_out) {}

Connections& Network::get_connections() {return connections;}

//all connecting handled here
//by convention a connection goes from low to high id
bool Network::connect(int in, int out, double weight) {
   connections.push_back(Connection(in,out,weight));
   //TODO: check for redundant connections
   //TODO: check if actually suppose to be circular
   return true;

}

std::ostream &operator<<(std::ostream& out, Network &concs) {
    for(Connection con: concs.get_connections()) {
      out << con.get_id() << ": " << con.get_in() << "->";
      out << con.get_out() << " | ";
    }
    return out;
 }

/*
int Coordinate::getX() const { return x; }
int Coordinate::getY() const { return y; }

void Coordinate::setX(int x) { Coordinate::x = x; }
void Coordinate::setY(int y) { Coordinate::y = y; }
*/
/*
Coordinate &Coordinate::Neuron=(const Coordinate &coord)
{
    setX(coord.x);
    setY(coord.y);

    return *this;
}
*/
