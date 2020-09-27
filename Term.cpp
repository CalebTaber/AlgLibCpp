#include "Term.h"
#include <cstdlib>

Term::Term(double value, std::map<char, double> *variables) {
    this->coefficient = value;
    this->variables = *variables;
}

Term::~Term() {
    // TODO
}
