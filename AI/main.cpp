#include <iostream>
#include <vector>

#include "network.h"
#include "network_utils.h"
#include "population.h"

using namespace std;

double dummy_feedback(vector<double> &inputs, vector<double>& outputs) {
   int tot = 0;
   for(int i: outputs) tot+=i;
   return tot;
}

int main() {
   //input

   cout << "Network #1" << endl;
   Network n1(2, 2);
   n1.connect(0, 2, 0.1);
   n1.connect(1, 3, 0.3);
   n1.connect(0, 3, 0.15);
   cout << n1 << endl;

   cout << "Network #2" << endl;
   Network n2(2, 2);
   n2.connect(0, 2, 0.1);
   n2.connect(1, 3, 0.3);
   n2.connect(1, 2, 0.5);
   cout << n2 << endl;

   cout << "Network #3" << endl;
   Network n3 = breed(n1, n2);
   cout << n3 << endl;

   // Create test network inputs
   std::vector<double> network_inputs;
   network_inputs.push_back(1);
   network_inputs.push_back(2);

   // Calculate network outputs
   const std::vector<double> network_output(n3.evaluate(network_inputs));
   for (double val : network_output) {
       cout << "Output: " << val << endl;
   }

   //Test population

   Population p(4,4, 10, &dummy_feedback);
   p.run_generation();

   return 0;
}
