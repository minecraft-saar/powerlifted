#ifndef SEARCH_PARSER_H
#define SEARCH_PARSER_H

#include "structures.h"

#include <fstream>
#include <vector>

class Task;

bool parse(Task &task, const std::ifstream &in);
void output_error(std::string &msg);

bool is_sparse_representation(std::string &canary);
bool is_next_section_correct(std::string &canary, const std::string &expected);
/*
 * Read next n values from stdin and copy to v
 */
void copy_next_n_values(int n, std::vector<int> &v);

void parse_types(Task &task, int number_types);
void parse_predicates(Task &task, int number_predicates);
void parse_objects(Task &task, int number_objects);
void parse_initial_state(Task &task, int initial_state_size);
void parse_goal(Task &task, int goal_size);
void parse_action_schemas(Task &task, int number_action_schemas);

void parse_landmarks(std::string file, Task &task, LMOrdering type_of_ordering);
FactLm create_fact_lm(std::vector<std::string> &arguments, bool and_con, int num_of_preds, bool is_in_ordering, Task &task, LMOrdering type_of_ordering);
FactLm create_action_lm(std::vector<std::string> &arguments, bool and_con, int num_of_preds, bool is_in_ordering, Task &task, LMOrdering type_of_ordering);

#endif  // SEARCH_PARSER_H
