#include <iostream>

#include "network.h"
#include "network_utils.h"
#include "population.h"

using namespace std;

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

   return 0;
}
