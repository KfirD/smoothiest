#include <iostream>
#include <vector>

#include "network.h"
#include "network_utils.h"
#include "trainer.h"

using namespace std;

int main() {
   //Test population
   Trainer t;
   t.train(40);

   return 0;
}

// #define CATCH_CONFIG_MAIN
// #include "catch.h"
//
// TEST_CASE( "Network is checked", "[network]" ) {
//
// }
