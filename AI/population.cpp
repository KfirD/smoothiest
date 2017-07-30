
#include <iostream>

#include "network.h"
#include "network_utils.h"
#include "population.h"

Network breed(const Network &nw1, const Network &nw2)
{
    Network child = nw1;
    Connections &conns1 = child.get_connections();
    const Connections &conns2 = nw2.get_connections();

    /*
     * Loop through each of parent2's connections doing:
     * - if parent2 has any connection that child doesn't, add it to child
     * - if parent2 has any disabled shared connections, disable it for child
     */
    for (const Connection &otherCon : conns2) {
        bool match = false;
        for (Connection &childCon : conns1) {
            if (otherCon.get_id() == childCon.get_id()) {
                match = true;
                if (!otherCon.enabled) childCon.enabled = false;
                break;
            }
        }

        if (!match)
            conns1.push_back(otherCon);
    }

    child.mutate();
    return child;
}
