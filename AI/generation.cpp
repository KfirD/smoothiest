
#include <exception>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "generation.h"
#include "network.h"
#include "utilities.h"

static const double elite_percentage = 0.40;

Generation::Generation(int input_count,
        int output_count,
        int pop_size,
        double (*_feedback)(vector<double>&, vector<double>&)):
    input_count(input_count),
    output_count(output_count),
    population_size(pop_size),
    species_counter(0),
    fitness_function(_feedback)
{
    for (int i = 0; i < pop_size; i++) {
        organisms.push_back({Organism::UnspeciedOrganism, 0, Network(input_count, output_count, &innovation_tracker)});
    }
    // Increment innovation tracker up by number of inputs and outputs
    for (int i = 0; i < input_count * output_count; i++) { innovation_tracker.new_innovation(); }

    speciate_organisms();
}

Generation Generation::create_new_generation()
{
    Generation new_gen(*this);

    // Steps to create new generation
    // 1. Resize each species
    // 2. Fill each species to its new size using the top r% of organisms
    // 3. Re-speciate all organisms

    resize_species();
    populate_species();
    speciate_organisms();

    return new_gen;
}

// d = c1(E / N) + c2(D / N) + c3 * W
bool Generation::are_same_species(const Network &n1, const Network &n2) const
{
    const double compatibility_threshold = 3;

    const double c1 = 3;
    const double c2 = 4;
    const double c3 = 0.4;

    const Connections &n1_cons = n1.get_connections();
    const Connections &n2_cons = n2.get_connections();

    double N = std::max(n1_cons.size(), n2_cons.size());
    double E = n1.disjoint_count_with(n2);
    double D = n1.excess_count_with(n2);
    double W = n1.average_weight_difference_with(n2);

    double compatibility_value = (c1 * (E / N) + c2 * (D / N) + c3 * W);
    return compatibility_value <= compatibility_threshold;
}

void Generation::speciate_organisms()
{
    // Loop through each organism
    for (int i = 0; i < organisms.size(); i++) {
        // Get random organism
        int random_index = random_big() % organisms.size();

        // std::cout << std::endl << "Initial random number: " << random_index << std::endl;

        // Starting from the random organism, loop through all organisms until an unspecied organism is found
        int organism_index = 0;
        for (organism_index = random_index;
            organisms.at(organism_index).species_name != -1;
            organism_index = ((organism_index + 1) % organisms.size()))
        {
            // std::cout << "Random organism finding loop: " << organism_index << std::endl;
        }

        Organism &current_organism = organisms.at(organism_index);

        if (current_organism.species_name != Organism::UnspeciedOrganism) std::cout << "ERROR: current organism is specied." << std::endl;

        // Loop through each species
        for (auto species_it = species.begin();
            species_it != species.end();
            ++species_it)
        {
            std::vector<int> &current_species_indexes = species_it->second.get_organism_indexes();

            // Loop through each organism in the current species
            for (auto specied_organism_it = current_species_indexes.begin();
                specied_organism_it != current_species_indexes.end();
                ++specied_organism_it)
            {
                const Organism &current_specied_organism = organisms.at(*specied_organism_it);

                if (are_same_species(current_specied_organism.network, current_organism.network)) {
                    // std::cout << "The organism DID MATCH a species" << std::endl;
                    current_organism.species_name = current_specied_organism.species_name;
                    species.at(current_organism.species_name).add(organism_index);
                    species.at(current_organism.species_name).size++;

                    goto endloop;   // GOTO, POSSIBLE BUGS
                }
            }
        }
        // std::cout << "The organism DID NOT match any existing species" << std::endl;

        // If the organism didn't match with any existing species, add it to a new species
        species.emplace(species_counter, Species());
        current_organism.species_name = species_counter;
        species.at(species_counter).add(organism_index);
        species.at(species_counter).size++;
        species_counter++;

        // Inner loop jumps here if the organism matches with an existing species
        // This skips over the default new species creation above
        endloop: ;
    }
}

