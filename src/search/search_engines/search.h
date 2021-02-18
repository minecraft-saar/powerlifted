#ifndef SEARCH_SEARCH_H
#define SEARCH_SEARCH_H

#include "search_space.h"
#include "utils.h"

#include "../search_statistics.h"
#include "../utils/system.h"

#include "../states/extensional_states.h"
#include "../states/sparse_states.h"
#include "../successor_generators/successor_generator.h"
#include "../task.h"

#include <utility>
#include <vector>
#include <iostream>
#include <queue>

// Forward declarations
class SuccessorGenerator;
class Heuristic;
class Task;


class SearchBase {
public:
    SearchBase() = default;
    virtual ~SearchBase() = default;

    virtual utils::ExitCode search(const Task &task,
                       SuccessorGenerator &generator,
                       Heuristic &heuristic) = 0;

    virtual void print_statistics() const = 0;

    template <class PackedStateT>
    bool check_goal(const Task &task,
                    const SuccessorGenerator &generator,
                    clock_t timer_start,
                    const DBState &state,
                    const SearchNode &node,
                    const SearchSpace<PackedStateT> &space) const {
        if (!task.is_goal(state)) return false;
        std::cout << "Number of Predicate Landmarks in goal: " << state.num_of_predicate_landmarks() << " Number of fullfilled goal Landmarks: " << state.num_of_fullfilled_goals() << std::endl;
        if(state.num_of_predicate_landmarks()-state.num_of_fullfilled_goals()!=0){
            for(auto landmark : state.predicate_landmarks){
                std::cout << landmark.second.name << " " ;
                for(auto arg : landmark.second.arguments){
                    std::cout << arg.index << " ";
                }
                std::cout << " is fullfilled: " << landmark.second.was_true_last_step << " is Goal: " << landmark.second.is_goal << " num of greedy effects : " << landmark.second.num_of_greedy_effects << std::endl ;
            }
            int unfullfilled = state.num_of_predicate_landmarks()-state.num_of_fullfilled_goals();
            std::cout << unfullfilled << " Unfullfilled Landmarks in Goal" << std::endl;
            exit(-1);
        }
        print_goal_found(generator, timer_start);
        auto plan = space.extract_plan(node);
        print_plan(plan, task);
        return true;
    }

protected:

    SearchStatistics statistics;

};

#endif //SEARCH_SEARCH_H
