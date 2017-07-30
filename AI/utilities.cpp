#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unordered_map>

#include "utilities.h"

using namespace std;

double random_p() {
   static bool first = true;
   if (first) {
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      srand((time_t)ts.tv_nsec);
      first = false;
   }
   return ((double) rand() / RAND_MAX);
}

int random_big() {
   static bool first = true;
   if (first) {
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      srand((time_t)ts.tv_nsec);
      first = false;
   }
   return rand();
}
