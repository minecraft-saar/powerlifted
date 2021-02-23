#include "greedy_best_first_search.h"
#include "search.h"
#include "utils.h"
#include "../action.h"
#include "../task.h"
#include "../heuristics/heuristic.h"
#include "../open_lists/greedy_open_list.h"
#include "../states/extensional_states.h"
#include "../states/sparse_states.h"
#include "../successor_generators/successor_generator.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

template <class PackedStateT>
utils::ExitCode GreedyBestFirstSearch<PackedStateT>::search(const Task &task,
                                                SuccessorGenerator &generator,
                                                Heuristic &heuristic)
{
    cout << "Starting greedy best first search" << endl;
    clock_t timer_start = clock();
    StatePackerT packer(task);

    GreedyOpenList queue;

    SearchNode& root_node = space.insert_or_get_previous_node(packer.pack(task.initial_state), LiftedOperatorId::no_operator, StateID::no_state);
    heuristic_layer = heuristic.compute_heuristic(task.initial_state, task);
    root_node.open(0, heuristic_layer);
    cout << "Initial heuristic value " << heuristic_layer << endl;
    statistics.report_f_value_progress(heuristic_layer);
    queue.do_insertion(root_node.state_id, make_pair(heuristic_layer, 0));

    if (check_goal(task, generator, timer_start, task.initial_state, root_node, space)) return utils::ExitCode::SUCCESS;

    while (not queue.empty()) {
        StateID sid = queue.remove_min();
        SearchNode &node = space.get_node(sid);
        int h = node.h;
        int g = node.g;
        if (node.status == SearchNode::Status::CLOSED) {
            continue;
        }
        //cout << "Node is being expanded, heuristic value: " << h << endl;
        //node.close();
        statistics.report_f_value_progress(h); // In GBFS f = h.
        statistics.inc_expanded();

        if (h < heuristic_layer) {

            heuristic_layer = h;
            cout << "New heuristic value expanded: h=" << h
                 << " [expansions: " << statistics.get_expanded()
                 << ", evaluations: " << statistics.get_evaluations()
                 << ", generations: " << statistics.get_generated()
                 << ", time: " << double(clock() - timer_start) / CLOCKS_PER_SEC << "]" << '\n';
            auto plan = space.extract_plan(node);
            /*for (const LiftedOperatorId &a:plan) {
                std::cout << task.actions[a.get_index()].get_name() << " ";
                for (const int obj : a.get_instantiation()) {
                    std::cout << task.objects[obj].getName() << " ";
                }
                std::cout << std::endl;
            }*/
        }
        node.close();
        assert(sid.id() >= 0 && (unsigned) sid.id() < space.size());

        DBState state = packer.unpack(space.get_state(sid));
        if (check_goal(task, generator, timer_start, state, node, space)) return utils::ExitCode::SUCCESS;

        // Let's expand the state, one schema at a time. If necessary, i.e. if it really helps
        // performance, we could implement some form of std iterator
        for (const auto& action:task.actions) {
            auto applicable = generator.get_applicable_actions(action, state);
            statistics.inc_generated(applicable.size());

            for (const LiftedOperatorId& op_id:applicable) {
                if(action.get_name() == "pickup" && op_id.get_instantiation()[0] == 1){
                    std::cout << "picking up block 1" << std::endl;
                }

                if(action.get_name() == "pickup" && op_id.get_instantiation()[0] == 2){
                    std::cout << "picking up block 2" << std::endl;
                }

                if(action.get_name() == "stack" && op_id.get_instantiation()[0] == 2 && op_id.get_instantiation()[1] == 1){
                    std::cout << "stacking block 2 on block 1" << std::endl;
                }
                DBState s = generator.generate_successor(op_id, action, state);
                //handeling landmark tracking
                if(task.using_landmarks) {
                    s.set_landmarks(state, action,
                                    op_id); // op_id beinhaltet die IDs für die Action und die gegründeten Parameter
                }
                int dist = g + action.get_cost();
                int new_h = heuristic.compute_heuristic(s, task);
                statistics.inc_evaluations();

                if(action.get_name() == "pickup" && op_id.get_instantiation()[0] == 1){
                    std::cout << " h value for picking up block 1: " << new_h << std::endl;
                }
                if(action.get_name() == "pickup" && op_id.get_instantiation()[0] == 2){
                    std::cout << " h value for picking up block 2: " << new_h << std::endl;
                }

                auto& child_node = space.insert_or_get_previous_node(packer.pack(s), op_id, node.state_id);
                if (child_node.status == SearchNode::Status::NEW) {
                    // Inserted for the first time in the map
                    child_node.open(dist, new_h);
                    queue.do_insertion(child_node.state_id, make_pair(new_h, dist));
                }
                else {
                    if (dist < child_node.g) {
                        child_node.open(dist, new_h); // Reopening
                        statistics.inc_reopened();
                        queue.do_insertion(child_node.state_id, make_pair(new_h, dist));
                    }
                }
            }
        }
    }

    print_no_solution_found(timer_start);

    return utils::ExitCode::SEARCH_UNSOLVABLE;
}

template <class PackedStateT>
void GreedyBestFirstSearch<PackedStateT>::print_statistics() const {
    statistics.print_detailed_statistics();
    space.print_statistics();
}

// explicit template instantiations
template class GreedyBestFirstSearch<SparsePackedState>;
template class GreedyBestFirstSearch<ExtensionalPackedState>;