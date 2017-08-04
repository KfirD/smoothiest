#include <iostream>
#include <vector>

#include "network.h"
#include "network_utils.h"
#include "trainer.h"

using namespace std;

int main() {
   Trainer t;
   t.train(5);

   // Test population;
   // Network n1(3,3);
   // Network n2(3,3);
   //
   // cout << n1 <<endl;
   // std::vector<double> output = n1.evaluate({1,1,1});
   // for (int i = 0; i < 6; i++) {
   //    cout << "#### input: " << 1 << ", output: " << output[i] << endl;
   // }
   // cout << n1 << endl;
   // for(int i = 0; i < 2; i++) {
   //    n1.mutate();
   //    std::vector<double> output = n1.evaluate({1,1,1});
   //    for (int i = 0; i < 3; i++) {
   //       cout << "#### input: " << 1 << ", output: " << output[i] << endl;
   //    }
   //    cout << n1 << endl;
   // }

   return 0;
}
