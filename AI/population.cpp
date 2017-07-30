#include <iostream>
#include <vector>

#include "network.h"
#include "network_utils.h"
#include "population.h"

using namespace std;

Network breed(const Network &nw1, const Network &nw2)
{
    Network child = nw1;
    Connections &childCons = child.get_connections();
    const Connections &parentCons = nw2.get_connections();

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

        if (!match)
            childCons.push_back(parentCon);
    }

    child.mutate();
    return child;
}

//TODO --------------------------------------
Network &Population::get_random_network() {
   return networks.back();
}

int Population::get_size() {
   return networks.size();
}

vector<double> Population::generate_random_input() {
   return {1,2};
}
//--------------------------------------

//Population()
//Notice the constructor takes in a function point to a feedback
//function
Population::Population(int num_inputs, int num_outputs, int size,
double (*_feedback)(vector<double>&, vector<double>&)) {
   feedback = _feedback;
   //populate population with single mutation networks
   for(int i = 0; i < size; i ++) {
      networks.push_back(Network(num_inputs, num_outputs));
      networks.back().mutate();
   }
}

void Population::reset_fitnesses() {
   std::fill(fitnesses.begin(), fitnesses.end(), 0);
}

void Population::evaluate_fitness(int num_times) {
   for(int trial = 0; trial< num_times; trial++) {
      int index = 0;
      for(Network &network: networks) {
         vector<double> inputs = generate_random_input();
         vector<double> outputs = network.evaluate(inputs);
         double fitness = feedback(inputs, outputs);
         fitnesses[index++] += fitness;
      }
   }
}


void Population::kill_inferior_population(double percentage) {
   /*
   typedef std::pair<int, int> ipair;
   std::list<ipair> thelist;
   thelist.sort([](const ipair & a, const ipair & b) { return a.first < b.first; });
   */
}

void Population::restore_population(int target_size) {
   while(get_size() < target_size) {
      Network &n1 = get_random_network();
      Network &n2 = get_random_network();
      networks.push_back(breed(n1,n2));
   }
}

void Population::run_generation() {
   reset_fitnesses();
   evaluate_fitness(100); //evaulates 100 times
   kill_inferior_population(80); //kills bottom 80%
   restore_population(100);
}
