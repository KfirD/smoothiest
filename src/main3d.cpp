#include <iostream>
#include <memory>
#include <vector>

#include "cartpole3d.h"
#include "generation.h"
#include "neat_evaluator.h"
#include "renderer3d.h"
#include "organism.h"

constexpr int POP_SIZE = 50;
constexpr int EVAL_RUNS = 3;

int main() {
    std::cout << "NEAT Triple CartPole 3D" << std::endl;
    std::cout << "=======================" << std::endl;
    
    Renderer3D renderer(1200, 800);
    if(!renderer.init()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return 1;
    }
    
    CartPole3D env;
    Generation generation(POP_SIZE, env.getNumInputs(), env.getNumOutputs());
    
    std::cout << "Press ESC to quit, watch evolution!" << std::endl;
    
    while(!renderer.shouldQuit()) {
        std::cout << "\n--- Generation " << generation.getGenerationNumber() << " ---" << std::endl;
        
        for(auto& org : generation.getOrganisms()) {
            double totalSteps = 0;
            for(int run = 0; run < EVAL_RUNS; run++) {
                totalSteps += evaluateOrganism(org.get(), &env, env.MAX_STEPS);
            }
            org->setFitness(totalSteps / EVAL_RUNS);
        }
        
        generation.computeFitness();
        
        Organism* best = generation.getBestOrganism();
        
        if(best) {
            std::cout << "Best: " << (int)best->getFitness() << " | Avg: " << (int)generation.getAverageFitness() << std::endl;
            
            CartPole3D vizEnv;
            vizEnv.reset();
            while(!vizEnv.isDone()) {
                std::vector<double> state = vizEnv.getState();
                int action = best->act(state);
                vizEnv.step(action);
                renderer.render(vizEnv.getX(), vizEnv.getZ(), vizEnv.getTheta(), vizEnv.getPhi(),
                             generation.getGenerationNumber(), vizEnv.getSteps(), 
                             (int)best->getFitness(), (int)generation.getAverageFitness());
            }
            std::cout << "Survived: " << vizEnv.getSteps() << " steps" << std::endl;
        }
        
        if(generation.getBestFitness() >= env.MAX_STEPS) {
            std::cout << "\n*** SOLVED! ***" << std::endl;
            break;
        }
        
        generation.evolve();
    }
    
    renderer.close();
    return 0;
}
