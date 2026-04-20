#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <cmath>

#include "network_utils.h"

class Network {
public:
    Network();
    Network(int numIn, int numOut);
    Network(const Network& other);

    const Connections& getConnections() const;
    Connections& getConnections();
    bool connect(int in, int out, double weight);

    Connection& getRandomConnection();
    Neuron& getRandomNeuron();

    int addNewNeuron(int betweenIn, int betweenOut);
    void mutate(double pNewNode, double pNewCon, double pMutateWeight);
    static Network crossover(const Network& parent1, const Network& parent2);

    std::vector<double> forward(const std::vector<double>& inputs);
    double getOutput(int idx) const { return outputs[idx]; }
    int getNumInputs() const { return numInputs; }
    int getNumOutputs() const { return numOutputs; }
    int getNumNeurons() const { return neurons.size(); }

    void setInput(int idx, double value);
    void clearInputs();

    bool hasCycle() const;

    int getNeuronCount() const { return neuronCount; }

private:
    const int numInputs;
    const int numOutputs;
    int neuronCount;
    Neurons neurons;
    Connections connections;
    std::vector<double> inputs;
    std::vector<double> outputs;
    std::unordered_map<int, int> neuronIndexMap;

    void computeTopology();
    std::vector<int> topologicalSort() const;
    void evaluateNetwork();

    friend std::ostream& operator<<(std::ostream& out, const Network& concs);
};

std::ostream& operator<<(std::ostream& out, const Network& concs);

#endif
