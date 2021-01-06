
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
    if (!action.is_ground()) {
        cerr << "Action Not ground" << endl;
    }

    for (auto &landmark : parent.action_landmarks)
        action_landmarks.push_back(landmark);

    for (auto &landmark : parent.predicate_landmarks)
        predicate_landmarks.push_back(landmark);

    for (const Atom &effect : action.get_effects()) {
        predicate_landmarks.erase(std::remove_if(predicate_landmarks.begin(),
                                                 predicate_landmarks.end(),
                                                 [& effect, op_id](auto landmark) {
                                                     return fact_lm_equal_to_ground_effect(landmark, effect, op_id);
                                                 }));
    }

    for (auto landmark : action_landmarks) {
        //if landmark.name equal action.name check action parameters
        action_landmarks.erase(std::remove_if(action_landmarks.begin(),
                                              action_landmarks.end(),
                                              [& action, &op_id](auto landmark) {
                                                  return action_lm_equal_to_action(landmark, action, op_id);
                                              }));
    }

}

void DBState::set_initial_landmarks(std::vector<FactLm> input_predicate_landmarks,
                                    std::vector<ActionLm> input_action_landmarks) {
    for (auto &landmark : input_action_landmarks) {
        action_landmarks.push_back(landmark);
    }

    cout << "predicate landmarks" << endl;
    for (auto &landmark : input_predicate_landmarks) {
        predicate_landmarks.push_back(landmark);
    }
    cout << "Initiale Action Landmarken gesetzt. Anzahl: " << num_of_action_landmarks() << endl;

    /*for (auto &landmark : predicate_landmarks) {
        cout << "Name of Landmark: ";
        cout << landmark.name << endl;
    } */
    cout << "Initiale Fact Landmarken gesetzt. Anzahl: " << num_of_predicate_landmarks() << endl;



}

bool fact_lm_equal_to_ground_effect(FactLm factLm, Atom effect, LiftedOperatorId grounded_action) {
    if (effect.name == factLm.name) {
        if (effect.negated == factLm.negated) {
            if (effect.arguments.size() == factLm.arguments.size()) {
                for (Argument arg : effect.arguments) {
                    int objIndex = arg.index;
                    if (!arg.constant) {
                        objIndex = grounded_action.get_instantiation()[arg.index];
                    }
                    auto it = find_if(factLm.arguments.begin(), factLm.arguments.end(),
                                      [objIndex](const Argument &fact) {
                                          return (objIndex == fact.index);
                                      });
                    if (it == factLm.arguments.end()) {
                        return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

bool action_lm_equal_to_action(ActionLm actionLm, ActionSchema action, LiftedOperatorId grounded_action) {
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
