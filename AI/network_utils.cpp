
#include <cmath>
#include <iostream>
#include <vector>

#include "activation.h"
#include "network.h"

//Neuron ---------------------------------------------------------------
Neuron::Neuron(int id): id(id) {}
int Neuron::get_id() const { return id; }

void Neuron::add_input(int new_in) {
   inputs.push_back(new_in);
}
void Neuron::add_output(int new_out) {
   outputs.push_back(new_out);
}

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

void Connection::set_weight(double new_weight) {weight = new_weight;}
void Connection::enable() {enabled = true;}
void Connection::disable() {enabled = false;}


//Trigger Function -----------------------------------------------------
