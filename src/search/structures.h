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

class LandmarkObj {
public:
    std::string name;
    unsigned int arity;
    bool negated;
    int pred_index;
    int lm_index;
    std::vector<Argument> arguments;
    bool and_lm;
    bool is_goal;
    bool is_action;
    int num_of_greedy_effects = 0;
    int num_of_natural_precons = 0;
    bool is_true_now = false;
    bool was_true_last_step = false;
    bool needs_check = true;
    // unfullfilledPrecons is only relevant if there are Landmark Orderings
    std::optional <std::vector<int>> precons_greedy_order = std::nullopt;
    std::optional <std::vector<int>> precons_reasonable_order = std::nullopt;
    std::optional <std::vector<int>> effects_nat_order = std::nullopt;
    std::optional <std::vector<LandmarkObj>> other_preds = std::nullopt;

    LandmarkObj(int lmIndex, std::string &name, int arity, bool negated, int index, std::vector<Argument> &tuples, bool andlm, bool isGoal, bool isAction) :
            name(std::move(name)), arity(arity), negated(negated),  pred_index(index), lm_index(lmIndex), arguments(std::move(tuples)), and_lm(andlm), is_goal(isGoal), is_action(isAction) , num_of_greedy_effects(0){}

    LandmarkObj(const LandmarkObj &to_copy){
        name = to_copy.name;
        arity = to_copy.arity;
        pred_index = to_copy.pred_index;
        lm_index = to_copy.lm_index;
        arguments = to_copy.arguments;
        and_lm = to_copy.and_lm;
        is_goal = to_copy.is_goal;
        is_action = to_copy.is_action;
        num_of_greedy_effects = to_copy.num_of_greedy_effects;
        num_of_natural_precons = to_copy.num_of_natural_precons;
        is_true_now = to_copy.is_true_now;
        was_true_last_step = to_copy.was_true_last_step;
        needs_check = to_copy.needs_check;
        precons_greedy_order = to_copy.precons_greedy_order;
        precons_reasonable_order = to_copy.precons_reasonable_order;
        effects_nat_order = to_copy.effects_nat_order;
        other_preds = to_copy.other_preds;
    }

    void createEffectVec() {
        if (!effects_nat_order.has_value()){
            effects_nat_order = std::vector<int>();
        } else {
            std::cout << "Effect Vector of Landmark already exists" << std::endl ;
            exit(1);
        }
    }

    void addEffect(int lm){
        if (effects_nat_order.has_value()){
            effects_nat_order->push_back(lm);
        } else {
            std::cout << "Effect Vector of Landmark does not exist" << std::endl ;
            exit(1);
        }
    }

    void createGreedyPreconsVec() {
        if (!precons_greedy_order.has_value()){
            precons_greedy_order = std::vector<int>();
        } else {
            std::cout << " Greedy Precons Vector of Landmark already exists" << std::endl ;
            exit(1);
        }
    }

    void addGreedyPrecon(int lm){
        if (precons_greedy_order.has_value()){
            precons_greedy_order->push_back(lm);
        } else {
            std::cout << "Greedy Precon Vector of Landmark does not exist" << std::endl ;
            exit(1);
        }
    }

    void createReasonablePreconsVec() {
        if (!precons_reasonable_order.has_value()){
            precons_reasonable_order = std::vector<int>();
        } else {
            std::cout << "Reasonable Precons Vector of Landmark already exists" << std::endl ;
            exit(1);
        }
    }

    void addReasonablePrecon(int lm){
        if (precons_reasonable_order.has_value()){
            precons_reasonable_order->push_back(lm);
        } else {
            std::cout << "Reasonable Precon Vector of Landmark does not exist" << std::endl ;
            exit(1);
        }
    }

    void createOtherPreds() {
        if (!other_preds.has_value()){
            other_preds = std::vector<LandmarkObj>();
        } else {
            std::cout << "OtherPreds Vector of Landmark already exists" << std::endl ;
            exit(1);
        }
    }

    bool isAnd(){
        return and_lm;
    }

    void addOtherPred(LandmarkObj pred){
        if(other_preds.has_value()){
            other_preds->push_back(pred);
        } else {
            std::cout << "OtherPreds Vector of Landmark does not exist" << std::endl ;
            exit(1);
        }
    }

    bool operator==(const LandmarkObj& rhs){
        if(this->pred_index == rhs.pred_index){
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

    virtual ~LandmarkObj()=default;
};

enum class LMOrdering{Natural, Greedy, Reasonable, None};

#endif //SEARCH_STRUCTURES_H
