#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "generation.h"
#include "innovation_tracker.h"
#include "network.h"
#include "network_utils.h"
#include "species.h"
#include "population.h"
#include "utilities.h"

const double kill_percentage = 0.80;
const int training_runs = 100;

using namespace std;

//Population()
//Notice the constructor takes in a function point to a feedback
//function
Population::Population(int _num_inputs, int _num_outputs, int size,
    double (*_feedback)(vector<double>&, vector<double>&)):

    num_inputs(_num_inputs),
    num_outputs(_num_outputs),
    pop_size(size),
    feedback(_feedback),
    current_generation(num_inputs, num_outputs, pop_size, _feedback)
{

}

Organism &Population::get_best_organism()
{
    return current_generation.get_best_organism();
}

void Population::run_generation() {
    current_generation = current_generation.create_new_generation();
}
