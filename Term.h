#ifndef ALGLIB_TERM_H
#define ALGLIB_TERM_H

#include <map>

class Term {
private:
    double coefficient;
    std::map<char, double> variables;

public:
    Term(double value, std::map<char, double> *variables);
    ~Term();
    double get_value() { return coefficient; }
    std::map<char, double> get_variables() { return variables; }
    void setValue(const double val) { coefficient = val; }
};


#endif //ALGLIB_TERM_H
