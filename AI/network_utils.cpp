
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
    activation_id(std::rand() % (activation_function_count - 2) + 2), // std::rand() % activation_function_count
    override_value(-1),
    override_flag(false) {}

Neuron::Neuron(int id, double override_value):
    id(id),
    activation_id(std::rand() % (activation_function_count - 2) + 2), // std::rand() % activation_function_count
    override_value(override_value),
    override_flag(true) {}

int Neuron::get_id() const { return id; }

double Neuron::evaluate(const Neurons &neurons,
    const Connections &connections,
    const std::unordered_map<int, int> &connection_map) const
{
    if (override_flag) {
        //cout << "EVALUATING INPUT NODE" << endl;
        return override_value;
    }

    // cout << "EVALUATING neuron # " << id << endl;

    std::vector<double> values;
    for (int index : inputs) {
        const Neuron &currentNeuron = neurons[index];
        int cantor_val = cantor(currentNeuron.get_id(), id);
        const Connection &currentConnection = connections[connection_map.at(cantor_val)];

        double weight = currentConnection.get_weight();
        double value = currentNeuron.evaluate(neurons, connections, connection_map);

        cout << "CURRENT NEURON # " << currentNeuron.id << endl;
        cout << currentConnection << endl;
        cout << "cantor: " << cantor_val << endl;
        cout << "weight: " << weight << endl;
        cout << "value: " << value << endl;
        cout << "end value: " << weight * value << endl;

        values.push_back(weight * value);
    }
    return activation_functions[activation_id](values);
}

void Neuron::add_input(int new_in) { inputs.push_back(new_in); }
void Neuron::add_output(int new_out) { outputs.push_back(new_out); }

bool Neuron::is_overriden() const { return override_flag; }
void Neuron::set_override_value(double new_value)
{
    override_flag = true;
    override_value = new_value;
}
double Neuron::get_override_value() const { return override_value; }

int Neuron::get_activation_id() const { return activation_id; }
void Neuron::set_activation_id(int id) { activation_id = id; }


std::vector<int> &Neuron::get_inputs() { return inputs; }
const std::vector<int> &Neuron::get_inputs() const { return inputs; }
std::vector<int> &Neuron::get_outputs() { return outputs; }
const std::vector<int> &Neuron::get_outputs() const { return outputs; }

std::ostream &operator<<(std::ostream &out, const Neuron &neuron)
{
    out << neuron.get_id() << ": \tact: " << activation_function_names[neuron.get_activation_id()] << "\t";
    if (neuron.is_overriden())
        out << "override: " << neuron.get_override_value();
    // out << endl;

    // const std::vector<int> &inputs = neuron.get_inputs();
    // out << "Inputs: \t";
    // for (int index : inputs) {
    //     out << index << ", ";
    // }
    // out << endl;
    //
    // const std::vector<int> &outputs = neuron.get_outputs();
    // out << "Outputs: \t";
    // for (int index : outputs) {
    //     out << index << ", ";
    // }
    // out << endl;

    return out;
}

//Connection -----------------------------------------------------------
//bijective function (int,int)->int
int cantor(int x, int y) {
   return (x+y)*(x+y+1)/2 + y;
}

Connection::Connection(int in, int out, double weight):
    id(cantor(in, out)),
    in(in),
    out(out),
    weight(weight) {}

int Connection::get_id() const {return id;}
int Connection::get_in() const {return in;}
int Connection::get_out() const {return out;}
double Connection::get_weight() const {return weight;}

void Connection::set_weight(double new_weight) {weight = new_weight;}
void Connection::enable() {enabled = true;}
void Connection::disable() {enabled = false;}

std::ostream &operator<<(std::ostream &out, const Connection &con)
{
    out << con.get_id() << ": \t" << con.get_in() << "->";
    out << con.get_out() << "\t w: " << con.get_weight();

    return out;
}

//Trigger Function -----------------------------------------------------
