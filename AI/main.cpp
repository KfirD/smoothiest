#include <iostream>
#include <vector>

#include "network.h"
#include "network_utils.h"
#include "trainer.h"
#include "generation.h"

using namespace std;

int main() {
    Trainer t;
    t.train(60);

    return 0;
}
