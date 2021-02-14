#include "parser.h"
#include "action_schema.h"
#include "goal_condition.h"
#include "task.h"
#include "structures.h"
#include "predicate.h"

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

/**
 * For the format of the intermediate file produced by the PDDL translation,
 * check the comments of the translation source code.
 *
 * @param task
 * @param in
 * @return
 */

bool parse(Task &task, const ifstream &in) {

    // String used to guarantee consistency throughout the parsing
    string canary;

    if (not is_sparse_representation(canary)) {
        return false;
    }

    int number_types;
    cin >> canary >> number_types;
    if (not is_next_section_correct(canary, "TYPES")) {
        return false;
    }
    cout << "Total number of types: " << number_types << endl;
    parse_types(task, number_types);

    int number_predicates;
    cin >> canary >> number_predicates;
    if (not is_next_section_correct(canary, "PREDICATES")) {
        return false;
    }
    cout << "Total number of predicates: " << number_predicates << endl;
    parse_predicates(task, number_predicates);


    int number_objects;
    cin >> canary >> number_objects;
    if (not is_next_section_correct(canary, "OBJECTS")) {
        return false;
    }
    cout << "Total number of objects: " << number_objects << endl;
    parse_objects(task, number_objects);


    int initial_state_size;
    cin >> canary >> initial_state_size;
    if (not is_next_section_correct(canary, "INITIAL-STATE")) {
        return false;
    }
    cout << "Total number of atoms in the initial state: " << initial_state_size << endl;
    task.create_empty_initial_state(task.predicates.size());
    parse_initial_state(task, initial_state_size);


    int goal_size;
    cin >> canary >> goal_size;
    if (not is_next_section_correct(canary, "GOAL")) {
        return false;
    }
    cout << "Total number of fluent atoms in the goal state: " << goal_size << endl;
    parse_goal(task, goal_size);


    int number_action_schemas;
    cin >> canary >> number_action_schemas;
    if (not is_next_section_correct(canary, "ACTION-SCHEMAS")) {
        return false;
    }
    cout << "Total number of action schemas: " << number_action_schemas << endl;
    parse_action_schemas(task, number_action_schemas);

    return true;
}

void parse_action_schemas(Task &task, int number_action_schemas) {
    vector<ActionSchema> actions;
    for (int i = 0; i < number_action_schemas; ++i) {
        string name;
        int cost, args, precond_size, eff_size;
        cin >> name >> cost >> args >> precond_size >> eff_size;
        vector<Parameter> parameters;
        vector<Atom> preconditions, effects;
        vector<pair<int, int>> inequalities;
        vector<bool> positive_nul_precond(task.predicates.size(), false),
                negative_nul_precond(task.predicates.size(), false),
                positive_nul_eff(task.predicates.size(), false),
                negative_nul_eff(task.predicates.size(), false);
        for (int j = 0; j < args; ++j) {
            string param_name;
            int index, type;
            cin >> param_name >> index >> type;
            parameters.emplace_back(param_name, index, type);
        }
        for (int j = 0; j < precond_size; ++j) {
            string precond_name;
            int index;
            bool negated;
            int arguments_size;
            cin >> precond_name >> index >> negated >> arguments_size;
            if (arguments_size == 0) {
                assert(task.nullary_predicates.find(index) != task.nullary_predicates.end());
                if (!negated)
                    positive_nul_precond[index] = true;
                else
                    negative_nul_precond[index] = true;
                continue;
            }
            bool is_inequality = (boost::iequals(precond_name, "=") and negated);
            vector<Argument> arguments;
            if (is_inequality) {
                assert(arguments_size == 2);
                int obj1, obj2;
                char c, d;
                cin >> c >> obj1 >> d >> obj2;
                arguments.emplace_back(obj1, c == 'c');
                arguments.emplace_back(obj2, d == 'c');
                if (find(inequalities.begin(), inequalities.end(), make_pair(obj2, obj1)) !=
                    inequalities.end()) {
                    continue;
                }
                inequalities.emplace_back(obj1, obj2);
            } else {
                for (int k = 0; k < arguments_size; ++k) {
                    char c;
                    int obj_index;
                    cin >> c >> obj_index;
                    if (c == 'c') {
                        arguments.emplace_back(obj_index, true);
                    } else if (c == 'p') {
                        arguments.emplace_back(obj_index, false);
                    } else {
                        cerr << "Error while reading action schema " << name
                             << ". Argument is neither constant or "
                                "object"
                             << endl;
                        exit(-1);
                    }
                }
            }
            preconditions.emplace_back(move(precond_name), index, move(arguments), negated);
        }
        for (int j = 0; j < eff_size; ++j) {
            string eff_name;
            int index;
            bool negated;
            int arguments_size;
            cin >> eff_name >> index >> negated >> arguments_size;
            vector<Argument> arguments;
            if (arguments_size == 0) {
                assert(task.nullary_predicates.find(index) != task.nullary_predicates.end());
                if (!negated)
                    positive_nul_eff[index] = true;
                else
                    negative_nul_eff[index] = true;
                continue;
            }
            for (int k = 0; k < arguments_size; ++k) {
                char c;
                int obj_index;
                cin >> c >> obj_index;
                if (c == 'c') {
                    arguments.emplace_back(obj_index, true);
                } else if (c == 'p') {
                    arguments.emplace_back(obj_index, false);
                } else {
                    cerr << "Error while reading action schema " << name
                         << ". Argument is neither constant or "
                            "object"
                         << endl;
                    exit(-1);
                }
            }
            effects.emplace_back(move(eff_name), index, move(arguments), negated);
        }
        ActionSchema a(name,
                       i,
                       cost,
                       parameters,
                       preconditions,
                       effects,
                       inequalities,
                       positive_nul_precond,
                       negative_nul_precond,
                       positive_nul_eff,
                       negative_nul_eff);
        actions.push_back(a);
    }
    task.initialize_action_schemas(actions);
}

