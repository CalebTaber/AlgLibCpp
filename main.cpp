#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <cmath>
#include "Term.h"

#define cout std::cout
#define stack std::stack
#define queue std::queue
#define cin std::cin
#define string std::string
#define endl std::endl

Term* operate(Term* one, Term* two, const string* op) {
    double first = one->getValue();
    double second = two->getValue();
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

bool isOperator(string* s) {
    const string operators[] = {"+", "-", "*", "/", "^"};

    for (int i = 0; i < operators->length(); i++) {
        // cout << operators[i] << "\t" << *s << endl;

        if (operators[i] == *s) return true;
    }

    return false;
}

// TODO change void* to string and fix it

queue<void*> tokenize(string* expression) {
    queue<void*> tokens;

    for (int i = 1, j = 0; i < expression->length(); i++) {
        string c = expression->substr(i, i);
        if (i == expression->length() - 1) {
            // If at end of string, add last term
            tokens.push(new Term(std::stod(expression->substr(j, i + 1))));
        } else if (isOperator(&c)) {
            tokens.push(new Term(std::stod(expression->substr(j, i))));
            tokens.push(new string(c)); // Make a copy of the string so that the memory addresses don't get all wacky
        }
    }

    return tokens;
}

void sya(string* expression) {

    std::map<string, int> opsLevels;
    opsLevels.emplace("+", 2);
    opsLevels.emplace("-", 2);
    opsLevels.emplace("*", 3);
    opsLevels.emplace("/", 3);
    opsLevels.emplace("^", 4);

    stack<string> ops;
    stack<Term*> output;

    queue<void*> tokens = tokenize(expression);
    cout << (tokens.empty()) << endl;
    while (!tokens.empty()) {
        cout << tokens.front() << endl;
        tokens.pop();
    }

    // cout << "Answer: " << output.top()->getValue() << endl;
}

int main() {
    // Get input expression from user
    cout << "Enter an expression" << endl;
    string expression;
    std::getline(cin, expression);
    // TODO remove spaces from expression
    cout << expression << endl;

    sya(&expression);

    return 0;
}
