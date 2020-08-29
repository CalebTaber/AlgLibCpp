#ifndef ALGLIB_TERM_H
#define ALGLIB_TERM_H


class Term {
private:
    double value;

public:
    Term(double val);
    double getValue() { return value; }
    void setValue(double value) { this->value = value; }
};


#endif //ALGLIB_TERM_H
