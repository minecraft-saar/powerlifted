#ifndef SEARCH_OPEN_LISTS_GREEDY_OPEN_LIST_H_
#define SEARCH_OPEN_LISTS_GREEDY_OPEN_LIST_H_

#include "../search_engines/nodes.h"

#include <deque>
#include <map>

class GreedyOpenList {
    typedef std::deque<StateID> Bucket;

    std::map<int, Bucket> buckets;
    int size;

public:
    GreedyOpenList() : size(0) {}

    void do_insertion(const StateID &entry, int key) {
        buckets[key].push_back(entry);
        ++size;
    }

    StateID remove_min() {
        assert(size > 0);
        auto it = buckets.begin();
        assert(it != buckets.end());
        Bucket &bucket = it->second;
        assert(!bucket.empty());
        StateID result = bucket.front();
        bucket.pop_front();
        if (bucket.empty())
            buckets.erase(it);
        --size;
        return result;
    }

    bool empty() {
        return size == 0;
    }



};

#endif //SEARCH_OPEN_LISTS_GREEDY_OPEN_LIST_H_
