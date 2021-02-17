
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
    for (auto &landmark : parent.predicate_landmarks)
        this->predicate_landmarks.push_back(landmark);

    std::vector<LandmarkObj *> to_delete;
    std::vector<LandmarkObj> to_add;
    for (auto landmark = predicate_landmarks.begin(); landmark < predicate_landmarks.end();) {
        bool needs_check = true;
        bool is_present;
        //check reasonable orderings cannot be done inside check_presence_of_factlm since actionLms can be precon
        if (landmark->precons_reasonable_order.has_value()) {
            std::vector precons = landmark->precons_reasonable_order.value();
            for (auto precon_landmark = precons.begin(); precon_landmark < precons.end();) {
                bool precon_is_present;
                LandmarkObj *lm = *precon_landmark;
                if (lm->is_action) {
                    precon_is_present = action_lm_equal_to_action(*landmark, action, op_id);;
                } else {
                    precon_is_present = check_presence_of_fact_lm(*lm);
                }
                if (precon_is_present) {
                    precon_landmark = precons.erase(precon_landmark);
                } else {
                    precon_landmark++;
                }
            }
            if (!precons.empty()) {
                needs_check = false;
                is_present = false;
            }
        }
        if (needs_check) {
            if (landmark->is_action) {
                is_present = action_lm_equal_to_action(*landmark, action, op_id);
            } else {
                is_present = check_presence_of_fact_lm(*landmark);
            }
        }

        if (is_present) {
            landmark->is_true_now = true;
            // If the landmark has preconditions, we can now remove them from our list
            // To not break the current for-loop we add them to to_delete vector and delete them after the loop
            if (landmark->precons_greedy_order.has_value()) {
                if(!landmark->was_true_last_step){
                    std::vector<LandmarkObj *> precons = landmark->precons_greedy_order.value();
                    for (auto precon_landmark : precons) {
                        precon_landmark->num_of_greedy_effects--;
                        if (precon_landmark->num_of_greedy_effects == 0) {
                            to_delete.push_back(precon_landmark);
                        }
                    }
                }
            }
            //if the landmrk still has effects_nat_order we need to keep it since effects_nat_order can only become true if their premise was true in the last step
            if (landmark->num_of_greedy_effects != 0) {
                num_of_fullfilled_goal_lms++;
                landmark++;
                continue;
            }

            if (landmark->effects_nat_order.has_value()) {
                std::vector<LandmarkObj *> effects = landmark->effects_nat_order.value();
                for (auto lm : effects) {
                    lm->num_of_natural_precons--;
                }
                landmark->effects_nat_order->clear();
            }


            //Goal Landmarks are never deleted
            if (landmark->is_goal) {
                num_of_fullfilled_goal_lms++;
                landmark++;
            } else {
                //this landmark is true but is neither a goal nor has any effects_nat_order
                landmark = predicate_landmarks.erase(landmark);
            }
            //landmark is not present
        } else {
            if(landmark->was_true_last_step){
                if (landmark->precons_greedy_order.has_value()) {

                    check_precons_for_reinsertion(*landmark, to_add);
                    }
            }
            landmark->is_true_now = false;
            landmark++;
        }
    }



    //delete landmarks that were keep because they were preconditions, but are no longer needed
    for (auto landmark : to_delete) {
        std::vector<LandmarkObj>::iterator it = find(to_add.begin(), to_add.end(), *landmark);
        if(it != to_add.end()){
            continue;
        }
        remove(predicate_landmarks.begin(), predicate_landmarks.end(), *landmark);
    }
    to_delete.clear();

    for(auto &landmark : to_add){
        this->predicate_landmarks.push_back(landmark);
    }

    //update truth values for future use
    for (auto landmark : predicate_landmarks) {
        landmark.was_true_last_step = landmark.is_true_now;
    }

}

std::vector<LandmarkObj> DBState::check_precons_for_reinsertion(LandmarkObj lm, std::vector<LandmarkObj> to_add){
    if(lm.is_action){
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
        std::vector<LandmarkObj>::iterator it = find(predicate_landmarks.begin(), predicate_landmarks.end(), lm);
        if(it != predicate_landmarks.end()){
            it->num_of_greedy_effects++;
            return to_add;
        }
        it = find(to_add.begin(), to_add.end(), lm);
        if(it != to_add.end()){
            it->num_of_greedy_effects++;
            return to_add;
        }
        lm.num_of_greedy_effects++;
        to_add.push_back(lm);
        bool is_present = check_presence_of_fact_lm(lm);
        if(!is_present){
            if (lm.precons_greedy_order.has_value()) {
                std::vector<LandmarkObj *> precons = lm.precons_greedy_order.value();
                for (auto precon_landmark : precons) {
                    check_precons_for_reinsertion(*precon_landmark, to_add);
                }
            }
        }
        return to_add;
    }

}