void parse_goal(Task &task, int goal_size) {
    vector<AtomicGoal> goals;
    unordered_set<int> positive_nullary_goals, negative_nullary_goals;
    for (int i = 0; i < goal_size; ++i) {
        string name;
        int predicate_index;
        bool negated;
        int number_args;
        cin >> name >> predicate_index >> negated >> number_args;
        if (number_args == 0) {
            if (negated)
                negative_nullary_goals.insert(predicate_index);
            else
                positive_nullary_goals.insert(predicate_index);
            continue;
        }
        vector<int> args;
        copy_next_n_values(number_args, args);
        goals.emplace_back(predicate_index, args, negated);
    }
    task.create_goal_condition(goals, positive_nullary_goals, negative_nullary_goals);
}

void parse_initial_state(Task &task, int initial_state_size) {
    for (int i = 0; i < initial_state_size; ++i) {
        string name;
        int index;
        int predicate_index;
        bool negated;
        int number_args;
        cin >> name >> index >> predicate_index >> negated >> number_args;
        if (number_args == 0) {
            assert(task.nullary_predicates.find(predicate_index) != task.nullary_predicates.end());
            task.initial_state.set_nullary_atom(predicate_index, true);
        }
        vector<int> args;
        copy_next_n_values(number_args, args);
        if (!task.initial_state.get_nullary_atoms()[predicate_index]) {
            if (!task.predicates[predicate_index].isStaticPredicate())
                task.initial_state.add_tuple(predicate_index, args);
            else
                task.static_info.add_tuple(predicate_index, args);
        }
    }
}

void parse_objects(Task &task, int number_objects) {
    for (int i = 0; i < number_objects; ++i) {
        string name;
        int index;
        int n;
        cin >> name >> index >> n;
        vector<int> types;
        copy_next_n_values(n, types);
        task.add_object(name, index, types);
    }
}

void parse_predicates(Task &task, int number_predicates) {
    for (int j = 0; j < number_predicates; ++j) {
        string predicate_name;
        int index;
        int number_args;
        bool static_pred;
        cin >> predicate_name >> index >> number_args >> static_pred;
        if (number_args == 0) {
            task.nullary_predicates.insert(index);
        }
        vector<int> types;
        copy_next_n_values(number_args, types);
        task.add_predicate(predicate_name, index, number_args, static_pred, types);
    }
}

void parse_types(Task &task, int number_types) {
    for (int i = 0; i < number_types; ++i) {
        string type_name;
        int type_index;
        cin >> type_name >> type_index;
        task.add_type(type_name);
    }
}

