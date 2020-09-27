#include <iostream>
#include <string>
#include <stack>
#include <cctype>
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

    return new Term(1337);
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
            delete one;
            delete two;
        } else {
            // If the token is an operand
            string number;
            if (front[0] == '`') front.replace(0, 1, "-"); // Check for a negation symbol
            output.push(new Term(std::stod(front)));
        }

        tokens->pop();
    }

    return output.top();
}

string preprocess(const string* input) {
    string processed;

    for (int j = 0, i = 0; i < input->length(); i++) {
        char c = input->at(i);

        // If at the end of the input string
        if (i == input->length() - 1) {
            processed.append(input->substr(j, (i - j + 1)));
            continue;
        }

        /** ---------------Processing-------------- **/

        if (c == ' ') { // Eliminate spaces
            processed.append(input->substr(j, (i - j)));
            j = i + 1;
            continue;
        }
        else if (c == '-') {
            // If the '-' is negating a term, not indicating subtraction
            // TODO this may break parsing when variables are introduced to expressions
            if (i == 0 || (!isdigit(input->at(i - 1)) && !isalpha(input->at(i - 1)))) {
                processed.append(input->substr(j, (i - j)));
                processed.append("`");
                j = i + 1;
                continue;
            }
        }
        else if (c == '(') {
            string b = input->substr(i - 1,1);

            if (i == 0) continue;
            else if (!is_Operator(&b) && b != "`") {
                processed.append(input->substr(j, (i - j)));
                processed.append("*");
                j = i;
                continue;
            }
        }
    }

    return processed;
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

queue<string> infix_to_postfix(vector<string> *tokens) {
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

    string preprocessed = preprocess(&expression);
    cout << "Processed input: " << preprocessed << endl;
    vector<string> tokens = tokenize(&preprocessed);
    // print_tokens(&tokens);

    cout << endl;

    // Convert from infix to postfix notation
    queue<string> postfixed = infix_to_postfix(&tokens);

    // Evaluate and get result
    Term *result = evaluate(&postfixed);
    cout << result->getValue() << endl;
    delete result;

    return 0;
}
