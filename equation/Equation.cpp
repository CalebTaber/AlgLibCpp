#include "Equation.h"

#define string std::string

Equation::Equation(string input) {
    int eqIndex = input.find('=');
    left = new Expression(input.substr(0, eqIndex));
    right = new Expression(input.substr(eqIndex + 1, input.length() - (eqIndex + 1)));
}

Equation::~Equation() {
    delete left;
    delete right;
}

void Equation::solveFor(char variable) {
    // Need to subtract any terms from the left that do not include the given variable
    // Then need to divide out any other variables that may be attached to the given one


}

string Equation::toString() {
    string result;

    result.append(left->toString());
    result.append("=");
    result.append(right->toString());

    return result;
}
