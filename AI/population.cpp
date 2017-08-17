#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "generation.h"
#include "innovation_tracker.h"
#include "network.h"
#include "network_utils.h"
#include "population.h"
#include "utilities.h"

const double kill_percentage = 0.80;
const int training_runs = 100;

using namespace std;

Network breed(const Rank &rank1, const Rank &rank2)
{
    // Child inherits genes from most fit parent
    // then randomly choose between parents for shared genes

    // Find most fit parent
    const Network *most_fit_parent = nullptr;
    const Network *least_fit_parent = nullptr;
    if (rank1.second > rank2.second) {
        most_fit_parent = &rank1.first;
        least_fit_parent = &rank2.first;
    } else {
        most_fit_parent = &rank2.first;
        least_fit_parent = &rank1.first;
    }

    Network child = *most_fit_parent;
    const Network &parent = *least_fit_parent;
    Connections &childCons = child.get_connections();
    const Connections &parentCons = parent.get_connections();

    for (const Connection &parentCon : parentCons) {
        for (Connection &childCon : childCons) {
            if (parentCon.get_innovation_number() == childCon.get_innovation_number()) {
                if (random_p() < 0.5)
                    childCon.set_weight(parentCon.get_weight());
                break;
            }
        }
    }

    child.mutate();
    return child;
}

//Population()
//Notice the constructor takes in a function point to a feedback
//function
Population::Population(int _num_inputs, int _num_outputs, int size,
    double (*_feedback)(vector<double>&, vector<double>&)):

    num_inputs(_num_inputs),
    num_outputs(_num_outputs),
    pop_size(size),
    feedback(_feedback),
    current_generation(num_inputs, num_outputs, pop_size)
{
    //populate population with single mutation networks
    for (int i = 0; i < pop_size; i ++) {
        ranks.push_back(Rank(Network(num_inputs, num_outputs, &innovation_tracker), 0));
        Network &n = ranks.back().first;
        // Removed mutations because NEAT specifically doesn't want mutated networks
    }
    // Increment innovation tracker up by number of inputs and outputs
    for (int i = 0; i < num_inputs * num_outputs; i++) { innovation_tracker.new_innovation(); }
}

vector<double> Population::generate_random_input() const {
    vector<double> random_inputs;
    for(int i = 0; i < num_inputs; i++) {
        random_inputs.push_back(1 - 2 * random_p());
    }
    return random_inputs;
}

int Population::get_size() const {
   return ranks.size();
}

Network &Population::get_random_network() {
   int random = random_big() % get_size();
   return ranks[random].first;
}

void Population::reset_fitnesses() {
   for (Rank &rank: ranks) {
      rank.second = 0;
   }
}

// Helper function for Population::evaluate_fitness
static void evaluate_network(int network_index,
    std::vector<Rank> &ranks,
    std::vector<double> inputs,
    int trials,
    double (*feedback)(vector<double> &, vector<double> &))
{
    static std::mutex ranks_mutex;
    Rank &rank = ranks[network_index];
    Network &network = rank.first;


    double fitness = 0;
    for (int i = 0; i < trials; i++) {
        vector<double> outputs = network.evaluate(inputs);
        fitness += feedback(inputs, outputs);
    }

    std::lock_guard<std::mutex> guard(ranks_mutex);
    rank.second = fitness;
}

void Population::evaluate_fitness(int num_times) {
    // Parallel algorithm
    std::vector<std::thread> threads;
    for (int i = 0; i < pop_size; i++) {
        threads.emplace_back(evaluate_network,
            i,
            std::ref(ranks),
            generate_random_input(),
            num_times,
            feedback);
    }

    for (auto &it : threads) {
        it.join();
    }

    // // Sequential algorithm
    // for (int i = 0; i < pop_size; i++) {
    //     evaluate_network(i, ranks, generate_random_input(), num_times, feedback);
    // }
}

void Population::replace_inferior_population(double percentage) {
    // Sort worst networks to lower 'percentage' of ranks
    std::sort(ranks.begin(), ranks.end(), [](const Rank &a, const Rank &b) {
        return a.second < b.second;
    });

    // Overwrite bottom 'percentage' of networks by breeding top performers
    int kill_num = ranks.size() * percentage;
    int keep_num = ranks.size() - kill_num;
    int n1_offset = random_big() % keep_num;
    int n2_offset = random_big() % keep_num;
    for (int i = 0; i < kill_num; i++) {
        const int parent1_index = kill_num + ((n1_offset + i) % keep_num);
        const int parent2_index = ranks.size() - 1 - ((n2_offset + i) % keep_num);

        const Rank &r1 = ranks[parent1_index];
        const Rank &r2 = ranks[parent2_index];

        ranks[i].first = breed(r1, r2);
    }
}

Network &Population::get_best_network()
{
    std::sort(ranks.begin(), ranks.end(), [](const Rank &a, const Rank &b) {
        return b.second < a.second;
    });
    return ranks.front().first;
}

void Population::run_generation() {
    reset_fitnesses();
    evaluate_fitness(training_runs);
    replace_inferior_population(kill_percentage);
}

double Population::get_average_fitness() const
{
    double fitness = 0;
    for (const Rank &rank : ranks) {
        fitness += rank.second;
    }
    return fitness / ranks.size();
}
