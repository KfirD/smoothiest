
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

Connection::Connection(int in, int out, double weight):
   in(in),
   out(out),
   weight(weight),
   id(cantor(in,out))
{}

int Connection::get_id() const {return id;}
int Connection::get_in() const {return in;}
int Connection::get_out() const {return out;}
double Connection::get_weight() const {return weight;}


//Trigger Function -----------------------------------------------------
