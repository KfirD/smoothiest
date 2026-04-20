#include <iostream>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>

#include "cartpole.h"
#include "generation.h"
#include "renderer.h"
#include "organism.h"

constexpr int POP_SIZE = 50;
constexpr int NUM_INPUTS = 6;
constexpr int NUM_OUTPUTS = 2;
constexpr int EVAL_RUNS = 3;

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    std::cout << "NEAT Double CartPole" << std::endl;
    std::cout << "===================" << std::endl;
    
    Renderer renderer(800, 600);
    if(!renderer.init()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return 1;
    }
    
    Generation generation(POP_SIZE, NUM_INPUTS, NUM_OUTPUTS);
    
    std::cout << "Press ESC to quit, watch evolution in real-time!" << std::endl;
    
    while(!renderer.shouldQuit()) {
        std::cout << "\n--- Generation " << generation.getGenerationNumber() << " ---" << std::endl;
        
        for(auto& org : generation.getOrganisms()) {
            double totalSteps = 0;
            
            for(int run = 0; run < EVAL_RUNS; run++) {
                CartPole cp;
                cp.reset();
                int steps = 0;
                while(!cp.isDone() && steps < CartPole::MAX_STEPS) {
                    std::vector<double> state = cp.getState();
                    int action = org->act(state);
                    cp.step(action);
                    steps++;
                }
                totalSteps += steps;
            }
            
            org->setFitness(totalSteps / EVAL_RUNS);
        }
        
        generation.computeFitness();
        
        Organism* best = generation.getBestOrganism();
        
        if(best) {
            std::cout << "Best: " << (int)best->getFitness() << " | Avg: " << (int)generation.getAverageFitness() << std::endl;
            
            CartPole vizCp;
            vizCp.reset();
            while(!vizCp.isDone()) {
                std::vector<double> state = vizCp.getState();
                int action = best->act(state);
                vizCp.step(action);
                renderer.render(&vizCp, generation.getGenerationNumber(), 
                    vizCp.getSteps(), (int)best->getFitness(), (int)generation.getAverageFitness());
            }
            std::cout << "Survived: " << vizCp.getSteps() << " steps" << std::endl;
        }
        
        if(generation.getBestFitness() >= CartPole::MAX_STEPS) {
            std::cout << "\n*** SOLVED! ***" << std::endl;
            break;
        }
        
        generation.evolve();
    }
    
    renderer.close();
    return 0;
}
