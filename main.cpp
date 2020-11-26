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
const char g_TERM[] = "[0-9]*[a-z]*[^]";

bool atEndOfString(int i, const string* s) {
    return (i == s->length() - 1);
}

map<char, double> addVariables(map<char, double> *one, map<char, double> *two) {
    map<char, double> new_vars;

    map<char, double>::iterator iter = (one->size() >= two->size()) ? one->begin() : two->begin();

    if (one->size() >= two->size()) {
        while (iter != one->end()) {
            map<char, double>::iterator find_in_two = two->find(iter->first);
            if (find_in_two != two->end()) {
                new_vars.emplace(iter->first, (iter->second + find_in_two->second));
            } else new_vars.emplace(iter->first, iter->second);

            iter++;
        }
    } else {
        while (iter != two->end()) {
            map<char, double>::iterator find_in_one = one->find(iter->first);
            if (find_in_one != two->end()) {
                new_vars.emplace(iter->first, (iter->second + find_in_one->second));
            } else new_vars.emplace(iter->first, iter->second);
            iter++;
        }
    }

    return new_vars;
}

// TODO Break up into multiple separate functions
Term *operate(Term *one, Term *two, const string *op) {
    double first = one->get_value();
    double second = two->get_value();

    map<char, double> one_vars = one->get_variables();
    map<char, double> two_vars = two->get_variables();
    map<char, double> variables = addVariables(&one_vars, &two_vars);

    cout << first << " " + *op + " " << second << endl;

    if (*op == "+") {
        return new Term(first + second, &variables);
    } else if (*op == "-") {
        return new Term(first - second, &variables);
    } else if (*op == "*") {
        return new Term(first * second, &variables);
    } else if (*op == "/") {
        return new Term(first / second, &variables);
    } else if (*op == "^") {
        return new Term(std::pow(first, second), &variables);
    }

    return new Term(1337, &variables);
}

void printTokens(const vector<string> *tokens) {
    for (string s : *tokens) {
        cout << s << endl;
    }
}

bool isOperator(const char c) {
    return g_OPERATORS.find(c) != string::npos;
}

bool isOperator(const string *s) {
    return isOperator(s->at(0));
}

Term* parse_term(const string *s) {
    Term* term;

    map<char, double> variables;

    bool parse_vars = false;
    int partition = -1;
    for (int i = 0, j = 0; i < s->length(); i++) {
        char c = s->at(i);

        // If at the end of the term
        if (i == s->length() - 1) {
            if (isalpha(c)) {
                j = i + 1;
                variables.emplace(c, 1);
            }

            if (parse_vars) {
                // Add the last variable to the map
                if (variables.find(j) != variables.end()) variables.emplace(s->at(j), std::stod(s->substr(j + 1, (i - j + 1))));
                term = new Term(std::stod(s->substr(0, partition)), &variables);
            } else term = new Term(std::stod(*s), &variables);
        }

        // If at the beginning of the variables
        if (!parse_vars && isalpha(c)) {
            j = i;
            partition = j;
            parse_vars = true;
            continue;
        }

        if (parse_vars) {
            if (isalpha(c)) variables.emplace(s->at(j), std::stod(s->substr(j + 1, (i - j))));
        }
    }

    return term;
}

Term *evaluate(queue<string> *tokens) {
    stack<Term *> output;

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
            // TODO add operable() call to check if the operation can go through
            output.push(operate(one, two, &front));
            delete one;
            delete two;
        } else {
            // If the token is an operand
            string number;
            if (front[0] == '`') front.replace(0, 1, "-"); // Check for a negation symbol
            output.push(parse_term(&front));
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

int main() {
    cout << "Enter an expression" << endl;
    string expression;
    std::getline(cin, expression);
    cout << expression << endl;

    string preprocessed = preprocess(&expression);
    cout << "Processed input: " << preprocessed << endl;
    vector<string> tokens = tokenizeExpression(&preprocessed);
    // print_tokens(&tokens);

    cout << endl;

    // Convert from infix to postfix notation
    queue<string> postfixed = infixToPostfix(&tokens);

    // Evaluate and get result
    Term *result = evaluate(&postfixed);
    cout << result->get_value() << endl;
    delete result;

    return 0;
}
