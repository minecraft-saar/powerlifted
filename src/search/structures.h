#ifndef SEARCH_STRUCTURES_H
#define SEARCH_STRUCTURES_H

#include "hash_structures.h"

#include <string>
#include <utility>
#include <unordered_set>
#include <vector>
#include <iostream>

/**
 * @brief GroundAtom is an alias for vector of integers. It is represented
 * as a list of object indices.
 */
typedef std::vector<int> GroundAtom;


/**
 * @brief Represent a parameter for a given action schema.
 *
 * @var name: Name of the parameter in the action schema definition.
 * @var index: Index of this parameter in the list of parameters in the schema.
 * @var type: Type related to this parameter.
 */
struct Parameter {
    Parameter(std::string name, int index, int type)
            : name(std::move(name)), index(index), type(type) {}

    std::string name;
    int index;
    int type;
};


/**
 * @brief Implements an argument composing an atom. It can be a free
 * variable or a constant.
 *
 * @var index: If the argument is a constant, then it represents the index of the object,
 * otherwise it represents the index of the free variable in the parameters of the action
 * schema.
 * @var constant: Indicates whether the argument is a constanst or not (free variable, then).
 *
 */
struct Argument {
    Argument(int index, bool constant) : index(index), constant(constant) {}
    int index;
    bool constant;
};

/**
 * @brief A relation is a "table" with set of tuples corresponding to some
 * predicate in a state.
 *
 * @var predicate_symbol: Indicates its corresponding predicate.
 * @var tuples: Set of tuples (vectors) corresponding to the ground atoms in this relation.
 *
 */
struct Relation {
    Relation() = default;
    Relation(int predicate_symbol,
        std::unordered_set<GroundAtom, TupleHash> &&tuples)
            : predicate_symbol(predicate_symbol),
              tuples (std::move(tuples)) {}

    Relation(const Relation &) = default;


    bool operator==(const Relation &other) const {
        return predicate_symbol == other.predicate_symbol && tuples == other.tuples;
    }

    int predicate_symbol{};
    std::unordered_set<GroundAtom, TupleHash> tuples;
};


/**
 * @brief Represent a lifted atom by its name, predicate symbol index,
 * list of arguments, and whether it is negated or not.
 *
 * @var name: String representing atom name
 * @var predicate_symbol: predicate symbol index
 * @var arguments: list of Argument objects representing the free variables
 * or constants of the atom
 * @var negated: boolean variable indicating whether the atom is negated
 * or not (in whatever context it occurs)
 *
 * @see Argument (structures.h)
 */
struct Atom {
    Atom(std::string &&name, int predicate_symbol,
        std::vector<Argument> &&tuples, bool negated) :
            name(std::move(name)),
            predicate_symbol(predicate_symbol),
            arguments(std::move(tuples)),
            negated(negated) {}

    std::string name;
    int predicate_symbol;
    std::vector<Argument> arguments;
    bool negated;
};

struct ActionLm {
    ActionLm(std::string &name, int arity, std::vector<Argument> &tuples) : name(std::move(name)), arity(arity), arguments(std::move(tuples)){}
    std::string name;
    unsigned int arity;
    std::vector<Argument> arguments;

};

class FactLm {
public:
    std::string name;
    unsigned int arity;
    bool negated;
    int index;
    std::vector<Argument> arguments;
    bool andLM;
    bool is_Goal;
    int num_of_effects;
    bool is_true_now = false;
    bool was_true_last_step = false;
    // unfullfilledPrecons is only relevant if there are Landmark Orderings
    std::optional <std::vector<FactLm*>> precons = std::nullopt;
    //std::optional <std::vector<FactLm*>> effects;
    std::optional <std::vector<FactLm>> otherPreds = std::nullopt;

    FactLm(std::string &name, int arity, bool negated, int index, std::vector<Argument> &tuples, bool andlm, bool is_goal) :
    name(std::move(name)), arity(arity), negated(negated), index(index), arguments(std::move(tuples)), andLM(andlm), is_Goal(is_goal), num_of_effects(0){}

    FactLm(const FactLm &to_copy){
        name = to_copy.name;
        arity = to_copy.arity;
        index = to_copy.index;
        arguments = to_copy.arguments;
        andLM = to_copy.andLM;
        is_Goal = to_copy.is_Goal;
        num_of_effects = to_copy.num_of_effects;
        is_true_now = to_copy.is_true_now;
        was_true_last_step = to_copy.was_true_last_step;
        precons = to_copy.precons;
        otherPreds = to_copy.otherPreds;
    }
/*
    void createEffectVec() {
        if (!effects.has_value()){
            effects = std::vector<FactLm *>();
        } else {
            std::cout << "Effect Vector of Landmark already exists" << std::endl ;
            exit(1);
        }
    }

    void addEffect(FactLm *lm){
        if (effects.has_value()){
            std::vector<FactLm*> vec = effects.value();
            vec.push_back(lm);
        } else {
            std::cout << "Effect Vector of Landmark does not exist" << std::endl ;
            exit(1);
        }
    }*/

    void createPreconsVec() {
        if (!precons.has_value()){
            precons = std::vector<FactLm *>();
        } else {
            std::cout << "Precons Vector of Landmark already exists" << std::endl ;
            exit(1);
        }
    }

    void addPrecon(FactLm *lm){
        if (precons.has_value()){
            std::vector<FactLm*> vec = precons.value();
            vec.push_back(lm);
        } else {
            std::cout << "Effect Vector of Landmark does not exist" << std::endl ;
            exit(1);
        }
    }

    void createOtherPreds() {
        if (!otherPreds.has_value()){
            otherPreds = std::vector<FactLm>();
        } else {
            std::cout << "OtherPreds Vector of Landmark already exists" << std::endl ;
            exit(1);
        }
    }

    bool isAnd(){
        return andLM;
    }

    void addOtherPred(FactLm pred){
        if(otherPreds.has_value()){
            std::vector<FactLm> preds = otherPreds.value();
            preds.push_back(pred);
        } else {
            std::cout << "OtherPreds Vector of Landmark does not exist" << std::endl ;
            exit(1);
        }
    }

    bool operator==(const FactLm& rhs){
        if(this->index == rhs.index){
            if(this->negated != rhs.negated){
                return false;
            }
            if(this->arity !=rhs.arity){
                return false;
            }
            for (unsigned int i= 0; i< this->arguments.size(); i++){
                if(this->arguments.at(i).index != rhs.arguments.at(i).index){
                    return false;
                } else {
                    if(this->arguments.at(i).constant != rhs.arguments.at(i).constant){
                        return false;
                    }
                }
            }
            return true;
        }
        return false;
    }

    virtual ~FactLm()=default;
};

#endif //SEARCH_STRUCTURES_H
