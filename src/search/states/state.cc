
#include "state.h"
#include "../action.h"

#include <iostream>

#include <boost/functional/hash.hpp>

using namespace std;

void DBState::add_tuple(int relation, const GroundAtom &args) {
    relations[relation].tuples.insert(args);
}

std::size_t hash_value(const DBState &s) {
    std::size_t seed = 0;
    for (bool b : s.nullary_atoms) {
        boost::hash_combine(seed, b);
    }
    for (const Relation &r : s.relations) {
        std::vector<std::size_t> x;
        for (const GroundAtom &vga : r.tuples) {
            std::size_t aux_seed = vga.size();
            for (auto &i : vga)
                aux_seed ^= i + 0x9e3779b9 + (aux_seed << 6) + (aux_seed >> 2);
            x.push_back(aux_seed);
            //boost::hash_combine(seed, vga);
        }
        std::sort(x.begin(), x.end());
        for (std::size_t e : x) {
            boost::hash_combine(seed, e);
        }
    }
    return seed;
}

void DBState::set_landmarks(DBState parent, ActionSchema action, const LiftedOperatorId &op_id) {
    this->predicate_landmarks.insert(parent.predicate_landmarks.begin(), parent.predicate_landmarks.end());

    std::vector<int> to_delete;
    std::vector<int> to_add;
    for (auto &pair_index_lm : predicate_landmarks) {
        LandmarkObj &landmark = pair_index_lm.second;
        bool needs_check = landmark.needs_check;
        if (!needs_check && !pair_index_lm.second.was_checked_this_round && landmark.can_have_greedy_effects) {
            //we don't actually care about the presence right now, but if landmark is reinserted we might care about was_true value
            num_of_fullfilled_goal_lms++;
            pair_index_lm.second.is_true_now = check_presence_of_fact_lm(pair_index_lm.second);
            pair_index_lm.second.was_checked_this_round = true;
            continue;
        }
        if (!needs_check) {
            if (landmark.num_of_natural_effects == 0 && landmark.num_of_reasonable_effects == 0) {
                if(!landmark.can_have_greedy_effects){
                    to_delete.push_back(landmark.lm_index);
                }
            } else {
                num_of_fullfilled_goal_lms++;
            }
            continue;
        }
        bool is_present = false;
        if (landmark.is_action) {
            is_present = action_lm_equal_to_action(landmark, action, op_id);
        } else {
            is_present = check_presence_of_fact_lm(landmark);
        }
        landmark.was_checked_this_round = true;
        if (is_present) {
            pair_index_lm.second.is_true_now = true;
            // We meed to do some maintance of greedy effects if the landmark was false before and became true
            // If this was the only (remaining) effect the precondition had we now no longer need to check the precon
            // If the precon was already checked this loop and found false it could have reinserted its own precons,
            // since it (thought it) needs to become true again, this may no longer be the case, so we need to clean up
            if (landmark.precons_greedy_order.has_value() && !landmark.was_true_last_step) {
                std::vector<int> precons = landmark.precons_greedy_order.value();
                for (auto precon_landmark : precons) {
                    auto &found_precon = predicate_landmarks.find(precon_landmark)->second;
                    found_precon.num_of_greedy_effects--;
                    if (found_precon.num_of_greedy_effects == 0) {
                        found_precon.needs_check = false;
                        if (!found_precon.was_checked_this_round) {
                            found_precon.was_checked_this_round = true;
                            num_of_fullfilled_goal_lms++;
                        } else {
                            //was checked and found false, so added its precons to reinsert list, need to rectify this
                            if (!found_precon.is_true_now) {
                                if (found_precon.precons_greedy_order.has_value()) {
                                    for (auto elem : found_precon.precons_greedy_order.value()) {
                                        auto it = std::find(to_add.begin(), to_add.end(), elem);
                                        if (it != to_add.end()) {
                                            predicate_landmarks.at(elem).num_of_greedy_effects--;
                                            to_add.erase(it);
                                        }
                                    }
                                }
                                num_of_fullfilled_goal_lms++;
                            }
                        }
                        //to_delete.push_back(precon_landmark);
                    }
                }
            }

            //if the landmrk still has greedy precons we need to keep it since greedy effects can only become true if their premise was true in the last step
            if (landmark.num_of_greedy_effects != 0) {
                num_of_fullfilled_goal_lms++;
                continue;
            }
            if (landmark.num_of_natural_effects != 0 && !landmark.is_goal) {
                //need to keep the landmark for now to check the effects, but we don't care about it becoming false again
                //so we don't need to check it in future states
                landmark.needs_check = false;
                num_of_fullfilled_goal_lms++;
                continue;
            }
            if (landmark.num_of_reasonable_effects != 0 && !landmark.is_goal) {
                //need to keep the landmark for now to check the effects, but we don't care about it becoming false again
                //so we don't need to check it in future states
                landmark.needs_check = false;
                num_of_fullfilled_goal_lms++;
                continue;
            }

            //Goal Landmarks are never deleted
            if (landmark.is_goal) {
                num_of_fullfilled_goal_lms++;
            } else {
                //this landmark is true but is neither a goal nor has any effects
                to_delete.push_back(pair_index_lm.first);
            }
        } else {
            //landmark is not present
            if (landmark.was_true_last_step) {
                if (landmark.precons_greedy_order.has_value()) {
                    for (auto precons_index : landmark.precons_greedy_order.value()) {
                        std::vector<int> tmp = check_precons_for_reinsertion(predicate_landmarks.at(precons_index),
                                                                             to_add);
                        for (auto elem : tmp) {
                            to_add.push_back(elem);
                        }
                    }
                }
            }
            pair_index_lm.second.is_true_now = false;
        }
    }

    // delete landmarks that were keep because they had effects
    // and landmarks that don't have effects and simply became true
    for (auto &landmark : to_delete) {
        auto it = find(to_add.begin(), to_add.end(), landmark);
        if (it != to_add.end()) {
            continue;
        }
        predicate_landmarks.erase(landmark);
    }
    to_delete.clear();
    for (auto &landmark : to_add) {
        predicate_landmarks.find(landmark)->second.needs_check = true;
    }
    //update truth values for future use
    for (auto &pair : predicate_landmarks) {
        pair.second.was_true_last_step = pair.second.is_true_now;
        pair.second.is_true_now = false;
        pair.second.was_checked_this_round = false;
    }
}

