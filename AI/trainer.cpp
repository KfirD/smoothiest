
#include <iostream>
#include <vector>

#include "trainer.h"
#include "utilities.h"

const int input_count = 3;
const int output_count = 3;
const int population_size = 100;

static vector<double> generate_random_input()
{
    vector<double> random_inputs;
    for(int i = 0; i < input_count; i++) {
        random_inputs.push_back(1 - 2 * random_p());
    }
    return random_inputs;
}

static double change(double input)
{
    return std::pow(input, 2);
}

static double feedback(vector<double> &inputs, vector<double> &outputs)
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
    std::cout << "Running generation " << 0 << std::endl;
    pop.run_generation();

    Organism starting_best_organism = pop.get_best_organism();
    for (int i = 1; i < generations; i++) {
        std::cout << "Running generation " << i << std::endl;
        pop.run_generation();
        // std::cout << "Average fitness: " << pop.get_average_fitness() << std::endl;
    }
    Organism ending_best_organism = pop.get_best_organism();
    Network &best_network = ending_best_organism.network;

    std::vector<double> inputs1 = generate_random_input();
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

    int starting_fitness = starting_best_organism.fitness;
    int ending_fitness = ending_best_organism.fitness;
    std::cout << "The network improved by " << ending_fitness - starting_fitness << std::endl;
}
