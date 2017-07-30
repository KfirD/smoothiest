
#include <iostream>

#include "network.h"
#include "network_utils.h"
#include "population.h"

Network breed(const Network &nw1, const Network &nw2)
{
    Network child = nw1;
    Connections &childCons = child.get_connections();
    const Connections &parentCons = nw2.get_connections();

    /*
     * Loop through each of parent2's connections doing:
     * - if parent2 has any connection that child doesn't, add it to child
     * - if parent2 has any disabled shared connections, disable it for child
     */
    for (const Connection &parentCon : parentCons) {
        bool match = false;
        for (Connection &childCon : childCons) {
            if (parentCon.get_id() == childCon.get_id()) {
                match = true;
                if (!parentCon.enabled) childCon.enabled = false;
                break;
            }
        }

        if (!match)
            childCons.push_back(parentCon);
    }

    child.mutate();
    return child;
}
