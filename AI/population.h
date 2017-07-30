
#ifndef POPULATION_H
#define POPULATION_H

#include "network.h"

using Networks = vector<Network>;

class Population {
public:
   Population(int num_inputs, int num_outputs, int size,
   double (*_feedback)(vector<double>&, vector<double>&));

   Networks networks;
   vector<int> fitnesses;
   double (*feedback)(vector<double>&, vector<double>&);

   int get_size();
   void run_generation();
   void reset_fitnesses();
   void evaluate_fitness(int num_times);
   void kill_inferior_population(double percentage);
   void restore_population(int target_size);

private:
   Network &get_random_network();
   vector<double> generate_random_input();
};

Network breed(const Network &network1, const Network &network2);

#endif
