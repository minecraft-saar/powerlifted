
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

void DBState::set_landmarks(DBState parent, ActionSchema action, const LiftedOperatorId &op_id,
                            LMOrdering type_of_lm_ordering) {
    for (auto &landmark : parent.predicate_landmarks)
        this->predicate_landmarks.push_back(landmark);

    std::vector<FactLm *> to_delete;
    for (auto landmark = predicate_landmarks.begin(); landmark < predicate_landmarks.end();) {
        //bool is_present = fact_lm_equal_to_ground_effect(*landmark, action, op_id);
        bool is_present;
        if (landmark->is_action) {
            is_present = action_lm_equal_to_action(*landmark, action, op_id, type_of_lm_ordering);
        } else {
            is_present = check_presence_of_fact_lm(*landmark, type_of_lm_ordering);
        }

        if (is_present) {
            landmark->is_true_now = true;
            if(type_of_lm_ordering == LMOrdering::Greedy){
                // If the landmark has preconditions, we can now remove them from our list
                // To not break the current for-loop we add them to to_delete vector and delete them after the loop
                if (landmark->precons.has_value()) {
                    std::vector<FactLm *> precons = landmark->precons.value();
                    for (auto precon_landmark : precons) {
                        precon_landmark->num_of_effects--;
                        //std::vector<FactLm*> effects_of_precon_lm = precon_landmark->effects.value();
                        //std::remove(effects_of_precon_lm.begin(),effects_of_precon_lm.end(), landmark);
                        if (precon_landmark->num_of_effects == 0) {
                            to_delete.push_back(precon_landmark);
                        }
                    }
                }
                //if the landmrk still has effects we need to keep it since effects can only become true if their premise was true in the last step
                if (landmark->num_of_effects != 0) {
                    num_of_fullfilled_goal_lms++;
                    landmark++;
                    continue;
                }
            } else if(type_of_lm_ordering == LMOrdering::Reasonable){
                if(landmark->effects.has_value()){
                    std::vector<FactLm *> effects = landmark->effects.value();
                    for(auto lm : effects){
                        lm->num_of_precons--;
                    }
                }
            }

            //Goal Landmarks are never deleted
            if (landmark->is_goal) {
                num_of_fullfilled_goal_lms++;
                landmark++;
            } else {
                //this landmark is true but is neither a goal nor has any effects
                landmark = predicate_landmarks.erase(landmark);
            }
        //landmark is not present
        } else {
            landmark->is_true_now = false;
            landmark++;
        }
    }

    //delete landmarks that were keep because they were preconditions, but are no longer needed
    for (auto landmark : to_delete) {
        remove(predicate_landmarks.begin(), predicate_landmarks.end(), *landmark);
    }
    to_delete.clear();
    //update truth values for future use
    for (auto landmark : predicate_landmarks) {
        landmark.was_true_last_step = landmark.is_true_now;
    }

    /*
    for (auto landmark = action_landmarks.begin(); landmark < action_landmarks.end();) {
        bool is_present = action_lm_equal_to_action(*landmark, action, op_id);
        if (is_present) {
            landmark = action_landmarks.erase(landmark);
            continue;
        }
        landmark++;
    }
    */

}

void DBState::set_initial_landmarks(std::vector<FactLm> input_predicate_landmarks,
                                    LMOrdering type_of_lm_ordering) {
    cout << "Initiale Action Landmarken gesetzt. Anzahl: " << num_of_action_landmarks() << endl;

    for (auto &landmark : input_predicate_landmarks) {
        bool is_present;
        if (landmark.is_action) {
            is_present = false;
        } else {
            is_present = check_presence_of_fact_lm(landmark, type_of_lm_ordering);
        }
        //greedy necessary ordering
        if (type_of_lm_ordering == LMOrdering::Greedy || type_of_lm_ordering == LMOrdering::None) {
            if ((is_present && landmark.is_goal) || (is_present && landmark.num_of_effects != 0)) {
                num_of_fullfilled_goal_lms++;
                landmark.is_true_now = true;
                predicate_landmarks.push_back(landmark);
            }
        } else if (type_of_lm_ordering == LMOrdering::Reasonable) {
            if (is_present) {
                landmark.is_true_now = true;
                if (landmark.effects.has_value()) {
                    std::vector effects = landmark.effects.value();
                    for (auto lm : effects) {
                        lm->num_of_precons--;
                    }
                    landmark.effects->clear();
                }
            }
            if (is_present && landmark.is_goal) {
                num_of_fullfilled_goal_lms++;
                predicate_landmarks.push_back(landmark);
            }
        }
        if (!is_present) {
            /*cout << landmark.name << " ";
            for (unsigned int i = 0; i < landmark.arguments.size(); i++)
                cout << landmark.arguments[i].index << " ";
            cout << endl;*/
            predicate_landmarks.push_back(landmark);
        }
    }

    for (auto landmark : predicate_landmarks) {
        landmark.was_true_last_step = landmark.is_true_now;
    }
    /*for (auto &landmark : predicate_landmarks) {
        cout << "Name of Landmark: ";
        cout << landmark.name << endl;
    } */
    cout << "Initiale Fact Landmarken gesetzt. Anzahl: " << num_of_predicate_landmarks() << endl;


}

bool DBState::check_presence_of_fact_lm(FactLm factLM, LMOrdering ordering) {
    bool andOrFactLm = factLM.other_preds.has_value();
    if(ordering == LMOrdering::Greedy){
        if (factLM.precons.has_value()) {
            std::vector precons = factLM.precons.value();
            for (auto landmark : precons) {
                if (!landmark->was_true_last_step) {
                    return false;
                }
            }
        }
    }
    if(ordering == LMOrdering::Reasonable){
        if(factLM.num_of_precons != 0){
            return false;
        }
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
                        if (check_presence_of_fact_lm(landmark, ordering)) {
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
                        match = check_presence_of_fact_lm(lm, ordering);
                        if (!match) {
                            break;
                        }
                    }
                }
            }
            /*if (match && orderedFactLm) {
                std::vector effects = factLM.effects.value();
                for (auto *landmark : effects) {
                    landmark->removePrecon();
                }
                effects.clear();
            }*/
            //cout << factLM.name << endl;
            return match;
        }
    }

    //this landmark is not present but check if it is an OR Landmark
    if (andOrFactLm) {
        if (!factLM.isAnd()) {
            std::vector<FactLm> otherPreds = factLM.other_preds.value();
            for (auto &landmark : otherPreds) {
                if (check_presence_of_fact_lm(landmark, ordering)) {
                    return true;
                }
            }
        }
    }
    return false;

}

bool DBState::action_lm_equal_to_action(FactLm actionLm, ActionSchema action, LiftedOperatorId grounded_action, LMOrdering ordering) {
    if(ordering == LMOrdering::Reasonable){
        if(actionLm.num_of_precons != 0){
            return false;
        }
    }
    if(ordering == LMOrdering::Greedy){
        if (actionLm.precons.has_value()) {
            std::vector precons = actionLm.precons.value();
            for (auto landmark : precons) {
                if (!landmark->was_true_last_step) {
                    return false;
                }
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


bool DBState::fact_lm_equal_to_ground_effect(FactLm factLM, ActionSchema action,
                                             const LiftedOperatorId &grounded_action) {
    //cout << "checking fact equal to ground effect" << endl;
    bool orderedFactLm = factLM.precons.has_value();
    //bool andOrFactLm = factLM.other_preds.has_value();
    if (orderedFactLm) {
        std::vector precons = factLM.precons.value();
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
                        std::vector effects = factLM.effects.value();
                        for (auto *landmark : effects) {
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


