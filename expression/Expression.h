#ifndef ALGLIB_EXPRESSION_H
#define ALGLIB_EXPRESSION_H

#include <vector>
#include <string>
#include <queue>

#include "../term/Term.h"

class Expression {
private:
    std::vector<Term*> terms;
    std::vector<std::string> operators; // TODO implement

    void evaluate(std::queue<std::string> *tokens);

    void simplify();

public:
    Expression(std::string input);

    Expression(std::vector<Term*> *termQueue);

    std::string toString();

    void print();

};


#endif //ALGLIB_EXPRESSION_H