bool is_sparse_representation(string &canary) {
    cin >> canary;
    if (canary != "SPARSE-REPRESENTATION") {
        cerr << "Representation is not sparse. Not supported." << endl;
        return false;
    }
    return true;
}

bool is_next_section_correct(string &canary, const string &expected) {
    if (canary != expected) {
        cerr << "Error while reading " << expected << " section." << endl;
        output_error(canary);
        return false;
    }
    return true;
}

void copy_next_n_values(int n, vector<int> &v) {
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        v.push_back(x);
    }
}

void output_error(string &msg) {
    cerr << "String read was \'" << msg << "\' instead of the respective canary." << endl;
}


void parse_landmarks(std::string input_file, Task &task) {
    ifstream file(input_file);
    if (!file.is_open()) {
        cerr << "Landmarkfile could not be opened" << endl;
        abort();
    }
    //cout << "Start parsing Landmarks" << endl;
    std::string line;
    int num_lms = 0;
    int lms_read = 0;
    //find beginning of Landmark List
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        } else if (line.at(0) == ';') {
            continue;
        } else if (line.at(0) == 'L') {
            std::getline(file, line);
            std::string::size_type sz; // alias for size_t so the string conversion works
            num_lms = std::stoi(line, &sz);
            break;
        }
    }
    //Start of Landmark List
    while (std::getline(file, line)) {
        if (line.at(0) == ';' || line.empty()) {
            continue; //end of file reached
        }
        stringstream line_as_stream(line);
        int num_of_preds;
        bool actionlm, and_con, is_in_Ordering;
        std::string info;
        //parsing bool flags at beginning
        line_as_stream >> actionlm >> std::ws >> and_con >> std::ws >> num_of_preds >> std::ws;
        //getting at info about predicate/action in brackets
        info = line_as_stream.str().substr(line_as_stream.tellg());
        if (info.at(0) == '0') {
            is_in_Ordering = false;
            info.erase(0, 1);//remove bit for ordering
            info.erase(0, 1);//remove whitespace after
        } else if (info.at(0) == '1') {
            is_in_Ordering = true;
            info.erase(0, 1);//remove bit for ordering
            info.erase(0, 1);//remove whitespace after
        } else {
            is_in_Ordering = false;
        }

        std::vector<FactLm> factsInLM;
        //info.erase(0, 1); //remove opening bracket
        //info.erase( info.end()-1); //remove closing bracket
        std::vector<std::string> arguments;
        string atom = "";
        //cout << "Inhalt einer Klammer " << info << endl;
        for (auto x : info) { //parsing predicate/action into single atom strings
            if (x == '(') {
                continue;
            } else if (x == ')') {
                arguments.push_back(atom);
                if (actionlm) {
                    create_action_lm(arguments, and_con, num_of_preds, task);
                } else {
                    FactLm f = create_fact_lm(arguments, and_con, num_of_preds, is_in_Ordering, task);
                    factsInLM.push_back(f);
                }
                arguments.clear();
            } else if (x == ' ') {
                arguments.push_back(atom);
                //cout << "predicate names after parsing " << atom << endl;
                atom = "";
            } else {
                atom = atom + x;
            }
        }

        if (actionlm) {
            create_action_lm(arguments, and_con, num_of_preds, task);
        } else {
            if (num_of_preds > 1) { //means more than one predicate in LM
                if (factsInLM.at(0).otherPreds.has_value()) {
                    cout << "adding other Predicates to and landmark" << endl;
                    for (unsigned int i = 1; i < factsInLM.size(); i++) {
                        factsInLM.at(0).addOtherPred(factsInLM.at(i));
                        //factsInLM.at(0).effect = &factsInLM.at(2);
                    }
                }

            }
            task.fact_landmarks.push_back(factsInLM.at(0));
        }

        lms_read++;
        if (lms_read >= num_lms) {
            break;
        }
    } //end of Landmark List
    // Find beginning of Landmark ordering list
    //cout << "finished parsing Landmarks" << endl;
    int num_of_orderings = 0;
    int orderings_read = 0;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        } else if (line.at(0) == ';') {
            continue;
        } else if (line.at(0) == 'L') {
            std::getline(file, line);
            std::string::size_type sz; // alias for size_t so the string conversion works
            num_of_orderings = std::stoi(line, &sz);
            if (num_of_orderings == 0) {
                return;
            }
            break;
        }
    }
    // Parsing LM ordering
    //cout << "Trying to parse Ordering" << endl;
    while (std::getline(file, line)) {
        if (line.at(0) == ';' || line.empty()) {
            continue; //end of file reached
        }
        stringstream line_as_stream(line);
        int indexFirstLm, indexSecondLM;
        line_as_stream >> indexFirstLm >> std::ws >> indexSecondLM;
        FactLm firstLMFact = task.fact_landmarks.at(indexFirstLm);
        FactLm secondLMFact = task.fact_landmarks.at(indexSecondLM);

        //firstLMFact.addEffect(&secondLMFact);
        firstLMFact.num_of_effects++;
        secondLMFact.addPrecon(&firstLMFact);
        orderings_read++;
        if (num_of_orderings >= orderings_read) {
            break;
        }
    }


}


