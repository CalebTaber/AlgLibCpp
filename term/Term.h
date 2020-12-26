#ifndef ALGLIB_TERM_H
#define ALGLIB_TERM_H

#include <map>
#include <string>

class Term {
private:
    double coefficient;
    std::map<char, double> variables;
    std::string varsToString();

public:
    Term(double value, std::map<char, double> *variables);

    double getCoefficient() { return coefficient; }

    std::map<char, double> getVariables() { return variables; }

    void setValue(const double val) { coefficient = val; }

    std::string toString();

    bool varsEqual(Term* t);

    bool equals(Term* t);

    std::map<char, double> copyVariables();

    static Term* parseTerm(const std::string* s) ;
};


#endif //ALGLIB_TERM_H
