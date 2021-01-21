
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
    //cout << "setting landmarks" << endl;
    for (auto &landmark : parent.action_landmarks)
        this->action_landmarks.push_back(landmark);
    //cout << "set action landmarks" << endl;
    for (auto &landmark : parent.predicate_landmarks)
        this->predicate_landmarks.push_back(landmark);

    //int previous_landmark_count = predicate_landmarks.size();
    //for (const Atom &effect : action.get_effects()) {

    for (auto landmark = predicate_landmarks.begin(); landmark < predicate_landmarks.end();) {
        bool is_present = check_presence_of_fact_lm(*landmark);
        //bool is_present = fact_lm_equal_to_ground_effect(*landmark, effect, op_id);
        if (is_present) {
            landmark = predicate_landmarks.erase(landmark);
            continue;
        }
        landmark++;
    }
    //}
    /*if(previous_landmark_count != 0){
        cout << "Parent Landmark count: " << previous_landmark_count << " child landmark count: "<< predicate_landmarks.size() << endl;
    }*/

    for (auto landmark = action_landmarks.begin(); landmark < action_landmarks.end();) {
        bool is_present = action_lm_equal_to_action(*landmark, action, op_id);
        if (is_present) {
            landmark = action_landmarks.erase(landmark);
            continue;
        }
        landmark++;
    }
    /*
    for (auto landmark : action_landmarks) {
        //if landmark.name equal action.name check action parameters

        action_landmarks.erase(std::remove_if(action_landmarks.begin(),
                                              action_landmarks.end(),
                                              [& action, &op_id](auto landmark) {
                                                  return action_lm_equal_to_action(landmark, action, op_id);
                                              }));
    }*/
    //cout << "finished setting landmarks" << endl;
}

void DBState::set_initial_landmarks(std::vector<FactLm> input_predicate_landmarks,
                                    std::vector<ActionLm> input_action_landmarks) {
    for (auto &landmark : input_action_landmarks) {
        action_landmarks.push_back(landmark);
    }
    cout << "Initiale Action Landmarken gesetzt. Anzahl: " << num_of_action_landmarks() << endl;

    for (auto &landmark : input_predicate_landmarks) {
        bool is_present = check_presence_of_fact_lm(landmark);
        if (!is_present) {
            /*cout << landmark.name << " ";
            for (unsigned int i = 0; i < landmark.arguments.size(); i++)
                cout << landmark.arguments[i].index << " ";
            cout << endl;*/
            predicate_landmarks.push_back(landmark);
        }
    }
    /*for (auto &landmark : predicate_landmarks) {
        cout << "Name of Landmark: ";
        cout << landmark.name << endl;
    } */
    cout << "Initiale Fact Landmarken gesetzt. Anzahl: " << num_of_predicate_landmarks() << endl;


}

bool DBState::check_presence_of_fact_lm(FactLm factlm) {
    if (factlm.arguments.size() == 0) {
        if (!factlm.negated) {
            return nullary_atoms[factlm.index];
        } else {
            return !nullary_atoms[factlm.index];
        }
    } else {
        const auto &tuples = relations[factlm.index].tuples;
        for (auto &possibleInstant : tuples) {
            bool match = true;
            for (unsigned int i = 0; i < possibleInstant.size(); i++) {
                if (!factlm.arguments[i].constant) {
                    continue;
                }
                if (factlm.arguments[i].index != possibleInstant[i]) {
                    match = false;
                }
            }
            if (match) {
                //cout << factlm.name << endl;
                return true;
            }
        }
        return false;
    }
}


bool DBState::fact_lm_equal_to_ground_effect(FactLm factLm, Atom effect, LiftedOperatorId grounded_action) {
    //cout << "checking fact equal to ground effect" << endl;
    if (effect.name == factLm.name) {
        if (effect.negated == factLm.negated) {
            if (effect.arguments.size() == factLm.arguments.size()) {
                bool match = true;
                for (unsigned int i = 0; i < effect.arguments.size(); i++) {
                    if (!factLm.arguments[i].constant) {
                        continue;
                    }
                    Argument arg = effect.arguments[i];
                    int objIndex = arg.index;
                    if (!arg.constant) {
                        objIndex = grounded_action.get_instantiation()[arg.index];
                    }
                    if (objIndex != factLm.arguments[i].index) {
                        match = false;
                    }
                }
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
    return false;
}

bool DBState::action_lm_equal_to_action(ActionLm actionLm, ActionSchema action, LiftedOperatorId grounded_action) {
    if (actionLm.arity == action.get_parameters().size()) {
        if (actionLm.name == action.get_name()) {
            for (unsigned int i = 0; i < actionLm.arity; i++) {
                if (actionLm.arguments[i].index != grounded_action.get_instantiation()[i]) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}
