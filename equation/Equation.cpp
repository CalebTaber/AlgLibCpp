#include "Equation.h"
#include "../math/Arithmetic.h"

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
    left->simplify();

    // Move Terms without the variable from left side to right
    for (auto iter : left->getTerms()) {
        if (iter.first.find(variable) != string::npos) {
            Term* negOne = new Term(-1, nullptr);
            right->insertTerm(multiplyTerms(iter.second.top(), negOne));
            delete negOne;

            // Delete old Term from left side
            Term* t = iter.second.top();
            iter.second.pop();
            // delete t;
            left->removeTerms(iter.first);
        }
    }

    right->simplify();

    // Move Terms with the variable from right side to left
    for (auto iter : right->getTerms()) {
        if (iter.first.find(variable) != string::npos) {
            Term* negOne = new Term(-1, nullptr);
            left->insertTerm(multiplyTerms(iter.second.top(), negOne));
            delete negOne;

            // Delete old Term from left side
            Term* t = iter.second.top();
            iter.second.pop();
            // delete t;
            right->removeTerms(iter.first);
        }
    }

    left->simplify();

    // As of 01/16/2021, this function assumes that the equation is linear with one variable,
    // so there would only be one term on the left at this point
    Term* one = new Term(1, nullptr);
    auto* multiplicand = new Expression;
    multiplicand->insertTerm(divideTerms(one, new Term(left->getTerms().begin()->second.top()->getCoefficient(), nullptr)));

    right->multiply(multiplicand);
    left->multiply(multiplicand);

    delete one;
    delete multiplicand;
}

string Equation::toString() {
    string result;

    result.append(left->toString());
    result.append("=");
    result.append(right->toString());

    return result;
}
