#include <iostream>

#include "network.h"
#include "network_utils.h"

using namespace std;

int main() {
   //input

   Network n(3, 4);
   n.connect(1, 2, 0);
   cout << n << "\n";

   return 0;
}
