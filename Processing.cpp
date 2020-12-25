#include <iostream>

#include "Processing.h"
#include "Utils.h"

#define map std::map
#define string std::string
#define vector std::vector
#define queue std::queue
#define stack std::stack

#define cout std::cout
#define endl std::endl

void printTokens(const vector<string> *tokens) {
    for (string s : *tokens) {
        cout << s << endl;
    }
}

string preprocess(const string* input) {
    string processed;

    for (int j = 0, i = 0; i < input->length(); i++) {
        char c = input->at(i);

        // If at the end of the input string
        if (atEndOfString(i, input)) {
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
            else if (!isOperator(&b) && b != "`") {
                processed.append(input->substr(j, (i - j)));
                processed.append("*");
                j = i;
                continue;
            }
        }
    }

    return processed;
}

vector<string> tokenizeExpression(const string *expression) {
    vector<string> tokens;

    for (int i = 1, j = 0; i < expression->length(); i++) {
        const char c = expression->at(i);

        if (atEndOfString(i, expression)) { // If at end of the expression
            if (isOperator(c)) {
                tokens.push_back(expression->substr(j, (i - j))); // Add operand
                tokens.push_back(expression->substr(i, 1)); // Add operator that's at the end of the expression
            }
            else tokens.push_back(expression->substr(j, (expression->length() - j)));
        } else if (isOperator(c)) { // If c is an operator
            if (j != i) tokens.push_back(expression->substr(j, (i - j)));
            tokens.push_back(expression->substr(i, 1));
            j = i + 1;
        }
    }

    return tokens;
}

/**
 * Applies the Shunting-Yard Algorithm to convert a vector
 * of tokens in infix order into a queue in postfix order
 */
queue<string> infixToPostfix(vector<string> *tokens) {
    queue<string> postfixed;
    stack<string> operators;

    for (string s : *tokens) {
        if (isOperator(&s)) { // If the token is an operator
            int top_precedence = (operators.empty()) ? -1 : g_precedence.at(operators.top().at(0));
            int this_precedence = g_precedence.at(s.at(0));

            if (s == ")") {
                while (operators.top() != "(") {
                    postfixed.push(operators.top());
                    operators.pop();
                }

                operators.pop(); // Pop the '(' from the stack
            }
            else if (!operators.empty() && operators.top() == "(") operators.push(s);
            else if (top_precedence < this_precedence) operators.push(s);
            else {
                while (!operators.empty() && top_precedence > this_precedence) {
                    postfixed.push(operators.top());
                    operators.pop();

                    top_precedence = (operators.empty()) ? -1 : g_precedence.at(operators.top().at(0));
                }

                operators.push(s);
            }
        }
        else postfixed.push(s); // If the token is an operand
    }

    while (!operators.empty()) {
        postfixed.push(operators.top());
        operators.pop();
    }

    return postfixed;
}