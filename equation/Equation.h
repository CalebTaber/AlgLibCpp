#ifndef ALGLIB_EQUATION_H
#define ALGLIB_EQUATION_H

#include <string>

#include "../expression/Expression.h"

class Equation {
private:
    Expression* left;
    Expression* right;

public:
    Equation(std::string eq) ;

    ~Equation() ;

    std::string toString() ;

    void solveFor(char variable) ;

    void print() ;
};


#endif //ALGLIB_EQUATION_H
