#include <iostream>
#include <vector>

#include "network.h"
#include "network_utils.h"
#include "trainer.h"

using namespace std;

int main() {
    Trainer t;
    t.train(40);

    // std::cout << "Before" << std::endl;
    // Network n(2, 2);
    // int new_neuron = n.add_new_neuron();
    //
    // std::cout << std::endl << "BUGGY SHIT STARTS HERE" << std::endl;
    //
    // n.disconnect(0, 2);
    // n.disconnect(0, 3);
    // n.connect(0, new_neuron, 0.5);
    //
    // std::cout << "BUGGY SHIT ENDS HERE" << std::endl;
    //
    // std::cout << std::endl << "After" << std::endl;
    // for (auto &con : n.get_connections()) {
    //     std::cout << con << std::endl;
    // }

    return 0;
}

// #define CATCH_CONFIG_MAIN
// #include "catch.h"
//
// TEST_CASE( "Network is checked", "[network]" ) {
//
// }
