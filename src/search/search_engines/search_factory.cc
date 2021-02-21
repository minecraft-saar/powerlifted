
#include "search_factory.h"
#include "../states/extensional_states.h"
#include "../states/sparse_states.h"
#include "greedy_best_first_search.h"
#include "search.h"

#include <boost/algorithm/string.hpp>

SearchBase*
SearchFactory::create(const std::string& method, const std::string& state_type) {
    std::cout << "Creating search factory for method " << method << "..." << std::endl;
    bool using_ext_state = boost::iequals(state_type, "extensional");

    if (boost::iequals(method, "gbfs")) {
        if (using_ext_state) return new GreedyBestFirstSearch<ExtensionalPackedState>();
        else return new GreedyBestFirstSearch<SparsePackedState>();
    }
    else {
        std::cerr << "Invalid search method \"" << method << "\"" << std::endl;
        exit(-1);
    }
}
