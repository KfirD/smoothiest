#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <cmath>
#include <unordered_map>

#include "network.h"
#include "activation.h"
#include "innovation_tracker.h"

using namespace std;

static bool firstRand = true;

Network::Network() : numInputs(0), numOutputs(0), neuronCount(0) {}

double randomP() {
    if(firstRand) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        srand((time_t)ts.tv_nsec);
        firstRand = false;
    }
    return ((double) rand() / (RAND_MAX));
}

int randomBig() {
    if(firstRand) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        srand((time_t)ts.tv_nsec);
        firstRand = false;
    }
    return rand();
}

Network::Network(int numIn, int numOut):
    numInputs(numIn), numOutputs(numOut), neuronCount(numIn + numOut) {
    inputs.resize(numIn, 0.0);
    outputs.resize(numOut, 0.0);

    for(int i = 0; i < numIn; i++) {
        neurons.push_back(Neuron(i, 0.0));
        neuronIndexMap[i] = i;
    }
    for(int i = numIn; i < numIn + numOut; i++) {
        neurons.push_back(Neuron(i));
        neuronIndexMap[i] = i;
    }

    InnovationTracker& tracker = InnovationTracker::instance();
    for(int i = 0; i < numIn; i++) {
        for(int j = 0; j < numOut; j++) {
            int inn = tracker.getInnovation(i, numIn + j);
            connections.push_back(Connection(i, numIn + j, (randomP() * 2 - 1) * 0.5, inn));
        }
    }
}

Network::Network(const Network& other) :
    numInputs(other.numInputs), numOutputs(other.numOutputs), neuronCount(other.neuronCount) {
    inputs.resize(numInputs, 0.0);
    outputs.resize(numOutputs, 0.0);
    
    for(const auto& n : other.neurons) {
        if(n.id < other.numInputs) {
            neurons.push_back(Neuron(n.id, 0.0));
        } else {
            neurons.push_back(Neuron(n.id));
        }
    }
    for(const auto& c : other.connections) {
        connections.push_back(Connection(c.get_in(), c.get_out(), c.get_weight(), c.get_innovation()));
        connections.back().enabled = c.enabled;
    }
    neuronIndexMap = other.neuronIndexMap;
}

const Connections& Network::getConnections() const { return connections; }
Connections& Network::getConnections() { return connections; }

bool Network::connect(int in, int out, double weight) {
    if(in < 0 || out < 0 || in >= neuronCount || out >= neuronCount) return false;
    if(in >= numInputs && in < numInputs + numOutputs) return false;
    if(out >= 0 && out < numInputs) return false;

    for(const auto& conn : connections) {
        if(conn.get_in() == in && conn.get_out() == out) return false;
    }

    InnovationTracker& tracker = InnovationTracker::instance();
    int inn = tracker.getInnovation(in, out);
    connections.push_back(Connection(in, out, weight, inn));
    neurons[in].add_output(out);
    neurons[out].add_input(in);

    return true;
}

Neuron& Network::getRandomNeuron() {
    int rand = randomBig() % neurons.size();
    return neurons[rand];
}

Connection& Network::getRandomConnection() {
    int rand = randomBig() % connections.size();
    return connections[rand];
}

int Network::addNewNeuron(int betweenIn, int betweenOut) {
    (void)betweenIn;
    (void)betweenOut;
    int newId = neuronCount++;
    neurons.push_back(Neuron(newId, 0.0));
    neuronIndexMap[newId] = neurons.size() - 1;
    inputs.push_back(0.0);

    return newId;
}

