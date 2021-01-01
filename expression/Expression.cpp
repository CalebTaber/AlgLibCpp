#include <stack>
#include <iostream>

#include "Expression.h"
#include "../utils/Utils.h"
#include "../math/Arithmetic.h"
#include <map>

#define vector std::vector
#define string std::string
#define stack std::stack
#define queue std::queue
#define map std::map

#define cout std::cout
#define endl std::endl

const map<char, int> g_precedence = {
        {'+', 1},
        {'-', 1},
        {'*', 2},
        {'/', 2},
        {'^', 3},
        {'(', 4},
        {')', 4}
};

string removeSpaces(const string* s) {
    string result;

    for (char c : *s) {
        if (c != ' ') result += c;
    }

    return result;
}

string formatInputExpression(const string* s) {
    string input = removeSpaces(s);
    string processed;

    for (int j = 0, i = 0; i < input.length(); i++) {
        char c = input.at(i);

        // If at the end of the input string
        if (atEndOfString(i, &input)) {
            processed.append(input.substr(j, (i - j + 1)));
            break;
        }

        /** ---------------Formatting-------------- **/
        if (c == '-') {
            processed.append(input.substr(j, (i - j)));
            char pLast = (processed.length() > 0) ? processed.at(processed.length() - 1) : ' ';

            if (pLast == '`') { // If there are two or more negations in a row
                if(processed.length() > 1) {
                    if (processed.at(processed.length() - 2) == '+') processed.replace(processed.length() - 2, processed.length(), "+");
                    else processed[processed.length() - 1] = '+';
                }
            }
            else {
                if (pLast == ' ' || (isOperator(pLast) && pLast != ')')) processed.append("`"); // pLast is only ' ' when processed is empty
                else processed.append("+`");
            }

            j = i + 1;
        }
        else if (c == '(') {
            string b = (i > 0) ? input.substr(i - 1,1) : "";

            if (i == 0) continue;
            else if (!isOperator(&b) && b != "`") {
                processed.append(input.substr(j, (i - j)));
                processed.append("*");
                j = i;
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
    bool parsingVariables = false;

    for (int i = 0, j = 0; i < expression->length(); i++) {
        const char c = expression->at(i);

        if (isalpha(c)) parsingVariables = true;

        if (atEndOfString(i, expression)) { // If at end of the expression
            if (isOperator(c)) {
                if (parsingVariables && c == ')') tokens.push_back(expression->substr(j, (i - j) + 1));
                else {
                    if (i > j) tokens.push_back(expression->substr(j, (i - j))); // Add operand if there is one
                    tokens.push_back(expression->substr(i, 1)); // Add operator that's at the end of the expression
                }
            }
            else tokens.push_back(expression->substr(j, (expression->length() - j)));
        } else if (isOperator(c)) { // If c is an operator
            if (parsingVariables && isOperator(c) && expression->at(i - 1) == ')') parsingVariables = false;
            if (!parsingVariables) {
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
    string formatted = formatInputExpression(&input);
    cout << "Processed input: " << formatted << endl;
    vector<string> tokens = tokenizeExpression(&formatted);

    /*
    for (auto & t : tokens) {
        cout << t << endl;
    }
    cout << "END TOKENS" << endl;
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
    addLikeTerms();
}

Expression::~Expression() {
    for (auto t : terms) {
        delete t;
    }
}

void Expression::addLikeTerms() {
    map<string, stack<Term*>> likeTerms;

    for (auto t : terms) {
        auto findTVars = likeTerms.find(t->varsToString());

        if (findTVars != likeTerms.end()) findTVars->second.push(t);
        else {
            stack<Term*> v;
            v.push(t);
            likeTerms.emplace(t->varsToString(), v);
        }
    }

    for (const auto & vars : likeTerms) {
        stack<Term*> termStack = vars.second;
        while (termStack.size() > 1) {
            // Pop top two terms and add. Then push result. Repeat.
            Term* one = termStack.top();
            termStack.pop();
            Term* two = termStack.top();
            termStack.pop();

            const string op = "+";
            Term* result = operate(one, two, &op);
            termStack.push(result);

            delete one;
            delete two;
        }

        likeTerms.find(vars.first)->second = termStack; // Put the new stack back in the map
    }

    terms.clear();
    for (const auto & vars : likeTerms) {
        terms.push_back(vars.second.top());
    }
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
            // Note: the operands are popped in reverse order because they're in postfix order
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
        }
        else {
            // If the token is an operand
            string number;
            if (front[0] == '`') front.replace(0, 1, "-"); // Check for a negation symbol
            output.push(Term::parseTerm(&front));
        }

        tokens->pop(); // Pop operator
    }

    while (!output.empty()) {
        terms.push_back(output.top());
        output.pop();
    }

    addLikeTerms();
}

string Expression::toString() {
    string stringified;

    for (int i = (int) terms.size() - 1, o = 0; i >= 0; i--, o++) {
        stringified += terms.at(i)->toString();
        if (i > 0) stringified += " + ";
    }

    return stringified;
}

void Expression::print() {
    cout << toString() << endl;
}
