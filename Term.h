#ifndef ALGLIB_TERM_H
#define ALGLIB_TERM_H


class Term {
private:
    double value;

public:
    Term(double value);
    ~Term();
    double getValue() { return value; }
    void setValue(const double val) { value = val; }
};


#endif //ALGLIB_TERM_H
