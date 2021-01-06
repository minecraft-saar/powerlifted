//
// Created by julia on 02.12.20.
//

#ifndef SEARCH_LANDMARK_HEURISTIC_H
#define SEARCH_LANDMARK_HEURISTIC_H


#include "heuristic.h"

class landmark_heuristic : public Heuristic {
public:
    int compute_heuristic(const DBState & s, const Task& task) final;
};


#endif //SEARCH_LANDMARK_HEURISTIC_H
