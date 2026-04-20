#ifndef INNOVATION_TRACKER_H
#define INNOVATION_TRACKER_H

#include <unordered_map>
#include <utility>

struct pair_hash {
    size_t operator()(const std::pair<int, int>& p) const {
        return (static_cast<size_t>(p.first) << 32) | static_cast<size_t>(p.second);
    }
};

class InnovationTracker {
public:
    InnovationTracker();

    int getInnovation(int inNode, int outNode);
    int getNewInnovation(int inNode, int outNode);

    static InnovationTracker& instance();

private:
    int nextInnovation;
    std::unordered_map<std::pair<int, int>, int, pair_hash> innovationMap;
};

#endif
