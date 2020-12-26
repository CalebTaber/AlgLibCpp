#include <iostream>

#include "Arithmetic.h"

#define string std::string
#define map std::map

#define cout std::cout
#define endl std::endl

bool canAdd(Term* one, Term* two) {
    return one->varsEqual(two);
}

bool operable(Term* one, Term* two, const string* op) {
    if (one == nullptr || two == nullptr) return false;

    cout << one->toString();
    cout << " " << *op;
    cout << " " << two->toString() << endl;

    if (*op == "+" || *op == "-") return canAdd(one, two);
    else if (*op == "*") return true;
    else if (*op == "/") return (two->getCoefficient() != 0);

    return false;
}

map<char, double> operateOnVarsMaps(Term *one, Term *two, bool add) {
    map<char, double> result = one->copyVariables();

    map<char, double> oneVars = one->getVariables();
    map<char, double> twoVars = two->getVariables();
    for (auto iter : twoVars) {
        auto oneExponent = oneVars.find(iter.first);

        if (oneExponent == oneVars.end()) { // If the variable is not present in the first term's map
            if (add) result.emplace(iter.first, iter.second);
            else result.emplace(iter.first, -(iter.second));
        }
        else { // If both terms have the same variable (regardless of exponent)
            if (add) result.emplace(iter.first, oneExponent->second + iter.second);
            else result.emplace(iter.first, oneExponent->second - iter.second);
        }
    }

    return result;
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the two given Terms can be added.
 * @return The sum of the two given Terms
 */
Term* add(Term* one, Term* two) {
    map<char, double> varsCopy = one->copyVariables();
    return new Term((one->getCoefficient() + two->getCoefficient()), &varsCopy);
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the two given Terms can be subtracted.
 * @return The difference of the two given Terms
 */
Term* subtract(Term* one, Term* two) {
    map<char, double> varsCopy = one->copyVariables();
    return new Term((one->getCoefficient() - two->getCoefficient()), &varsCopy);
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the two given Terms can be multiplied.
 * @return The product of the two given Terms
 */
Term* multiply(Term* one, Term* two) {
    map<char, double> addedVars = operateOnVarsMaps(one, two, true);
    return new Term((one->getCoefficient() * two->getCoefficient()), &addedVars);
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the two given Terms can be divided.
 * @return The quotient of the two given Terms
 */
Term* divide(Term* one, Term* two) {
    map<char, double> subtractedVars = operateOnVarsMaps(one, two, false);
    return new Term((one->getCoefficient() / two->getCoefficient()), &subtractedVars);
}

/**
 * Function assumes that operable() has been called first to ensure that
 * the exponentiation can be evaluated.
 * @return Term one raised to the value of Term two
 */
Term* pow(Term* one, Term* two) {
    // TODO implement pow()
    return new Term(1337, nullptr);
}

Term *operate(Term *one, Term *two, const string *op) {
    if (*op == "+") return add(one, two);
    else if (*op == "-") return subtract(one, two);
    else if (*op == "*") return multiply(one, two);
    else if (*op == "/") return divide(one, two);
    else if (*op == "^") return pow(one, two);
    else return new Term(1337, nullptr);
}