void DBState::set_initial_landmarks(std::vector<LandmarkObj> input_predicate_landmarks) {
    for (auto &landmark : input_predicate_landmarks) {

        //check reasonable orderings cannot be done inside check_presence_of_factlm since actionLms can be precon
        if (landmark.precons_reasonable_order.has_value()) {
            std::vector precons = landmark.precons_reasonable_order.value();
            for (auto precon_landmark = precons.begin(); precon_landmark < precons.end();) {
                bool precon_is_present;
                LandmarkObj *lm = *precon_landmark;
                if (lm->is_action) {
                    precon_is_present = false;
                } else {
                    precon_is_present = check_presence_of_fact_lm(*lm);
                }
                if (precon_is_present) {
                    precon_landmark = precons.erase(precon_landmark);
                } else {
                    precon_landmark++;
                }
            }
            if (!precons.empty()) {
                predicate_landmarks.push_back(landmark);
                continue;
            }
        }


        bool is_present;
        if (landmark.is_action) {
            is_present = false;
        } else {
            is_present = check_presence_of_fact_lm(landmark);
        }
        //greedy necessary ordering

        if ((is_present && landmark.is_goal) || (is_present && landmark.num_of_greedy_effects != 0)) {
            num_of_fullfilled_goal_lms++;
            landmark.is_true_now = true;
            predicate_landmarks.push_back(landmark);

        } else if (is_present) {
            landmark.is_true_now = true;
            if (landmark.effects_nat_order.has_value()) {
                std::vector effects = landmark.effects_nat_order.value();
                for (auto lm : effects) {
                    lm->num_of_natural_precons--;
                }
                landmark.effects_nat_order->clear();
            }

            if (landmark.is_goal) {
                num_of_fullfilled_goal_lms++;
                predicate_landmarks.push_back(landmark);
            }
        }
        if (!is_present) {
            predicate_landmarks.push_back(landmark);
        }
    }

    for (auto landmark : predicate_landmarks) {
        landmark.was_true_last_step = landmark.is_true_now;
    }
    cout << "Initiale Fact Landmarken gesetzt. Anzahl: " << num_of_predicate_landmarks() << endl;


}

bool DBState::check_presence_of_fact_lm(LandmarkObj factLM) {
    bool andOrFactLm = factLM.other_preds.has_value();
    if (factLM.precons_greedy_order.has_value()) {
        std::vector precons = factLM.precons_greedy_order.value();
        for (auto landmark : precons) {
            if (!landmark->was_true_last_step) {
                return false;
            }
        }
    }
    if (factLM.num_of_natural_precons != 0) {
        return false;
    }

    //check for predicate with no arguments
    if (factLM.arguments.empty()) {
        bool match;
        if (factLM.negated) {
            match = !nullary_atoms[factLM.index];
        } else {
            match = nullary_atoms[factLM.index];
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
    const auto &tuples = relations[factLM.index].tuples;
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
    if (actionLm.num_of_natural_precons != 0) {
        return false;
    }
    if (actionLm.precons_greedy_order.has_value()) {
        std::vector precons = actionLm.precons_greedy_order.value();
        for (auto landmark : precons) {
            if (!landmark->was_true_last_step) {
                return false;
            }
        }
    }

    if (actionLm.arity == action.get_parameters().size()) {
        if (actionLm.name == action.get_name() && actionLm.index == action.get_index()) {
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


bool DBState::fact_lm_equal_to_ground_effect(LandmarkObj factLM, ActionSchema action,
                                             const LiftedOperatorId &grounded_action) {
    //cout << "checking fact equal to ground effect" << endl;
    bool orderedFactLm = factLM.precons_greedy_order.has_value();
    //bool andOrFactLm = factLM.other_preds.has_value();
    if (orderedFactLm) {
        std::vector precons = factLM.precons_greedy_order.value();
        for (auto landmark : precons) {
            if (!landmark->was_true_last_step) {
                return false;
            }
        }
    }

    if (factLM.arguments.empty()) {
        if (factLM.negated) {
            return action.get_negative_nullary_effects().at(factLM.index);
        } else {
            return action.get_positive_nullary_effects().at(factLM.index);
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
                        std::vector effects_nat_order = factLM.effects_nat_order.value();
                        for (auto *landmark : effects_nat_order) {
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


