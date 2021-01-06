#ifndef SEARCH_PREDICATE_H
#define SEARCH_PREDICATE_H

#include <string>
#include <utility>
#include <vector>

/**
 * @brief Instantiates a lifted predicate.
 */
class Predicate {
public:
  Predicate(std::string &&name, int index, int arity, bool static_predicate,
            std::vector<int> &&types)
      : name(std::move(name)), index(index), arity(arity),
        static_predicate(static_predicate), types(std::move(types)) {
    // Constructor
  }

  void addArgument(int i) { types.push_back(i); }

  const std::string &getName() const;

  int getArity() const;

  int getIndex() const {return  index;}

  const std::vector<int> &getTypes() const;

  bool isStaticPredicate() const;

private:
  std::string name;
  int index;
  int arity;
  bool static_predicate;
  std::vector<int> types;
};

#endif // SEARCH_PREDICATE_H
