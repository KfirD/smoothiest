#include <iostream>
#include <vector>
#include <list>

#include "network.h"
#include "network_utils.h"
#include "population.h"
#include "utilities.h"


using namespace std;

void breed_in_connection(Network &child, const Connection &parentCon)
{
    Connections &childCons = child.get_connections();
    Neurons &childNeurons = child.get_neurons();
    int in = parentCon.get_in();
    int out = parentCon.get_out();

    cout << "out: " << out << endl;

    if (out > 3000 || out < 3000) {
        cout << "##################### WARNING" << endl;
        cout << parentCon << endl;
    }

    // cout << "BREEDING CHILD" << endl;
    // cout << "Child: " << endl;
    // cout << child << endl;
    // cout << "Parent connection: " << endl;
    // cout << parentCon << endl;
    // Check if there is a new node in parentCon and add it if there is
    int child_neuron_count = childNeurons.size();
    if (child_neuron_count < in || child_neuron_count < out) {
        int new_id = child.add_new_neuron();
    }
    child.connect(in, out, parentCon.get_weight());
}

Network breed(const Network &nw1, const Network &nw2)
{
    Network child = nw1;
    Connections &childCons = child.get_connections();
    const Connections &parentCons = nw2.get_connections();

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
                if (!parentCon.enabled) childCon.disable();
                break;
            }
        }

        if (!match) {
            breed_in_connection(child, parentCon);
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
num_outputs(_num_outputs)
{
   feedback = _feedback;
   //populate population with single mutation networks
   for(int i = 0; i < size; i ++) {
      ranks.push_back(Rank (Network(_num_inputs, _num_outputs), 0));
      Network &n = ranks.back().first;
      n.mutate();
   }
}

vector<double> Population::generate_random_input() {
   vector<double> random_inputs;
   for(int i = 0; i < num_inputs; i++) {
      random_inputs.push_back(random_p());
   }
   return random_inputs;
}

int Population::get_size() {
   return ranks.size();
}

//TODO: optimize this. advancing thru lists takes O(N) time
Network &Population::get_random_network() {
   int rand = random_big()%ranks.size();
   auto ptr = ranks.begin();
   advance(ptr, rand);
   return (*ptr).first;
}

void Population::reset_fitnesses() {
   for(Rank &rank: ranks) {
      rank.second = 0;
   }
}

void Population::evaluate_fitness(int num_times) {
   for(int trial = 0; trial< num_times; trial++) {
      for(Rank &rank: ranks) {
         Network &network = rank.first;
         vector<double> inputs = generate_random_input();
         vector<double> outputs = network.evaluate(inputs);
         double fitness = feedback(inputs, outputs);
         rank.second += fitness;
      }
   }
}


void Population::kill_inferior_population(double percentage) {
   //sort list by rank
   ranks.sort([](const Rank & a, const Rank & b) {
      return a.second < b.second;
   });
   //get number to kill
   int kill_num = ranks.size()*percentage/100;
   for(int i = 0; i < kill_num - 1; i++) {
      Network &target = ranks.front().first;
      ranks.pop_front();

      //destruct network
   }
}

void Population::restore_population(int target_size) {
   while(get_size() < target_size) {
      Network &n1 = get_random_network();
      Network &n2 = get_random_network();
      ranks.push_back(Rank (breed(n1,n2),0));
   }
}

void Population::run_generation() {
   reset_fitnesses();
   evaluate_fitness(100); //evaulates 100 times
   kill_inferior_population(80); //kills bottom 80%
   restore_population(100);
}