void Network::mutate(double pNewNode, double pNewCon, double pMutateWeight) {
    (void)pMutateWeight;
    double randVal = randomP();

    if(connections.empty()) {
        for(int i = 0; i < numInputs; i++) {
            for(int j = 0; j < numOutputs; j++) {
                connect(i, numInputs + j, (randomP() * 2 - 1) * 0.5);
            }
        }
        return;
    }

    if(randVal < pNewNode && !connections.empty()) {
        Connection& c = getRandomConnection();
        if(!c.enabled) {
            c.enable();
            return;
        }

        c.disable();
        int newNeuronId = addNewNeuron(c.get_in(), c.get_out());
        connect(c.get_in(), newNeuronId, 1.0);
        connect(newNeuronId, c.get_out(), c.get_weight());
    }
    else if(pNewNode < randVal && randVal <= pNewNode + pNewCon) {
        int maxAttempts = 50;
        for(int attempt = 0; attempt < maxAttempts; attempt++) {
            int inNode = randomBig() % neuronCount;
            int outNode = randomBig() % neuronCount;

            if(inNode == outNode) continue;
            if(inNode >= numInputs && inNode < numInputs + numOutputs) continue;
            if(outNode < numInputs) continue;

            if(connect(inNode, outNode, (randomP() * 2 - 1) * 0.5)) {
                break;
            }
        }
    }
    else {
        Connection& c = getRandomConnection();
        double w = c.get_weight();
        double change = randomP() * 0.4 - 0.2;

        if(randomP() < 0.1) {
            w = (randomP() * 2 - 1) * 2.0;
        } else {
            w += change;
        }

        c.set_weight(w);
    }
}

void Network::setInput(int idx, double value) {
    if(idx >= 0 && idx < numInputs) {
        inputs[idx] = value;
        neurons[idx].set_override_value(value);
    }
}

void Network::clearInputs() {
    for(int i = 0; i < numInputs; i++) {
        inputs[i] = 0.0;
        neurons[i].clear_override();
    }
    for(int i = 0; i < numOutputs; i++) {
        outputs[i] = 0.0;
    }
    for(auto& neuron : neurons) {
        neuron.reset_activation();
    }
}

vector<int> Network::topologicalSort() const {
    vector<int> result;
    unordered_set<int> visited;
    unordered_set<int> recursionStack;

    function<bool(int)> dfs = [&](int node) -> bool {
        if(recursionStack.count(node)) return false;
        if(visited.count(node)) return true;

        visited.insert(node);
        recursionStack.insert(node);

        for(int output : neurons[node].get_outputs()) {
            if(!dfs(output)) return false;
        }

        recursionStack.erase(node);
        result.push_back(node);
        return true;
    };

    for(int i = 0; i < neuronCount; i++) {
        if(!visited.count(i)) {
            if(!dfs(i)) {
                return vector<int>();
            }
        }
    }

    reverse(result.begin(), result.end());
    return result;
}

bool Network::hasCycle() const {
    unordered_set<int> visited;
    unordered_set<int> recursionStack;

    function<bool(int)> dfs = [&](int node) -> bool {
        if(recursionStack.count(node)) return true;
        if(visited.count(node)) return false;

        visited.insert(node);
        recursionStack.insert(node);

        for(int output : neurons[node].get_outputs()) {
            const Connection* conn = nullptr;
            for(const auto& c : connections) {
                if(c.get_in() == node && c.get_out() == output && c.enabled) {
                    conn = &c;
                    break;
                }
            }
            if(conn && dfs(output)) return true;
        }

        recursionStack.erase(node);
        return false;
    };

    for(int i = 0; i < neuronCount; i++) {
        if(!visited.count(i)) {
            if(dfs(i)) return true;
        }
    }
    return false;
}

void Network::evaluateNetwork() {
    for(int iter = 0; iter < 10; iter++) {
        for(auto& neuron : neurons) {
            neuron.reset_activation();
        }

        for(const auto& conn : connections) {
            if(!conn.enabled) continue;

            int inIdx = conn.get_in();
            int outIdx = conn.get_out();

            double value = neurons[inIdx].get_activation() * conn.get_weight();
            neurons[outIdx].add_input_value(value);
        }

        for(int i = numInputs; i < neuronCount; i++) {
            neurons[i].compute_activation();
        }
    }

    for(int i = 0; i < numOutputs; i++) {
        outputs[i] = neurons[numInputs + i].get_activation();
    }
}

