//
// Created by julia on 02.12.20.
//

#include "landmark_heuristic.h"
#include "../states/state.h"
#include <iostream>

int landmark_heuristic::compute_heuristic(const DBState &s, const Task &task) {
    int predicate_landmarks = s.num_of_predicate_landmarks();
    int fullfilled_goal_lms = s.num_of_fullfilled_goals();
    //std::cout << "num of fullfilled goals: " << fullfilled_goal_lms << " Costs for FactLMs: " << predicate_landmarks << std::endl;
    return predicate_landmarks-fullfilled_goal_lms;
}
