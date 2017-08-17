#include <iostream>
#include <vector>

#include "network.h"
#include "network_utils.h"
#include "trainer.h"
#include "generation.h"

using namespace std;

int main() {
    Generation gen(3, 3, 100);
    std::cout << gen << std::endl;

    // int input_count = 3;
    // int output_count = 3;
    // InnovationTracker tracker;
    // for (int i = 0; i < input_count * output_count; i++) { tracker.new_innovation(); }
    // Network n1(input_count, output_count, &tracker);
    // Network n2(input_count, output_count, &tracker);
    //
    // std::cout << "Disjoint testing" << std::endl;
    //
    // std::cout << "Disjoint count between n1 and n2: " << n1.disjoint_count_with(n2) << std::endl;
    // std::cout << "Excess count between n1 and n2: " << n1.excess_count_with(n2) << std::endl;
    //
    // n2.disconnect(2, 5);
    // std::cout << "Disconnected 2-5 in n2" << std::endl;
    // std::cout << "Disjoint count between n1 and n2: " << n1.disjoint_count_with(n2) << std::endl;
    // std::cout << "Excess count between n1 and n2: " << n1.excess_count_with(n2) << std::endl;
    //
    // n2.disconnect(1, 4);
    // std::cout << "Disconnected 1-4 in n2" << std::endl;
    // std::cout << "Disjoint count between n1 and n2: " << n1.disjoint_count_with(n2) << std::endl;
    // std::cout << "Excess count between n1 and n2: " << n1.excess_count_with(n2) << std::endl;
    //
    // n2.connect(2, 5, 0.5);
    // std::cout << "Reconnected 2-5 in n2" << std::endl;
    // std::cout << "Disjoint count between n1 and n2: " << n1.disjoint_count_with(n2) << std::endl;
    // std::cout << "Excess count between n1 and n2: " << n1.excess_count_with(n2) << std::endl;
    //
    // n2.connect(1, 4, 0.5);
    // std::cout << "Reconnected 1-4 in n2" << std::endl;
    // std::cout << "Disjoint count between n1 and n2: " << n1.disjoint_count_with(n2) << std::endl;
    // std::cout << "Excess count between n1 and n2: " << n1.excess_count_with(n2) << std::endl;
    // std::cout << "N1: " << n1 << std::endl;
    // std::cout << "N2: " << n2 << std::endl;

    return 0;
}
