#include "innovation_tracker.h"

InnovationTracker::InnovationTracker() : nextInnovation(1) {}

int InnovationTracker::getInnovation(int inNode, int outNode) {
    std::pair<int, int> key = std::make_pair(inNode, outNode);
    auto it = innovationMap.find(key);
    if(it != innovationMap.end()) {
        return it->second;
    }
    return getNewInnovation(inNode, outNode);
}

int InnovationTracker::getNewInnovation(int inNode, int outNode) {
    std::pair<int, int> key = std::make_pair(inNode, outNode);
    int inn = nextInnovation++;
    innovationMap[key] = inn;
    return inn;
}

InnovationTracker& InnovationTracker::instance() {
    static InnovationTracker tracker;
    return tracker;
}
