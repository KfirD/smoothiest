
#include <iostream>

#include "trainer.h"

const int input_count = 5;
const int output_count = 5;
const int population_size = 100;

double feedback(vector<double> &inputs, vector<double> &outputs)
{
    if (inputs.size() != outputs.size()) {
        std::cerr << "ERROR: feedback inputs and outputs have mismatching sizes." << std::endl;
        return -1;
    }

    double diff = 0;
    for (int i = 0; i < inputs.size(); i++) {
        diff += std::pow(inputs[i] - outputs[i], 2);
    }
    return -diff;
}

Trainer::Trainer():
    pop(input_count, output_count, population_size, feedback)
{

}

void Trainer::train(int generations)
{
    for (int i = 0; i < generations; i++) {
        std::cout << "Running generation " << i << std::endl;
        pop.run_generation();
    }

    Network best_network(pop.get_best_network());
    std::vector<double> inputs1 = pop.generate_random_input();
    std::vector<double> outputs1 = best_network.evaluate(inputs1);

    std::cout << "The best network is:" << std::endl;
    std::cout << best_network << std::endl;

    std::cout << "inputs1: " << std::endl;
    for (double in : inputs1) {
        std::cout << in << std::endl;
    }
    std::cout << std::endl;

    std::cout << "outputs1: " << std::endl;
    for (double out : outputs1) {
        std::cout << out << std::endl;
    }
}
