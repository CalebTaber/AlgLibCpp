#include <iostream>
#include <cmath>

#include "Arithmetic.h"

#define string std::string
#define map std::map

#define cout std::cout
#define endl std::endl

bool canAdd(Term* one, Term* two) {
    return one->varsEqual(two);
}

bool canExponentiate(Term* one, Term* two) {
    if (!two->noVars()) return false;
    return true;
}

bool operable(Term* one, Term* two, const string* op) {
    if (one == nullptr || two == nullptr) return false;

//    cout << one->toString();
//    cout << " " << *op;
//    cout << " " << two->toString() << endl;

    if (*op == "+") return canAdd(one, two);
    else if (*op == "*") return true;
    else if (*op == "/") return (two->getCoefficient() != 0);
    else if (*op == "^") return canExponentiate(one, two);

    return false;
}

Term* operate(Term *one, Term *two, const string *op) {
    if (*op == "+") return addTerms(one, two);
    else if (*op == "*") return multiplyTerms(one, two);
    else if (*op == "/") return divideTerms(one, two);
    else if (*op == "^") return pow(one, two);
    else return new Term(1337, nullptr);
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the two given Terms can be added.
 * @return The sum of the two given Terms
 */
Term* addTerms(Term* one, Term* two) {
    map<char, double> varsCopy = one->copyVariables();
    return new Term((one->getCoefficient() + two->getCoefficient()), &varsCopy);
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the two given Terms can be multiplied.
 * @return The product of the two given Terms
 */
Term* multiplyTerms(Term* one, Term* two) {
    map<char, double> addedVars = addVariables(one, two);
    return new Term((one->getCoefficient() * two->getCoefficient()), &addedVars);
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the two given Terms can be divided.
 * @return The quotient of the two given Terms
 */
Term* divideTerms(Term* one, Term* two) {
    map<char, double> subtractedVars = subtractVariables(one, two);
    return new Term((one->getCoefficient() / two->getCoefficient()), &subtractedVars);
}

map<char, double> operateOnVarsMaps(Term *one, Term *two, bool add) {
    map<char, double> result;

    map<char, double> oneVars = one->copyVariables();
    map<char, double> twoVars = two->copyVariables();

    if (oneVars.empty()) return two->copyVariables();
    if (twoVars.empty()) return one->copyVariables();

    for (auto iter : twoVars) {
        auto termOneExp = oneVars.find(iter.first);

        if (termOneExp == oneVars.end()) { // If the variable is not present in the first term's map
            if (iter.second != 0) {
                if (add) result.emplace(iter.first, iter.second);
                else result.emplace(iter.first, -(iter.second));
            }
        }
        else { // If both terms have the same variable (regardless of exponent)
            if (add) {
                if (termOneExp->second + iter.second != 0) result.emplace(iter.first, termOneExp->second + iter.second);
            }
            else {
                if (termOneExp->second - iter.second != 0) result.emplace(iter.first, termOneExp->second - iter.second);
            }
        }
    }

    return result;
}

map<char, double> addVariables(Term* one, Term* two) {
    return operateOnVarsMaps(one, two, true);
}

map<char, double> subtractVariables(Term* one, Term* two) {
    return operateOnVarsMaps(one, two, false);
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the exponentiation can be evaluated.
 * @return Term one raised to the value of Term two
 */
Term* pow(Term* one, Term* two) {
    double coefficient = pow(one->getCoefficient(), two->getCoefficient());

    map<char, double> oneVars = one->getVariables();
    map<char, double> vars;
    for (auto iter : oneVars) {
        vars.emplace(iter.first, iter.second * two->getCoefficient());
    }

    return new Term(coefficient, &vars);
}
