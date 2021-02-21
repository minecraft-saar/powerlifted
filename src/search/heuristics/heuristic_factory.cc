
#include "heuristic_factory.h"
#include "blind_heuristic.h"
#include "goalcount.h"
#include "landmark_heuristic.h"

#include <iostream>

#include <boost/algorithm/string.hpp>

THeuristic *HeuristicFactory::create(const std::string &method, const Task &task)
{
    std::cout << "Creating search factory..." << std::endl;
    if (boost::iequals(method, "goalcount")) {
        return new THeuristicImpl<Goalcount, landmark_heuristic>;
    }
    else if (boost::iequals(method, "landmark")) {
        return new THeuristicImpl<landmark_heuristic, Goalcount>;
    }
    else {
        std::cerr << "Invalid heuristic \"" << method << "\"" << std::endl;
        exit(-1);
    }
}