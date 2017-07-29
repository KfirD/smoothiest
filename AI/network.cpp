
#include <iostream>

#include "network.h"

Network::Network(Neurons& input, Neurons& output):
input(input), output(output){}

Connections& Network::get_connections() {return connections;}

//all connecting handled here
//by convention a connection goes from low to high id
bool Network::connect(Neuron &src, Neuron &dst, double weight) {
   if(src.get_id() < dst.get_id()) {
      connections.push_back(Connection(src,dst,weight));
   }
   else {
      connections.push_back(Connection(dst,src,weight));
   }
   //should check for redundant connections


   return true;

}

std::ostream &operator<<(std::ostream& out, Network &coord) {
    for(Connection con: coord.get_connections()) {
      out << con.get_id() << ": " << con.get_src().get_id() << "->";
      out << con.get_dst().get_id() << " | ";
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
