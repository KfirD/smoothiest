#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>

#include "network_utils.h"

using namespace std;

class Network {
public:
   Network(int num_in, int num_out);
   const Connections& get_connections() const;
   Connections& get_connections();
   bool connect(int in, int out, double weight);
   void mutate();
   //void mutate();

private:
   const int num_in;
   const int num_out;
   Neurons neurons;
   Connections connections;
};

std::ostream &operator<<(std::ostream &out, const Network &concs);

#endif
