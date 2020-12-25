#include <stack>
#include <iostream>

#include "Expression.h"
#include "Utils.h"
#include "Arithmetic.h"
#include "Processing.h"

#define vector std::vector
#define string std::string
#define stack std::stack
#define queue std::queue

#define cout std::cout
#define endl std::endl

Expression::Expression(const string input) {
    string preprocessed = preprocess(&input);
    cout << "Processed input: " << preprocessed << endl;
    vector<string> tokens = tokenizeExpression(&preprocessed);
    // print_tokens(&tokens);

    // Convert from infix to postfix notation
    queue<string> postfixed = infixToPostfix(&tokens);
}

Expression* evaluate(queue<string> *tokens) {
    stack<Term*> output;

    while (!tokens->empty()) {
        string front = tokens->front();
        if (isOperator(&front)) {
            // Pop operands and operate
            // Note: the operands are popped in reverse order because they're in a stack
            Term *two = output.top();
            output.pop();

            Term *one = output.top();
            output.pop();

            // TODO implement an Expression struct/class
            // Be able to return an Expression from this function. An Expression
            // is a list of Terms

            // Push the result
            if (operable(one, two, &front)) output.push(operate(one, two, &front));
            // else TODO handle inoperability
            delete one;
            delete two;
        } else {
            // If the token is an operand
            string number;
            if (front[0] == '`') front.replace(0, 1, "-"); // Check for a negation symbol
            output.push(Term::parseTerm(&front));
        }

        tokens->pop();
    }

    return nullptr; // TODO change
}

string Expression::toString() {
    string stringified;

    for (auto iter : terms) {
        stringified += iter->toString();
    }

    return stringified;
}

void Expression::print() {
    cout << toString() << endl;
}