void DBState::set_initial_landmarks(std::unordered_map<int, LandmarkObj> input_predicate_landmarks) {
    std::vector<int> reduce_natural_precons;
    std::vector<int> reduce_reasonable_precons;
    for (auto pair_index_lm : input_predicate_landmarks) {
        LandmarkObj landmark = pair_index_lm.second;
        //check reasonable orderings cannot be done inside check_presence_of_factlm since actionLms can be precon

        bool is_present;
        if (pair_index_lm.second.is_action) {
            is_present = false;
        } else {
            if (pair_index_lm.second.precons_greedy_order.has_value()) {
                if (pair_index_lm.second.precons_greedy_order->size() != 0) {
                    is_present = false;
                } else {
                    is_present = check_presence_of_fact_lm(pair_index_lm.second);
                }
            } else {
                is_present = check_presence_of_fact_lm(pair_index_lm.second);

            }
        }
        //greedy necessary ordering
        if ((is_present && pair_index_lm.second.is_goal) ||
            (is_present && pair_index_lm.second.num_of_greedy_effects != 0)) {
            num_of_fullfilled_goal_lms++;
            pair_index_lm.second.is_true_now = true;
            //pair_index_lm.second.needs_check = false;
            predicate_landmarks.insert(pair_index_lm);

        } else if (is_present) {
            pair_index_lm.second.is_true_now = true;
            if (pair_index_lm.second.precons_nat_order.has_value()) {
                std::vector effects = pair_index_lm.second.precons_nat_order.value();
                for (auto lm : effects) {
                    reduce_natural_precons.push_back(lm);
                }
                pair_index_lm.second.precons_nat_order->clear();
            }
            if (pair_index_lm.second.precons_reasonable_order.has_value()) {
                std::vector effects = pair_index_lm.second.precons_reasonable_order.value();
                for (auto lm : effects) {
                    reduce_reasonable_precons.push_back(lm);
                }
                pair_index_lm.second.precons_reasonable_order->clear();
            }

            if (pair_index_lm.second.is_goal) {
                num_of_fullfilled_goal_lms++;
                predicate_landmarks.insert(pair_index_lm);
            }
        }
        if (!is_present) {
            predicate_landmarks.insert(pair_index_lm);
        }
    }

    for (auto &lm_pair : predicate_landmarks) {
        lm_pair.second.was_true_last_step = lm_pair.second.is_true_now;
        lm_pair.second.is_true_now = false;
    }
    for (auto index : reduce_natural_precons) {
        predicate_landmarks.find(index)->second.num_of_natural_effects--;
    }
    for (auto index : reduce_reasonable_precons) {
        predicate_landmarks.find(index)->second.num_of_reasonable_effects--;
    }

    cout << "Initiale Fact Landmarken gesetzt. Anzahl: " << num_of_predicate_landmarks() << endl;


}

