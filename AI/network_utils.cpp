
#include <cmath>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "activation.h"
#include "network.h"
#include "network_utils.h"

//Neuron ---------------------------------------------------------------
Neuron::Neuron(int id):
    id(id),
    activation_id(std::rand() % activation_function_count),
    override_value(-1),
    override_flag(false) {}

Neuron::Neuron(int id, double override_value):
    id(id),
    activation_id(std::rand() % activation_function_count),
    override_value(override_value),
    override_flag(true) {}

int Neuron::get_id() const { return id; }

double Neuron::evaluate(const Neurons &neurons, const std::unordered_map<int, const Connection &> &connection_map) const
{
    if (override_flag)
        return override_value;

    std::vector<double> values;
    for (int index : inputs) {
        const Neuron &currentNeuron = neurons[index];
        const Connection &currentConnection = connection_map.at(cantor(currentNeuron.get_id(), id));

        values.push_back(currentConnection.get_weight() * currentNeuron.evaluate(neurons, connection_map));
    }
    return activation_functions[activation_id](values);
}

void Neuron::add_input(int new_in) {
   inputs.push_back(new_in);
}

void Neuron::add_output(int new_out) {
   outputs.push_back(new_out);
}

void Neuron::set_override_value(double new_value)
{
    override_flag = true;
    override_value = new_value;
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
