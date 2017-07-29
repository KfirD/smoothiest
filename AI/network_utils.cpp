
#include <iostream>

#include "network.h"

//Neuron ---------------------------------------------------------------
Neuron::Neuron(int id): id(id) {}
int Neuron::get_id() const { return id; }

//Connection -----------------------------------------------------------
//bijective function (int,int)->int
int cantor(int x, int y) {
   return (x+y)*(x+y+1)/2 + y;
}

Connection::Connection(Neuron &src, Neuron &dst, double weight):
src(src), dst(dst), weight(weight) {
   id = cantor(src.get_id(), dst.get_id());
}

int Connection::get_id() const {return id;}
Neuron& Connection::get_src() {return src;}
Neuron& Connection::get_dst() {return dst;}
double Connection::get_weight() {return weight;}


//Trigger Function -----------------------------------------------------
