#include "Equation.h"



#define string std::string

Equation::Equation(string eq) {
    int index = eq.find('=');
    left = new Expression(eq.substr(0, index));
    right = new Expression(eq.substr(index + 1, eq.length() - (index + 1)));
}

Equation::~Equation() {
    delete left;
    delete right;
}

void Equation::solveFor(char variable) {

}

string Equation::toString() {
    string result;

    result.append(left->toString());
    result.append("=");
    result.append(right->toString());

    return result;
}

void Expression::print() {

}
