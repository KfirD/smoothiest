
#include <cmath>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "activation.h"
#include "network_utils.h"

Neuron::Neuron(int nid):
    id(nid),
    activation_id(0),
    override_value(-1),
    override_flag(false),
    input_sum(0.0),
    activation(0.0) {}

Neuron::Neuron(int nid, double override_value):
    id(nid),
    activation_id(0),
    override_value(override_value),
    override_flag(true),
    input_sum(0.0),
    activation(override_value) {}

void Neuron::compute_activation() {
    if(override_flag) {
        activation = override_value;
        return;
    }
    activation = tanh(input_sum);
}

int cantor(int x, int y) {
    return (x+y)*(x+y+1)/2 + y;
}

Connection::Connection(int nin, int nout, double w, int inn):
    id(cantor(nin, nout)),
    in(nin),
    out(nout),
    enabled(true),
    weight(w),
    innovation(inn) {}
