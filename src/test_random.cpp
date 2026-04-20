#include <iostream>
#include <cstdlib>
#include <vector>
#include "cartpole.h"

int main() {
    srand(time(NULL));
    int total = 0;
    for(int i = 0; i < 100; i++) {
        CartPole cp;
        cp.reset();
        int steps = 0;
        while(!cp.isDone() && steps < CartPole::MAX_STEPS) {
            int action = rand() % 2;
            cp.step(action);
            steps++;
        }
        total += steps;
    }
    std::cout << "Average steps with random: " << total / 100 << std::endl;
    return 0;
}
