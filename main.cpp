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

struct token {
    bool is_term;
    Term* term;
    string op;
};

void print_token(const token* t) {
    if (t->is_term) {
        cout << t->term->getValue() << endl;
    } else {
        cout << t->op << endl;
    }
}

Term* operate(Term* one, Term* two, const string* op) {
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

bool isOperator(const string* s) {
    const vector<string> operators = {"+", "-", "*", "/", "^"};

    for (int i = 0; i < operators.size(); i++) {
        if (operators[i] == *s) return true;
    }

    return false;
}

void print_tokens(queue<token*> tokens) {
    while (!tokens.empty()) {
        if (tokens.front()->is_term) cout << tokens.front()->term->getValue() << " ";
        else cout << tokens.front()->op << " ";
        tokens.pop();
    }

    cout << endl;
}

void print_tokens(stack<token*> tokens) {
    while (!tokens.empty()) {
        cout << tokens.top()->op << " ";
        tokens.pop();
    }

    cout << endl;
}

void sya(queue<token> tokens) {
    cout << endl;

    // TODO add parenthesis functionality
    std::map<string, int> precedence;
    precedence["+"] = 2;
    precedence["-"] = 2;
    precedence["*"] = 3;
    precedence["/"] = 3;
    precedence["^"] = 4;

    queue<token*> output;
    stack<token*> operators;

    while (!tokens.empty()) {
        if (tokens.front().is_term) {
            output.push(&tokens.front()); // If term, add to queue
            // cout << "push " << tokens.front().term->getValue() << endl;
            tokens.pop();
        } else {
            string op = tokens.front().op;
            string top_op = (!operators.empty()) ? operators.top()->op : "";

            // If stack is empty or the top operator has lower precedence, push the op
            if (operators.empty() || precedence[top_op] < precedence[op]) {
                // cout << "push " << op << endl;
                operators.push(&tokens.front());
            } else {
                // Pop operators until an equal or lower precedence operator is encountered
                print_tokens(output);
                print_tokens(operators);
                while (precedence[top_op] >= precedence[op]) {
                    Term* one = output.front()->term;
                    output.pop();
                    Term* two = output.front()->term;
                    output.pop();

                    token result = {true, operate(one, two, &operators.top()->op), ""};
                    output.push(&result);

                    operators.pop();
                    top_op = (!operators.empty()) ? operators.top()->op : "";
                }

                operators.push(&tokens.front());
                // cout << "push " << tokens.front().op << endl;
            }

            tokens.pop();
        }
    }

    // Pop all of the operators and operate
    while (!operators.empty()) {
        Term* one = output.front()->term;
        output.pop();
        Term* two = output.front()->term;
        output.pop();

        token result = {true, operate(one, two, &operators.top()->op), ""};
        output.push(&result);

        operators.pop();
    }

    cout << endl;

    while (!output.empty()) {
        print_token(output.front());
        output.pop();
    }

}

int main() {
    // Get input expression from user
    cout << "Enter an expression" << endl;
    string expression;
    std::getline(cin, expression);
    cout << expression << endl;

    queue<token> tokens;

    for (int i = 1, j = 0; i < expression.length(); i++) {
        string c = expression.substr(i, 1);

        // If there is a space, move past it and increment j to compensate
        if (c == " ") {
            j++;
            continue;
        }

        if (i == expression.length() - 1) { // If at end of string, add last term
            token t = {true, new Term(std::stod(expression.substr(j, (i - j + 1))))};
            cout << "t1 " << i << " " << j << " ";
            print_token(&t);
            tokens.push(t);
        } else if (isOperator(&c)) { // If operator, add the preceding term, then add the operator
            token t;
            t.is_term = true;
            t.term = new Term(std::stod(expression.substr(j, i - j)));

            tokens.push(t);
            cout << "t2 " << i << " " << j << " ";
            print_token(&t);
            token o = {false, nullptr, c};
            tokens.push(o); // Make a copy of the string so that the memory addresses don't get all wacky
            cout << "op " << i << " " << j << " ";
            print_token(&o);

            j = i + 1;
        }
    }

    sya(tokens);

    return 0;
}
