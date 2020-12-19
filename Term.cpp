#include "Term.h"

Term::Term(double value, std::map<char, double> *variables) {
    this->coefficient = value;
    if (variables != nullptr) this->variables = *variables;
}

Term::~Term() {
    // TODO
}
