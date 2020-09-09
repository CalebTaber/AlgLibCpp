#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <cmath>
#include <vector>
#include "Term.h"

#define cout std::cout
#define stack std::stack
#define queue std::queue
#define cin std::cin
#define string std::string
#define endl std::endl
#define vector std::vector
#define map std::map

const map<char, int> g_precedence = {
        {'+', 1},
        {'-', 1},
        {'*', 2},
        {'/', 2},
        {'^', 3},
        {'(', 4},
        {')', 4}
};
const string g_OPERATORS = "+-*/^()";

Term *operate(Term *one, Term *two, const string *op) {
    double first = one->getValue();
    double second = two->getValue();

    cout << first << " " + *op + " " << second << endl;

    if (*op == "+") {
        return new Term(first + second);
    } else if (*op == "-") {
        return new Term(first - second);
    } else if (*op == "*") {
        return new Term(first * second);
    } else if (*op == "/") {
        return new Term(first / second);
    } else if (*op == "^") {
        return new Term(std::pow(first, second));
    }

    return new Term(-9999);
}

void print_tokens(const vector<string> *tokens) {
    for (string s : *tokens) {
        cout << s << endl;
    }
}

bool is_Operator(const string *s) {
    return g_OPERATORS.find(s->at(0)) != string::npos;
}

bool is_Operator(const char *c) {
    return g_OPERATORS.find(*c) != string::npos;
}

Term *evaluate(queue<string> *tokens) {
    stack<Term *> output;

    while (!tokens->empty()) {
        string front = tokens->front();
        if (is_Operator(&front)) { // If the token is an operator
            // Pop operands and operate
            // Note: the operands are popped in reverse order because they're in a stack
            Term *two = output.top();
            output.pop();

            Term *one = output.top();
            output.pop();

            // Push the result
            output.push(operate(one, two, &front));
        } else output.push(new Term(std::stof(front))); // If the token is an operand

        tokens->pop();
    }

    return output.top();
}

vector<string> tokenize(const string *expression) {
    vector<string> tokens;

    for (int i = 1, j = 0; i < expression->length(); i++) {
        const char c = expression->at(i);

        if (i == expression->length() - 1) { // If at end of the expression
            if (is_Operator(&c)) {
                tokens.push_back(expression->substr(j, (i - j))); // Add operand
                tokens.push_back(expression->substr(i, 1)); // Add operator that's at the end of the expression
            }
            else tokens.push_back(expression->substr(j, (expression->length() - j)));
        } else if (is_Operator(&c)) { // If c is an operator
            if (j != i) tokens.push_back(expression->substr(j, (i - j)));
            tokens.push_back(expression->substr(i, 1));
            j = i + 1;
        }
    }

    return tokens;
}

queue<string> postfixify(vector<string> *tokens) {
    queue<string> postfixed;
    stack<string> operators;

    for (string s : *tokens) {
        if (is_Operator(&s)) { // If the token is an operator
            int top_precedence = (operators.empty()) ? -1 : g_precedence.at(operators.top().at(0));
            int this_precedence = g_precedence.at(s.at(0));

            if (s == ")") {
                while (operators.top() != "(") {
                    postfixed.push(operators.top());
                    operators.pop();
                }

                operators.pop(); // Pop the '(' from the stack
            }
            else if ((!operators.empty() && operators.top() == "(") || top_precedence < this_precedence) operators.push(s);
            else {
                while (!operators.empty() && top_precedence > this_precedence) {
                    postfixed.push(operators.top());
                    operators.pop();

                    top_precedence = (operators.empty()) ? -1 : g_precedence.at(operators.top().at(0));
                }

                operators.push(s);
            }
        } else postfixed.push(s); // If the token is an operand
    }

    while (!operators.empty()) {
        postfixed.push(operators.top());
        operators.pop();
    }

    return postfixed;
}

int main() {
    // Get input expression from user
    cout << "Enter an expression" << endl;
    string expression;
    std::getline(cin, expression);
    cout << expression << endl;

    vector<string> tokens = tokenize(&expression);
    print_tokens(&tokens);

    cout << endl;

    // Convert from infix to postfix notation
    queue<string> postfixed = postfixify(&tokens);

    // Evaluate and get result
    Term *result = evaluate(&postfixed);
    cout << result->getValue() << endl;

    // Apply the shunting yard algorithm to the token vector
    // evaluate(&tokens);

    return 0;
}