vector<double> Network::forward(const vector<double>& input) {
    for(int i = 0; i < numInputs; i++) {
        neurons[i].set_override_value(i < (int)input.size() ? input[i] : 0.0);
    }
    
    for(int iter = 0; iter < 10; iter++) {
        for(auto& neuron : neurons) {
            neuron.reset_activation();
        }
        
        for(int i = 0; i < numInputs; i++) {
            neurons[i].restore_override();
        }

        for(const auto& conn : connections) {
            if(!conn.enabled) continue;

            int inIdx = conn.get_in();
            int outIdx = conn.get_out();

            double inValue = neurons[inIdx].get_activation();
            double w = conn.get_weight();
            double value = inValue * w;
            neurons[outIdx].add_input_value(value);
        }

        for(int i = numInputs; i < neuronCount; i++) {
            neurons[i].compute_activation();
        }
    }

    for(int i = 0; i < numOutputs; i++) {
        outputs[i] = neurons[numInputs + i].get_activation();
    }

    return outputs;
}

Network Network::crossover(const Network& parent1, const Network& parent2) {
    const Network* p1 = &parent1;
    const Network* p2 = &parent2;
    
    if(parent2.getNumNeurons() > parent1.getNumNeurons()) {
        std::swap(p1, p2);
    }
    
    Network child(p1->numInputs, p1->numOutputs);
    child.neuronCount = p1->neuronCount;
    child.neurons = p1->neurons;
    child.neuronIndexMap = p1->neuronIndexMap;
    child.inputs = p1->inputs;
    child.outputs = p1->outputs;
    child.connections.clear();
    
    const Connections& conn1 = p1->getConnections();
    const Connections& conn2 = p2->getConnections();
    
    std::unordered_map<int, const Connection*> conn2Map;
    for(const auto& c : conn2) {
        conn2Map[c.get_innovation()] = &c;
    }
    
    for(const auto& c1 : conn1) {
        auto it = conn2Map.find(c1.get_innovation());
        if(it != conn2Map.end()) {
            const Connection* c2 = it->second;
            if(randomP() < 0.5) {
                child.connections.push_back(Connection(c1.get_in(), c1.get_out(), c1.get_weight(), c1.get_innovation()));
            } else {
                child.connections.push_back(Connection(c2->get_in(), c2->get_out(), c2->get_weight(), c2->get_innovation()));
            }
            child.connections.back().enabled = c1.enabled && c2->enabled ? true : (randomP() < 0.5 ? c1.enabled : c2->enabled);
        } else {
            child.connections.push_back(Connection(c1.get_in(), c1.get_out(), c1.get_weight(), c1.get_innovation()));
            child.connections.back().enabled = c1.enabled;
        }
    }
    
    for(const auto& c2 : conn2) {
        bool found = false;
        for(const auto& c1 : conn1) {
            if(c1.get_innovation() == c2.get_innovation()) {
                found = true;
                break;
            }
        }
        if(!found) {
            child.connections.push_back(Connection(c2.get_in(), c2.get_out(), c2.get_weight(), c2.get_innovation()));
            child.connections.back().enabled = c2.enabled;
        }
    }
    
    for(const auto& c : child.connections) {
        if(c.enabled) {
            child.neurons[c.get_in()].add_output(c.get_out());
            child.neurons[c.get_out()].add_input(c.get_in());
        }
    }
    
    return child;
}

ostream& operator<<(ostream& out, const Network& net) {
    out << "Network(inputs=" << net.getNumInputs() << ", outputs=" << net.getNumOutputs() 
        << ", neurons=" << net.getNeuronCount() << ", connections=" << net.getConnections().size() << ")\n";
    for(const Connection& con : net.getConnections()) {
        out << "  " << con.get_in() << " -> " << con.get_out() 
            << " w=" << con.get_weight() << (con.enabled ? "" : " [DISABLED]") << "\n";
    }
    return out;
}
