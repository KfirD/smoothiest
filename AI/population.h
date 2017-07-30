#ifndef POPULATION_H
#define POPULATION_H

#include <list>

#include "network.h"

using Networks = vector<Network>;
using Rank = std::pair<Network, double>;

class Population {
public:
   Population(int num_inputs, int num_outputs, int size,
   double (*_feedback)(vector<double>&, vector<double>&));

   list<Rank> ranks;

   double (*feedback)(vector<double>&, vector<double>&);

   int get_size();
   void run_generation();
   void reset_fitnesses();
   void evaluate_fitness(int num_times);
   double get_network_fitness(Network &network, vector<double> &inputs, int trials);
   void kill_inferior_population(double percentage);
   void restore_population(int target_size);
   Network get_best_network();
   vector<double> generate_random_input();
private:
   const int num_inputs;
   const int num_outputs;
   Network &get_random_network();
};

Network breed(const Network &network1, const Network &network2);

#endif
