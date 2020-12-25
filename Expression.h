#ifndef ALGLIB_EXPRESSION_H
#define ALGLIB_EXPRESSION_H

#include <vector>
#include <string>
#include <queue>

#include "Term.h"

class Expression {
private:
    std::vector<Term*> terms;

public:
    Expression(std::string input);

    Expression(std::vector<Term*> termQueue);

    Expression* evaluate(std::queue<std::string> *tokens);

    std::string toString();

    void print();

};


#endif //ALGLIB_EXPRESSION_H
