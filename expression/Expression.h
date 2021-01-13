#ifndef ALGLIB_EXPRESSION_H
#define ALGLIB_EXPRESSION_H

#include <vector>
#include <string>
#include <queue>

#include "../term/Term.h"

class Expression {
private:
    std::vector<Term*> terms;

    void evaluate(std::queue<std::string> *tokens);

public:
    Expression(std::string input);

    ~Expression();

    void addLikeTerms();

    std::string toString();

    void print();

};


#endif //ALGLIB_EXPRESSION_H
