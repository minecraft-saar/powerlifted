//
// Created by julia on 02.12.20.
//

#include "landmark_heuristic.h"
#include "../states/state.h"
#include <iostream>

int landmark_heuristic::compute_heuristic(const DBState &s, const Task &task) {
    int action_landmarks = s.num_of_action_landmarks();
    int predicate_landmarks = s.num_of_predicate_landmarks();
    std::cout << "Costs in Heuristic for ActionLMs: " << action_landmarks << " Costs for FactLMs: " << predicate_landmarks << std::endl;
    return action_landmarks+predicate_landmarks;
}
