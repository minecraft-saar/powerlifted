#include "search_statistics.h"

#include "utils/logging.h"
#include "utils/timer.h"
#include "utils/system.h"
#include "utils/system_unix.h"

#include <iostream>

using namespace std;

SearchStatistics::SearchStatistics() : SearchStatistics(utils::Verbosity::NORMAL)
{}

SearchStatistics::SearchStatistics(utils::Verbosity verbosity)
        : verbosity(verbosity) {
    expanded_states = 0;
    reopened_states = 0;
    evaluated_states = 0;
    evaluations = 0;
    generated_states = 0;
    dead_end_states = 0;
    generated_ops = 0;

    lastjump_expanded_states = 0;
    lastjump_reopened_states = 0;
    lastjump_evaluated_states = 0;
    lastjump_generated_states = 0;

    lastjump_value = make_pair(-1, -1);
}

void SearchStatistics::report_f_value_progress(std::pair<int, int> f) {
    if (f > lastjump_value) {
        lastjump_value = f;
        print_progress_line('f');
        lastjump_expanded_states = expanded_states;
        lastjump_reopened_states = reopened_states;
        lastjump_evaluated_states = evaluated_states;
        lastjump_generated_states = generated_states;
    }
}

void SearchStatistics::print_progress_line(char metric) const {
    if (verbosity >= utils::Verbosity::NORMAL) {
        cout << metric << " = " << "("  << lastjump_value.first << ", " << lastjump_value.second << ")"
             << " [";
        print_basic_statistics();
        cout << "]" << endl;
    }
}

void SearchStatistics::print_checkpoint_line(int g) const {
    if (verbosity >= utils::Verbosity::NORMAL) {
        cout << "[g=" << g << ", ";
        print_basic_statistics();
        cout << "]" << endl;
    }
}

void SearchStatistics::print_basic_statistics() const {
    cout << evaluated_states << " evaluated, "
         << expanded_states << " expanded, "
         << generated_states << " generated, ";
    if (reopened_states > 0) {
        cout << reopened_states << " reopened, ";
    }
    cout << "t=" << utils::g_timer;
    cout << ", " << utils::get_peak_memory_in_kb() << " KB";
}

void SearchStatistics::print_detailed_statistics() const {
    cout << "Expanded " << expanded_states << " state(s)." << endl;
    cout << "Reopened " << reopened_states << " state(s)." << endl;
    cout << "Evaluated " << evaluated_states << " state(s)." << endl;
    cout << "Evaluations: " << evaluations << endl;
    cout << "Generated " << generated_states << " state(s)." << endl;
    cout << "Dead ends: " << dead_end_states << " state(s)." << endl;

    if (lastjump_value >= make_pair(0, 0)) {
        cout << "Expanded until last jump: "
             << lastjump_expanded_states << " state(s)." << endl;
        cout << "Reopened until last jump: "
             << lastjump_reopened_states << " state(s)." << endl;
        cout << "Evaluated until last jump: "
             << lastjump_evaluated_states << " state(s)." << endl;
        cout << "Generated until last jump: "
             << lastjump_generated_states << " state(s)." << endl;
    }

#if OPERATING_SYSTEM == LINUX
    cout << "Peak memory usage: " << utils::get_peak_memory_in_kb() << " kB\n";
#endif
}
