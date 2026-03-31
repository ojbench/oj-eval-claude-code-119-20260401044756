#ifndef LINEARSCAN_HPP
#define LINEARSCAN_HPP

// don't include other headfiles
#include <string>
#include <vector>
#include <set>
#include <stack>

class Location {
public:
    // return a string that represents the location
    virtual std::string show() const = 0;
    virtual int getId() const = 0;
    virtual ~Location() {}
};

class Register : public Location {
private:
    int regId;
public:
    Register(int regId) : regId(regId) {
    }
    virtual std::string show() const {
        return "reg" + std::to_string(regId);
    }
    virtual int getId() const {
        return regId;
    }
};

class StackSlot : public Location {
public:
    StackSlot() {}
    virtual std::string show() const {
        return "stack";
    }
    virtual int getId() const {
        return -1;
    }
};

struct LiveInterval {
    int startpoint;
    int endpoint;
    Location* location = nullptr;
};

// Comparator for active set: sort by endpoint
struct EndpointComparator {
    bool operator()(const LiveInterval* a, const LiveInterval* b) const {
        return a->endpoint < b->endpoint;
    }
};

class LinearScanRegisterAllocator {
private:
    int regNum;
    std::set<LiveInterval*, EndpointComparator> active;
    std::stack<int> freeRegs; // Stack for FILO allocation

    void expireOldIntervals(LiveInterval& i) {
        // Iterate through active intervals in order of increasing endpoint
        auto it = active.begin();
        while (it != active.end()) {
            LiveInterval* j = *it;
            if (j->endpoint >= i.startpoint) {
                // No more expired intervals
                return;
            }
            // Remove j from active and add its register back to free pool
            it = active.erase(it);
            freeRegs.push(j->location->getId());
        }
    }

    void spillAtInterval(LiveInterval& i) {
        // Get the last interval in active (largest endpoint)
        auto it = active.rbegin();
        LiveInterval* spill = *it;

        if (spill->endpoint > i.endpoint) {
            // Spill the interval with the largest endpoint
            i.location = spill->location;
            spill->location = new StackSlot();
            active.erase(std::prev(active.end()));
            active.insert(&i);
        } else {
            // Spill i itself
            i.location = new StackSlot();
        }
    }

public:
    LinearScanRegisterAllocator(int regNum) : regNum(regNum) {
        // Initialize free registers stack (FILO)
        // Push in reverse order so that register 0 is on top
        for (int i = regNum - 1; i >= 0; i--) {
            freeRegs.push(i);
        }
    }

    void linearScanRegisterAllocate(std::vector<LiveInterval>& intervalList) {
        // Process each interval in order of increasing start point
        for (size_t idx = 0; idx < intervalList.size(); idx++) {
            LiveInterval& i = intervalList[idx];

            // Expire old intervals
            expireOldIntervals(i);

            // Check if we have free registers
            if (active.size() == static_cast<size_t>(regNum)) {
                // No free registers, need to spill
                spillAtInterval(i);
            } else {
                // Allocate a free register
                int regId = freeRegs.top();
                freeRegs.pop();
                i.location = new Register(regId);
                active.insert(&i);
            }
        }
    }
};

#endif
