#ifndef ALGLIB_EQUATION_H
#define ALGLIB_EQUATION_H

#include <string>
#include <iostream>

#include "../expression/Expression.h"

class Equation {
private:
    Expression* left;
    Expression* right;

public:
    Equation(std::string input) ;

    ~Equation() ;

    std::string toString() ;

    void solveFor(char variable) ;

    void print() { std::cout << toString() << std::endl; };
};


#endif //ALGLIB_EQUATION_H
