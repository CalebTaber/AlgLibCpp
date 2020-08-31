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

token* new_token(const bool is_term, Term* term, string op) {
    token* t = (token*) malloc(sizeof(token));
    t->is_term = is_term;
    t->term = term;
    if (!op.empty()) t->op = op;

    return t;
}

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
    // TODO fix 6+9-12/8 returns 6.75, but the correct answer is 13.5
    std::map<string, int> precedence;
    precedence["+"] = 2;
    precedence["-"] = 2;
    precedence["*"] = 3;
    precedence["/"] = 3;
    precedence["^"] = 4;

    stack<token*> output;
    stack<token*> operators;

    while (!tokens.empty()) {
        if (tokens.front().is_term) {
            output.push(&tokens.front()); // If term, add to queue
            cout << "push out " << tokens.front().term->getValue() << endl;
            tokens.pop();
        } else {
            string op = tokens.front().op;
            string top_op = (!operators.empty()) ? operators.top()->op : "";

            // If stack is empty or the top operator has lower precedence, push the op
            if (operators.empty() || precedence[top_op] < precedence[op]) {
                cout << "push op " << tokens.front().op << endl;
                operators.push(&tokens.front());
            } else {
                // Pop operators until an equal or lower precedence operator is encountered
                print_tokens(output);
                print_tokens(operators);
                while (precedence[top_op] >= precedence[op]) {
                    Term* two = output.top()->term;
                    cout << "pop out " << two->getValue() << endl;
                    output.pop();
                    Term* one = output.top()->term;
                    cout << "pop out " << one->getValue() << endl;
                    output.pop();

                    token* result = new_token(true, operate(one, two, &operators.top()->op), "");
                    output.push(result);
                    cout << "push out " << result->term->getValue() << endl;

                    cout << "pop op " << operators.top()->op << endl;
                    operators.pop();
                    top_op = (!operators.empty()) ? operators.top()->op : "";
                }

                operators.push(&tokens.front());
                cout << "push op " << operators.top()->op << endl;
            }

            tokens.pop();
        }
    }

    // Pop all of the operators and operate
    while (!operators.empty()) {
        Term* two = output.top()->term;
        cout << "pop out2 " << two->getValue() << endl;
        output.pop();
        Term* one = output.top()->term;
        cout << "pop out2 " << one->getValue() << endl;
        output.pop();

        string empty = "";
        token* result = new_token(true, operate(one, two, &operators.top()->op), "");
        cout << "push out2 " << result->term->getValue() << endl;
        output.push(result);

        cout << "pop op2 " << operators.top()->op << endl;
        operators.pop();
    }

    cout << endl;

    while (!output.empty()) {
        print_token(output.top());
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