bool DBState::check_presence_of_fact_lm(LandmarkObj factLM) {
    bool andOrFactLm = factLM.other_preds.has_value();
    bool precons_present = false;
    if (factLM.precons_nat_order.has_value()) {
        std::vector<int> precons = factLM.precons_nat_order.value();
        std::vector<int> precons_to_delete;
        for (auto landmark : precons) {
            if (predicate_landmarks.at(landmark).was_true_last_step) {
                predicate_landmarks.at(landmark).num_of_natural_effects--;
                precons_to_delete.push_back(landmark);
            }
        }
        for (auto elem : precons_to_delete) {
            auto it = std::remove(predicate_landmarks.at(factLM.lm_index).precons_nat_order->begin(),
                                  predicate_landmarks.at(factLM.lm_index).precons_nat_order->end(), elem);
            predicate_landmarks.at(factLM.lm_index).precons_nat_order->erase(it);
        }
        if (!predicate_landmarks.at(factLM.lm_index).precons_nat_order->empty()) {
            precons_present = true;
        }
    }

    if (factLM.precons_reasonable_order.has_value()) {
        std::vector<int> precons = factLM.precons_reasonable_order.value();
        std::vector<int> precons_to_delete;
        for (auto landmark : precons) {
            if (predicate_landmarks.at(landmark).was_true_last_step) {
                predicate_landmarks.at(landmark).num_of_reasonable_effects--;
                precons_to_delete.push_back(landmark);
            }
        }
        for (auto elem : precons_to_delete) {
            auto it = std::remove(predicate_landmarks.at(factLM.lm_index).precons_reasonable_order->begin(),
                                  predicate_landmarks.at(factLM.lm_index).precons_reasonable_order->end(), elem);
            predicate_landmarks.at(factLM.lm_index).precons_reasonable_order->erase(it);
        }
        if (!predicate_landmarks.at(factLM.lm_index).precons_reasonable_order->empty()) {
            precons_present = true;
        }
    }

    if (precons_present) {
        return false;
    }

    //check for predicate with no arguments
    if (factLM.arguments.empty()) {
        bool match;
        if (factLM.negated) {
            match = !nullary_atoms[factLM.pred_index];
        } else {
            match = nullary_atoms[factLM.pred_index];
        }
        if (match) {
            return true;
        } else {
            if (andOrFactLm) {
                if (!factLM.isAnd()) { //check OR Predicates
                    std::vector otherPreds = factLM.other_preds.value();
                    for (auto &landmark : otherPreds) {
                        if (check_presence_of_fact_lm(landmark)) {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
    }

    //Predicate has arguments
    const auto &tuples = relations[factLM.pred_index].tuples;
    for (auto &possibleInstant : tuples) {
        bool match = true;
        for (unsigned int i = 0; i < possibleInstant.size(); i++) {
            if (!factLM.arguments[i].constant) {
                continue;
            }
            if (factLM.arguments[i].index != possibleInstant[i]) {
                match = false;
            }
        }
        if (match) {
            if (andOrFactLm) {
                if (factLM.isAnd()) {
                    std::vector otherPreds = factLM.other_preds.value();
                    for (auto lm : otherPreds) {
                        //as soon as one is false we break, so no need to && with old value
                        match = check_presence_of_fact_lm(lm);
                        if (!match) {
                            break;
                        }
                    }
                }
            }
            //if I have already achieved this I don't care about preconditions
            if (!factLM.was_true_last_step) {
                if (factLM.precons_greedy_order.has_value()) {
                    std::vector precons = factLM.precons_greedy_order.value();
                    for (auto landmark : precons) {
                        if (!predicate_landmarks.find(landmark)->second.was_true_last_step) {
                            return false;
                        }
                    }
                }
            }
            return match;
        }
    }

    //this landmark is not present but check if it is an OR Landmark
    if (andOrFactLm) {
        if (!factLM.isAnd()) {
            std::vector<LandmarkObj> otherPreds = factLM.other_preds.value();
            for (auto &landmark : otherPreds) {
                if (check_presence_of_fact_lm(landmark)) {
                    return true;
                }
            }
        }
    }
    return false;

}

bool DBState::action_lm_equal_to_action(LandmarkObj actionLm, ActionSchema action, LiftedOperatorId grounded_action) {
    bool precons_present = false;
    if (actionLm.precons_nat_order.has_value()) {
        std::vector<int> precons = actionLm.precons_nat_order.value();
        std::vector<int> precons_to_delete;
        for (auto landmark : precons) {
            if (predicate_landmarks.at(landmark).was_true_last_step) {
                predicate_landmarks.at(landmark).num_of_natural_effects--;
                precons_to_delete.push_back(landmark);
            }
        }
        for (auto elem : precons_to_delete) {
            auto it = std::remove(predicate_landmarks.at(actionLm.lm_index).precons_nat_order->begin(),
                                  predicate_landmarks.at(actionLm.lm_index).precons_nat_order->end(), elem);
            predicate_landmarks.at(actionLm.lm_index).precons_nat_order->erase(it);
        }
        if (!predicate_landmarks.at(actionLm.lm_index).precons_nat_order->empty()) {
            precons_present = true;
        }
    }

    if (actionLm.precons_reasonable_order.has_value()) {
        std::vector<int> precons = actionLm.precons_reasonable_order.value();
        std::vector<int> precons_to_delete;
        for (auto landmark : precons) {
            if (predicate_landmarks.at(landmark).was_true_last_step) {
                predicate_landmarks.at(landmark).num_of_natural_effects--;
                precons_to_delete.push_back(landmark);
            }
        }
        for (auto elem : precons_to_delete) {
            auto it = std::remove(predicate_landmarks.at(actionLm.lm_index).precons_reasonable_order->begin(),
                                  predicate_landmarks.at(actionLm.lm_index).precons_reasonable_order->end(), elem);
            predicate_landmarks.at(actionLm.lm_index).precons_reasonable_order->erase(it);
        }
        if (!predicate_landmarks.at(actionLm.lm_index).precons_reasonable_order->empty()) {
            precons_present = true;
        }
    }
    if (precons_present) {
        return false;
    }

    if (actionLm.precons_greedy_order.has_value()) {
        std::vector precons = actionLm.precons_greedy_order.value();
        if (!precons.empty()) {
            std::cout << "I don't think actions can have greedy precons" << std::endl;
            exit(-1);
            //if (!predicate_landmarks.find(landmark)->second.was_true_last_step) {
            //    return false;
            //}
        }
    }

    if (actionLm.arity == action.get_parameters().size()) {
        if (actionLm.name == action.get_name() && actionLm.pred_index == action.get_index()) {
            for (unsigned int i = 0; i < actionLm.arity; i++) {
                if (!actionLm.arguments[i].constant) {
                    continue;
                }
                if (actionLm.arguments[i].index != grounded_action.get_instantiation()[i]) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

std::vector<int> DBState::check_precons_for_reinsertion(LandmarkObj lm, std::vector<int> to_add) {
    if (lm.is_action) {
        //something strange is happening, actions should not be greedy preconditions
        std::cout << "action is greedy precondition!" << std::endl;
        exit(-1);
        /*lm.num_of_greedy_effects++;
        to_add.push_back(lm);
        if (lm.precons_greedy_order.has_value()) {
            std::vector<LandmarkObj *> precons = lm.precons_greedy_order.value();
            for (auto precon_landmark : precons) {
                check_precons_for_reinsertion(*precon_landmark, to_add);
            }
        }
        return to_add;*/
    } else {
        auto it = predicate_landmarks.find(lm.lm_index);
        if (it != predicate_landmarks.end()) {
            it->second.num_of_greedy_effects++;
        } else {
            std::cout << "Problem in check Precons" << endl;
            exit(-1);
        }
        to_add.push_back(lm.lm_index);
        bool is_present = check_presence_of_fact_lm(lm);
        if (!is_present) {
            if (lm.precons_greedy_order.has_value()) {
                std::vector<int> precons = lm.precons_greedy_order.value();
                for (auto precon_landmark : precons) {
                    auto precon_pair = predicate_landmarks.find(precon_landmark);
                    std::vector<int> tmp = check_precons_for_reinsertion(precon_pair->second, to_add);
                    for (auto elem : tmp) {
                        to_add.push_back(elem);
                    }
                }
            }
        }
        return to_add;
    }

}


bool DBState::fact_lm_equal_to_ground_effect(LandmarkObj factLM, ActionSchema action,
                                             const LiftedOperatorId &grounded_action) {
    //cout << "checking fact equal to ground effect" << endl;
    bool orderedFactLm = factLM.precons_greedy_order.has_value();
    //bool andOrFactLm = factLM.other_preds.has_value();
    if (orderedFactLm) {
        std::vector precons = factLM.precons_greedy_order.value();
        for (auto landmark : precons) {
            if (!predicate_landmarks.find(landmark)->second.was_true_last_step) {
                return false;
            }
        }
    }

    if (factLM.arguments.empty()) {
        if (factLM.negated) {
            return action.get_negative_nullary_effects().at(factLM.pred_index);
        } else {
            return action.get_positive_nullary_effects().at(factLM.pred_index);
        }
    }

    for (auto effect : action.get_effects()) {
        if (effect.name == factLM.name) {
            if (effect.negated == factLM.negated) {
                if (effect.arguments.size() == factLM.arguments.size()) {
                    bool match = true;
                    for (unsigned int i = 0; i < effect.arguments.size(); i++) {
                        if (!factLM.arguments[i].constant) {
                            continue;
                        }
                        Argument arg = effect.arguments[i];
                        int objIndex = arg.index;
                        if (!arg.constant) {
                            objIndex = grounded_action.get_instantiation()[arg.index];
                        }
                        if (objIndex != factLM.arguments[i].index) {
                            match = false;
                        }
                    }
                    /*if (match && orderedFactLm) {
                        std::vector precons_nat_order = factLM.precons_nat_order.value();
                        for (auto *landmark : precons_nat_order) {
                            landmark->removePrecon();
                        }
                    }*/
                    return match;
                    /*cout << "landmark found equal effect " << effect.name << endl;
                    for (Argument arg : effect.arguments) {
                        std::string text = "Argument is constant: " + std::to_string(arg.constant);
                        text = text + " Index of Argument: ";
                        text = text + std::to_string(arg.index);
                        cout << text << endl;
                    }*/
                }
            }
        }
    }
    return false;
}


