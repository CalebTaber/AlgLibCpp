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

string formatInputExpression(const string* s) {
    string input = removeSpaces(s);
    string processed;

    for (int j = 0, i = 0; i < input.length(); i++) {
        char c = input.at(i);

        // If at the end of the input string
        if (endOfString(i, &input)) {
            processed.append(input.substr(j, (i - j + 1)));
            break;
        }

        if (c == '-') {
            processed.append(input.substr(j, (i - j)));
            char pLast = (processed.empty()) ? ' ' : processed[processed.length() - 1];

            if (pLast == '-') { // If there are two or more negations in a row
                if (processed.length() > 1) {
                    if (processed[processed.length() - 2] == '+') processed.replace(processed.length() - 2, processed.length(), "+");
                    else processed[processed.length() - 1] = '+';
                }
            }
            else {
                if (pLast == ' ' || (isOperator(pLast) && pLast != ')')) processed.append("-"); // pLast is only ' ' when processed is empty
                else processed.append("+-");
            }

            j = i + 1;
        }
        else if (c == '(') {
            char b = (i > 0) ? input.at(i - 1) : ' ';

            if (i == 0) continue;
            else if (!isOperator(b) && b != '-') {
                processed.append(input.substr(j, (i - j)));
                processed.append("*");
                j = i;
            } else if (b == '-') {
                processed.append(input.substr(j, i - j));
                processed.append("1*");
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

        if (endOfString(i, expression)) {
            if (isOperator(c)) {
                if (parsingVariables && c == ')') {
                    if (expression->at(i - 1) == ')') {
                        tokens.push_back(expression->substr(j, (i - j)));
                        tokens.emplace_back(")");
                    }
                    else tokens.push_back(expression->substr(j, (i - j) + 1));
                }
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
            int top_precedence = (operators.empty()) ? -1 : opPrecedence(operators.top().at(0));
            int this_precedence = opPrecedence(s.at(0));

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
                while (!operators.empty() && ((operators.top() != "(" && top_precedence >= this_precedence) || (operators.top() == ")" && top_precedence > this_precedence))) {
                    postfixed.push(operators.top());
                    operators.pop();

                    top_precedence = (operators.empty()) ? -1 : opPrecedence(operators.top().at(0));
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
//    cout << "Processed input: " << formatted << endl;
    vector<string> tokens = tokenizeExpression(&formatted);

//    for (auto & t : tokens) {
//        cout << t << endl;
//    }
//    cout << "END TOKENS" << endl;

    // Convert from infix to postfix notation
    queue<string> postfixed = infixToPostfix(&tokens);

//    while (!postfixed.empty()) {
//        cout << postfixed.front() << endl;
//        postfixed.pop();
//    }

    evaluate(&postfixed);
}

Expression::~Expression() {
    for (auto s : sortedTerms) {
        stack<Term*> terms;
        while (!terms.empty()) {
            Term* tmp = terms.top();
            terms.pop();

            delete tmp;
        }
    }
}

/**
 * Evaluates the given queue of tokens as an expression.
 * Note: The tokens must be in postfix order, generally achieved
 * by calling the function infixToPostfix() before evaluate()
 */
void Expression::evaluate(queue<string> *tokens) {
    stack<Term*> output;

    vector<Term*> terms;

    while (!tokens->empty()) {
        string front = tokens->front();
        if (isOperator(&front)) {
            // Pop operands and operate
            // Note: the operands are popped in reverse order because they're in postfix order
            Term* two = output.top();
//             cout << two->toString() << endl;
            output.pop();

            Term* one = output.top();
            // cout << one->toString() << endl;
            output.pop();

            // Push the result
            if (operable(one, two, &front)) {
                Term* r = operate(one, two, &front);
//                cout << r->toString() << endl;
                output.push(r);
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
            output.push(Term::parseTerm(&front));
        }

        tokens->pop(); // Pop operator
    }

    while (!output.empty()) {
        terms.push_back(output.top());
        output.pop();
    }

    insertTerms(&terms);
}

/**
 * Inserts the given Term into the sortedTerms map according to its variable composition
 */
void Expression::insertAndSortTerm(Term* t) {
    auto findTVars = sortedTerms.find(t->varsToString());

    if (findTVars != sortedTerms.end()) findTVars->second.push(t);
    else {
        stack<Term*> v;
        v.push(t);
        sortedTerms.emplace(t->varsToString(), v);
    }
}

/**
 * Inserts the given terms into the sorted map, then adds the like terms together
 */
void Expression::insertTerms(vector<Term*>* terms) {
    for (auto t : *terms) {
        insertAndSortTerm(t);
    }
    simplify();
}

/**
 * Iterates through the map of sorted Terms and adds the like Terms
 */
void Expression::simplify() {
    for (const auto & vars : sortedTerms) {
        stack<Term*> termStack = vars.second;
        while (termStack.size() > 1) {
            // Pop top two terms and add. Then push result. Repeat until only one term remains
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

        sortedTerms.find(vars.first)->second = termStack; // Put the new stack back in the map
    }
}

/**
 * Multiply this Expression by the given multiplicand Expression
 */
void Expression::multiply(Expression* multiplicand) {
    simplify();
    vector<Term*> products;

    // There will only be one Term in each stack since simplify() is called at the start of the function
    for (const auto& s : sortedTerms) {
        for (const auto& s2 : multiplicand->getTerms()) {
            products.push_back(multiplyTerms(s.second.top(), s2.second.top()));
        }
    }

    sortedTerms.clear();
    insertTerms(&products);
}

string Expression::toString() {
    string stringified;

    for (auto iter = --sortedTerms.end(); iter != sortedTerms.begin(); iter--) {
        stringified.append(iter->second.top()->toString());
        stringified.append("+");
    }

    // Add final term
    stringified.append(sortedTerms.begin()->second.top()->toString());

    return stringified;
}
