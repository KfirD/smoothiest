
#include "innovation_tracker.h"

InnovationTracker::InnovationTracker(): innovation_count(0) {}

int InnovationTracker::new_innovation()
{
    return innovation_count++;
}
