#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "network.h"
#include "network_utils.h"
#include "population.h"
#include "utilities.h"

const double kill_percentage = 0.80;
const int training_runs = 100;

using namespace std;

Network breed(const Network &nw1, const Network &nw2)
{
    Network child = nw1;
    Connections &childCons = child.get_connections();
    const Connections &parentCons = nw2.get_connections();

   // cout << "Parent 1: " << endl;
   // cout << nw1 << endl;
   //
   // cout << "Parent 2: " << endl;
   // cout << nw2 << endl;

    // Pad out child to size of network2 if need be
    const Neurons &parentNeurons = nw2.get_neurons();
    const Neurons &childNeurons = child.get_neurons();
    for (int i = childNeurons.size(); i < parentNeurons.size(); i++) {
        child.add_new_neuron();
    }

    /*
     * Loop through each of parent2's connections doing:
     * - if parent2 has any connection that child doesn't, add it to child
     * - if parent2 has any disabled shared connections, disable it for child
     */
    for (const Connection &parentCon : parentCons) {
        bool match = false;
        for (Connection &childCon : childCons) {
            if (parentCon.get_id() == childCon.get_id()) {
                match = true;
                // if (std::rand() % 2 == 0)
                //     childCon.set_weight(parentCon.get_weight());
                childCon.set_weight((parentCon.get_weight() + childCon.get_weight()) / 2);
                break;
            }
        }

        if (!match) {
            double new_weight = parentCon.get_weight();
            child.connect(parentCon.get_in(), parentCon.get_out(), new_weight);
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
    feedback(_feedback)
{
    //populate population with single mutation networks
    for (int i = 0; i < size; i ++) {
        ranks.push_back(Rank(Network(_num_inputs, _num_outputs), 0));
        Network &n = ranks.back().first;
        
        const int mutation_count = 0;
        for(int i = 0; i < mutation_count; i++) {
           n.mutate();
       }
    }
}

vector<double> Population::generate_random_input() {
    const int bias = 5; // so that inputs aren't too small

    vector<double> random_inputs;
    for(int i = 0; i < num_inputs; i++) {
        random_inputs.push_back(bias * random_p());
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
std::mutex ranks_mutex;
static void evaluate_network(int network_index,
    std::vector<Rank> &ranks,
    std::vector<double> inputs,
    int trials,
    double (*feedback)(vector<double> &, vector<double> &))
{
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

static void lol(std::vector<Rank> &ranks) { std::cout << "HELLO WORLD" << std::endl; }

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
        Network &n1 = ranks[kill_num + ((n1_offset + i) % keep_num)].first;
        Network &n2 = ranks[ranks.size() - ((n2_offset + i) % keep_num)].first;
        ranks[i].first = breed(n1, n2);
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
    evaluate_fitness(training_runs); //evaulates 100 times
    replace_inferior_population(kill_percentage); //kills bottom 80%
}

double Population::get_average_fitness() const
{
    double fitness = 0;
    for (const Rank &rank : ranks) {
        fitness += rank.second;
    }
    return fitness / ranks.size();
}
