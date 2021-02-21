#ifndef SEARCH_HEURISTIC_H
#define SEARCH_HEURISTIC_H

#include <utility>

class DBState;
class Task;

class Heuristic {
public:
    virtual ~Heuristic() = default;
    /**
     * @brief Virtual implementation of a heuristic function
     * @param s: State being evaluated
     * @param task: Planning task
     * @return Heuristic value
     */
    virtual int compute_heuristic(const DBState &s, const Task &task) = 0;
};


class THeuristic {
public:
    virtual ~THeuristic() = default;
    /**
     * @brief Virtual implementation of a heuristic function
     * @param s: State being evaluated
     * @param task: Planning task
     * @return Heuristic value
     */
    virtual std::pair<int, int> compute_heuristic(const DBState &s, const Task &task) = 0;
};

template <typename H1, typename H2>
class THeuristicImpl : public THeuristic {
private:
    H1 h1;
    H2 h2;

public:
    virtual ~THeuristicImpl() = default;

    virtual std::pair<int, int> compute_heuristic(const DBState &s, const Task &task) {
        return { h1.compute_heuristic(s, task), h2.compute_heuristic(s, task) };
    };
};

#endif //SEARCH_HEURISTIC_H
