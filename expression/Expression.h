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

    void addLikeTerms();

public:
    Expression(std::string input);

    Expression(std::vector<Term*> *input);

    ~Expression();

    std::string toString();

    void print();

};


#endif //ALGLIB_EXPRESSION_H
