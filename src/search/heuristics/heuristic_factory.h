#ifndef SEARCH_HEURISTIC_FACTORY_H
#define SEARCH_HEURISTIC_FACTORY_H

#include <string>

class Task;
class THeuristic;


/**
 * @brief Factory class to generate corresponding heuristic object
 */
class HeuristicFactory {
public:
    static THeuristic *create(const std::string& method, const Task &task);
};



#endif //SEARCH_HEURISTIC_FACTORY_H
