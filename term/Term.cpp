#include "Term.h"

#define string std::string
#define map std::map

Term::Term(double value, map<char, double> *variables) {
    this->coefficient = value;
    if (variables != nullptr) this->variables = *variables;
}

string Term::toString() {
    string result;

    result += std::to_string(coefficient);
    result += varsToString();

    return result;
}

string Term::varsToString() {
    string varString;
    if (variables.empty()) return varString;

    for (auto & variable : variables) {
        varString += variable.first;
        varString += "^(";
        varString += std::to_string(variable.second);
        varString += ")";
    }

    return varString;
}

bool Term::varsEqual(Term* t) {
    map<char, double> twoVars = t->getVariables();

    if (variables.size() != twoVars.size()) return false;

    for (auto & iter : variables) {
        auto var = twoVars.find(iter.first);

        if (var == twoVars.end()) return false;
        if (var->second != variables.find(iter.first)->second) return false;
    }

    for (auto & iter : twoVars) {
        auto var = variables.find(iter.first);

        if (var == variables.end()) return false;
        if (var->second != twoVars.find(iter.first)->second) return false;
    }

    return true;
}

bool Term::equals(Term* t) {
    return (coefficient == t->getCoefficient() && varsEqual(t));
}

map<char, double> Term::copyVariables() {
    map<char, double> copy;

    for (auto iter : variables) {
        copy.emplace(iter.first, iter.second);
    }

    return copy;
}

Term* Term::parseTerm(const string *s) {
    Term* term;

    map<char, double> variables;

    bool parse_vars = false;
    int partition = -1;
    for (int i = 0, j = 0; i < s->length(); i++) {
        char c = s->at(i);

        // If at the end of the term
        if (i == s->length() - 1) {
            if (isalpha(c)) {
                j = i + 1;
                variables.emplace(c, 1);
            }

            if (parse_vars) {
                // Add the last variable to the map
                if (variables.find(j) != variables.end()) variables.emplace(s->at(j), std::stod(s->substr(j + 1, (i - j + 1))));
                term = new Term(std::stod(s->substr(0, partition)), &variables);
            } else term = new Term(std::stod(*s), &variables);
        }

        // If at the beginning of the variables
        if (!parse_vars && isalpha(c)) {
            j = i;
            partition = j;
            parse_vars = true;
            continue;
        }

        if (parse_vars) {
            if (isalpha(c)) variables.emplace(s->at(j), std::stod(s->substr(j + 1, (i - j))));
        }
    }

    return term;
}
