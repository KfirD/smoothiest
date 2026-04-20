
#include <iostream>

#include "network.h"
#include "network_utils.h"
#include "population.h"

Network breed(const Network& nw1, const Network& nw2) {
    Network child = nw1;
    Connections& childCons = child.getConnections();
    const Connections& parentCons = nw2.getConnections();

    for(const Connection& parentCon : parentCons) {
        bool match = false;
        for(Connection& childCon : childCons) {
            if(parentCon.get_id() == childCon.get_id()) {
                match = true;
                if(!parentCon.enabled) childCon.disable();
                break;
            }
        }

        if(!match) {
            childCons.push_back(parentCon);
        }
    }

    child.mutate(0.33, 0.33, 0.34);
    return child;
}
