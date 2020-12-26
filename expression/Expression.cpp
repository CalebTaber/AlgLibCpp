#include <stack>
#include <iostream>

#include "Expression.h"
#include "../utils/Utils.h"
#include "../math/Arithmetic.h"

#define vector std::vector
#define string std::string
#define stack std::stack
#define queue std::queue

#define cout std::cout
#define endl std::endl

const std::map<char, int> g_precedence = {
        {'+', 1},
        {'-', 1},
        {'*', 2},
        {'/', 2},
        {'^', 3},
        {'(', 4},
        {')', 4}
};

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
                processed.append("+");
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

    // When parsing a term with exponentiated variables, this flag
    // prevents the carets from being parsed independently of the rest
    // of the term
    bool parsingTerm = false;

    for (int i = 1, j = 0; i < expression->length(); i++) {
        const char c = expression->at(i);

        if (isalpha(c)) parsingTerm = true;

        if (atEndOfString(i, expression)) { // If at end of the expression
            if (isOperator(c)) {
                if (parsingTerm && c == ')') tokens.push_back(expression->substr(j, (i - j) + 1));
                else {
                    tokens.push_back(expression->substr(j, (i - j))); // Add operand
                    tokens.push_back(expression->substr(i, 1)); // Add operator that's at the end of the expression
                }
            }
            else tokens.push_back(expression->substr(j, (expression->length() - j)));
        } else if (isOperator(c)) { // If c is an operator
            if (parsingTerm && isOperator(c) && expression->at(i-1) == ')') parsingTerm = false;
            if (!parsingTerm) {
                if (j != i) tokens.push_back(expression->substr(j, (i - j)));
                tokens.push_back(expression->substr(i, 1));
                j = i + 1;
            }
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

Expression::Expression(const string input) {
    string preprocessed = preprocess(&input);
    cout << "Processed input: " << preprocessed << endl;
    vector<string> tokens = tokenizeExpression(&preprocessed);

    /*
    for (auto & t : tokens) {
        cout << t << endl;
    }
    */

    // Convert from infix to postfix notation
    queue<string> postfixed = infixToPostfix(&tokens);

    /*
    while (!postfixed.empty()) {
        cout << postfixed.front() << endl;
        postfixed.pop();
    }
    */

    evaluate(&postfixed);
}

Expression::Expression(vector<Term*> *input) {
    terms = *input;
    simplify();
}

Expression::~Expression() {
    for (auto t : terms) {
        delete t;
    }
}

void Expression::simplify() {
    // TODO
}

/**
 * Evaluates the given queue of tokens as an expression.
 * Note: The tokens must be in postfix order, generally achieved
 * by calling the function infixToPostfix() before evaluate()
 */
void Expression::evaluate(queue<string> *tokens) {
    stack<Term*> output;

    while (!tokens->empty()) {
        string front = tokens->front();
        if (isOperator(&front)) {
            // Pop operands and operate
            // Note: the operands are popped in reverse order because they're in a stack
            Term* two = output.top();
            cout << two->toString() << endl;
            output.pop();

            Term* one = output.top();
            cout << one->toString() << endl;
            output.pop();

            // Push the result
            if (operable(one, two, &front)) {
                output.push(operate(one, two, &front));
                delete one;
                delete two;
            }
            else {
                output.push(one);
                output.push(two);
            }
        } else {
            // If the token is an operand
            string number;
            if (front[0] == '`') front.replace(0, 1, "-"); // Check for a negation symbol
            output.push(Term::parseTerm(&front));
        }

        tokens->pop();
    }

    while (!output.empty()) {
        terms.push_back(output.top());
        output.pop();
    }

    simplify();
}

string Expression::toString() {
    string stringified;

    for (auto term : terms) {
        stringified += term->toString() + " ";
    }

    return stringified;
}

void Expression::print() {
    cout << toString() << endl;
}
