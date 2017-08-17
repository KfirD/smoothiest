
#include <exception>
#include <iostream>
#include <vector>

#include "generation.h"
#include "network.h"
#include "utilities.h"

Generation::Generation(int input_count, int output_count, int pop_size):
    input_count(input_count),
    output_count(output_count),
    population_size(pop_size)
{
    std::vector<Organism> organisms;
    for (int i = 0; i < pop_size; i++) {
        organisms.push_back({-1, Network(input_count, output_count, &innovation_tracker)});
    }
    // Increment innovation tracker up by number of inputs and outputs
    for (int i = 0; i < input_count * output_count; i++) { innovation_tracker.new_innovation(); }

    // Loop through each organism
    for (int i = 0; i < organisms.size(); i++) {
        // Get random organism
        int random_index = random_big() % organisms.size();

        // std::cout << std::endl << "Initial random number: " << random_index << std::endl;

        // Starting from the random organism, loop through all organisms until an unspecied organism is found
        int organism_index = 0;
        for (organism_index = random_index;
            organisms.at(organism_index).species_index != -1;
            organism_index = ((organism_index + 1) % organisms.size()))
        {
            // std::cout << "Random organism finding loop: " << organism_index << std::endl;
        }

        Organism &current_organism = organisms.at(organism_index);

        if (current_organism.species_index != -1) std::cout << "ERROR: current organism is specied." << std::endl;

        // Loop through each species
        for (auto species_it = species.begin();
            species_it != species.end();
            ++species_it)
        {
            std::vector<Organism *> &current_species = species_it->get_organisms();

            // Loop through each organism in the current species
            for (auto specied_organism_it = current_species.begin();
                specied_organism_it != current_species.end();
                ++specied_organism_it)
            {
                const Organism &current_specied_organism = **specied_organism_it;

                if (are_same_species(current_specied_organism.network, current_organism.network)) {
                    // std::cout << "The organism DID MATCH a species" << std::endl;
                    current_organism.species_index = current_specied_organism.species_index;
                    species.at(current_organism.species_index).add(current_organism);

                    goto endloop;   // GOTO, POSSIBLE BUGS
                }
            }
        }
        // std::cout << "The organism DID NOT match any existing species" << std::endl;

        // If the organism didn't match with any existing species, add it to a new species
        species.emplace_back();
        current_organism.species_index = species.size() - 1;
        species.at(current_organism.species_index).add(current_organism);

        // Inner loop jumps here if the organism matches with an existing species
        // This skips over the default new species creation above
        endloop: ;
    }

/*
    for (auto organism_it = organisms.begin(); organism_it != organisms.end(); ++organism_it) {
        Organism &current_organism = *organism_it;

        bool found_match = false;
        const int max_tries = 100;
        for (int i = 0; i < max_tries; i++) {
            Organism &random_organism = organisms.at(random_big() % organisms.size());

            if (&random_organism == &current_organism) continue;

            if (are_same_species(random_organism.network, current_organism.network)) {
                if (random_organism.species_index == -1) {
                    species.push_back(Species());
                    Species &new_species = species.back();

                    int new_species_index = species.size() - 1;
                    random_organism.species_index = new_species_index;
                    current_organism.species_index = new_species_index;

                    species[new_species_index].add(random_organism);
                    species[new_species_index].add(current_organism);
                } else {
                    current_organism.species_index = random_organism.species_index;
                    species[random_organism.species_index].add(current_organism);
                }

                found_match = true;
                break;
            }
        }

        if (!found_match) {
            species.push_back(Species());
            Species &new_species = species.back();

            int new_species_index = species.size() - 1;
            current_organism.species_index = new_species_index;
            species[new_species_index].add(current_organism);
        }
    }
    */
}

// d = c1(E / N) + c2(D / N) + c3 * W
bool Generation::are_same_species(const Network &n1, const Network &n2) const
{
    const double compatibility_threshold = 4;

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

std::ostream &operator<<(std::ostream &out, const Generation &gen)
{
    out << "Species count: " << gen.species.size();
    return out;
}