void Generation::resize_species()
{
    // Species resizing algorithm:
    // new species size = (sum of all adjusted fitnesses in species) / mean adjusted fitness across population
    std::vector<double> adjusted_fitnesses;
    for (int i = 0; i < organisms.size(); i++) {
        Organism &current_organism = organisms.at(i);
        std::vector<double> random_inputs = generate_random_input();
        std::vector<double> outputs = current_organism.network.evaluate(random_inputs);
        current_organism.fitness = fitness_function(random_inputs, outputs);

        int species_size = species.at(current_organism.species_name).get_organism_indexes().size();

        adjusted_fitnesses.push_back(current_organism.fitness / species_size);
    }

    std::vector<double> sorted_adjusted_fitnesses = adjusted_fitnesses;
    std::sort(sorted_adjusted_fitnesses.begin(),
        sorted_adjusted_fitnesses.end(),
        [](double a, double b) {
            return a < b;
        });
    double mean_adjusted_fitness = sorted_adjusted_fitnesses.at(sorted_adjusted_fitnesses.size() / 2);

    std::cout << "Mean adjusted fitness: " << mean_adjusted_fitness << std::endl;

    // Calculate each species' new size
    int total_species_size = 0;
    for (auto &pair : species) {
        Species &current_species = pair.second;
        const std::vector<int> &organism_indexes = pair.second.get_organism_indexes();

        double adjusted_fitness_sum = 0;
        for (int organism_index : organism_indexes) {
            adjusted_fitness_sum += organisms.at(organism_index).fitness;
        }
        adjusted_fitness_sum /= organisms.size();
        double new_species_size = adjusted_fitness_sum / mean_adjusted_fitness;

        std::cout << "Species is going from size " << organisms.size();
        std::cout << " to " << new_species_size << std::endl;

        current_species.size = new_species_size;
        total_species_size += new_species_size;
    }

    // Adjust each species' size to fit within the population_size
    for (auto &pair : species) {
        Species &current_species = pair.second;
        current_species.size = 100 * (current_species.size / total_species_size);

        std::cout << "Species' new adjusted size is " << current_species.size << std::endl;
    }
}

void Generation::populate_species()
{
    // Fill each species to its new size using the top r% of organisms
    std::vector<Organism> new_organisms;
    for (auto &pair : species) {
        Species &current_species = pair.second;
        std::vector<int> &organism_indexes = current_species.get_organism_indexes();
        std::vector<Organism *> current_organisms(organism_indexes.size());

        // Get all organisms belonging to the current species
        for (int index : organism_indexes) {
            current_organisms.at(index) = &organisms.at(index);
        }

        // Sort organisms by fitness
        std::sort(current_organisms.begin(), current_organisms.end(), [](const Organism *a, const Organism *b) {
            return a->fitness > b->fitness;
        });

        organism_indexes.clear();
        for (int i = 0; i < current_species.size; i++) {
            const Organism &parent1 = *current_organisms.at(random_big() % (int)(current_organisms.size() * elite_percentage));
            const Organism &parent2 = *current_organisms.at(random_big() % (int)(current_organisms.size() * elite_percentage));

            new_organisms.push_back({Organism::UnspeciedOrganism, 0, breed(parent1, parent2)});
        }
    }

    organisms = new_organisms;
}

vector<double> Generation::generate_random_input() const
{
    vector<double> random_inputs;
    for(int i = 0; i < input_count; i++) {
        random_inputs.push_back(1 - 2 * random_p());
    }
    return random_inputs;
}

Network Generation::breed(const Organism &organism1, const Organism &organism2)
{
    // Child inherits genes from most fit parent
    // then randomly choose between parents for shared genes

    // Find most fit parent
    const Network *most_fit_parent = nullptr;
    const Network *least_fit_parent = nullptr;
    if (organism1.fitness > organism2.fitness) {
        most_fit_parent = &organism1.network;
        least_fit_parent = &organism2.network;
    } else {
        most_fit_parent = &organism2.network;
        least_fit_parent = &organism1.network;
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

std::ostream &operator<<(std::ostream &out, const Generation &gen)
{
    out << "Species count: " << gen.species.size();
    return out;
}
