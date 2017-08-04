
#include <iostream>

#include "trainer.h"

const int input_count = 5;
const int output_count = 5;
const int population_size = 100;

double change(double input)
{
    return std::pow(input, 2);
}

double feedback(vector<double> &inputs, vector<double> &outputs)
{
    if (inputs.size() != outputs.size()) {
        std::cerr << "ERROR: feedback inputs and outputs have mismatching sizes." << std::endl;
        return -1;
    }

    double diff = 0;
    for (int i = 0; i < inputs.size(); i++) {
        double goal = change(inputs[i]);
        double real = outputs[i];

        diff -= std::pow(std::abs(goal - real), 2);
    }
    return diff;
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

    std::cout << endl << "The best network is:" << std::endl;
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
    std::cout << std::endl;

    std::cout << "goals: " << std::endl;
    for (double goal : inputs1) {
        std::cout << change(goal) << std::endl;
    }
}