FactLm
create_fact_lm(std::vector<std::string> &arguments, bool and_con, int num_of_preds, bool is_in_Ordering, Task &task) {
    bool negated = false;
    if (arguments[0][0] == '!') {
        negated = true;
        arguments[0].erase(0, 1);
    }
    int arity = arguments.size() - 1;
    std::vector<Argument> formated_args;
    for (unsigned int i = 1; i < arguments.size(); i++) {
        string name = arguments[i];
        if (name == "???") {
            Argument arg(i - 1, false);
            formated_args.push_back(arg);
            continue;
        }
        auto it = find_if(task.objects.begin(), task.objects.end(), [&name](const Object &obj) {
            return name == obj.getName();
        });
        assert(it != task.objects.end());
        int index = it->getIndex();
        Argument arg(index, true);
        formated_args.push_back(arg);
    }
    string pred_name = arguments[0];
    auto it = find_if(task.predicates.begin(), task.predicates.end(), [&pred_name](const Predicate &pred) {
        return pred_name == pred.getName();
    });
    int pred_id = it->getIndex();

    GoalCondition goal = task.goal;

    bool is_goal = false;
    if (arity == 0) {
        if (negated) {
            std::unordered_set<int>::const_iterator got = goal.negative_nullary_goals.find(pred_id);
            if (got == goal.negative_nullary_goals.end())
                is_goal = false;
            else
                is_goal = true;

        } else {
            std::unordered_set<int>::const_iterator got = goal.positive_nullary_goals.find(pred_id);
            if (got == goal.positive_nullary_goals.end())
                is_goal = false;
            else
                is_goal = true;
        }
    } else {
        for (const AtomicGoal &atomicGoal : goal.goal) {
            if(atomicGoal.predicate == pred_id){
                bool allmatch = true;
                for(unsigned int i = 0; i< atomicGoal.args.size(); i++){
                    if(!formated_args.at(i).constant){
                        allmatch = false;
                        break;
                    }
                    if(formated_args.at(i).index != atomicGoal.args.at(i)){
                        allmatch = false;
                        break;
                    }
                }
                is_goal = allmatch;
                if(is_goal){
                    break;
                }
            }
        }
    }

    FactLm f(pred_name, arity, negated, pred_id, formated_args, and_con, is_goal);
    if (num_of_preds > 1) {
        f.createOtherPreds();
    }
    if (is_in_Ordering) {
        //f.createEffectVec();
        f.createPreconsVec();
    }
    return f;

    //cout << "setting Landmark Name: " << f.name << endl;
    //task.fact_landmarks.push_back(f);
    //cout << task.fact_landmarks[task.fact_landmarks.size()-1].name << endl;
}

void create_action_lm(std::vector<string> &arguments, bool and_con, int num_of_preds, Task &task) {
    assert(num_of_preds == 1);
    int arity = arguments.size() - 1;
    std::vector<Argument> formated_args;
    for (unsigned int i = 1; i < arguments.size(); i++) {
        string name = arguments[i];
        auto it = find_if(task.objects.begin(), task.objects.end(), [&name](const Object &obj) {
            return name == obj.getName();
        });
        assert(it != task.objects.end());
        int index = it->getIndex();
        Argument arg(index, true);
        formated_args.push_back(arg);
    }
    ActionLm actionlm(arguments[0], arity, formated_args);
    task.action_landmarks.push_back(actionlm);
}