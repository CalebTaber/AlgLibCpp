#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <cmath>

double operate(double one, double two, char op) {
    if (op == '+') {
        return one + two;
    } else if (op == '-') {
        return one - two;
    } else if (op == '*') {
        return one * two;
    } else if (op == '/') {
        return one / two;
    } else if (op == '^') {
        return std::pow(one, two);
    }

    return -9999;
}

void sya(std::string* expression) {

    std::string operators = "+-/*";
    std::map<char, int> opsLevels;
    opsLevels.emplace('+', 2);
    opsLevels.emplace('-', 2);
    opsLevels.emplace('*', 3);
    opsLevels.emplace('/', 3);
    opsLevels.emplace('^', 4);

    std::stack<char> ops;
    std::stack<double> output; // TODO change to a string stack so operators can be pushed to it

    for (int i = 1, j = 0; i < expression->length(); i++) {
        char c = expression->at(i);

        // Make sure spaces don't get included in parsing
        if (c == ' ') {
            j++;
            continue;
        }

        if (i == expression->length() - 1) {
            // If at end of string, add last term
            double term = std::stod(expression->substr(j, i + 1));
            output.push(term);

            // At the end of the expression, pop pop
            while (!ops.empty()) {
                // The first term popped is the second operand because of reverse polish notation
                double two = output.top();
                output.pop();
                double one = output.top();
                output.pop();

                char tmp_op = ops.top();
                ops.pop();

                output.push(operate(one, two, tmp_op));
            }
        } else if (operators.find(c) != std::string::npos) {
            double term = std::stod(expression->substr(j, i));
            output.push(term);

            int opLevel = opsLevels.find(c)->second;
            if (ops.empty() || opsLevels.find(ops.top())->second < opLevel) {
                // If the operator's precedence is higher than the top of the stack, add it to the stack
                ops.push(c);
            } else {
                // If the operator's precedence is lower than the top of the stack, pop and operate until it's not
                while (!ops.empty() && opsLevels.find(ops.top())->second >= opLevel) {
                    double one = output.top();
                    output.pop();
                    double two = output.top();
                    output.pop();

                    char tmp_op = ops.top();
                    ops.pop();

                    output.push(operate(one, two, tmp_op));
                }

                ops.push(c);
            }

            j = i + 1;
        }
    }

    while (!output.empty()) {
        std::cout << output.top() << std::endl;
        output.pop();
    }
}

int main() {
    // Get input expression from user
    std::cout << "Enter an expression" << std::endl;
    std::string expression;
    std::getline(std::cin, expression);
    std::cout << expression << std::endl;

    sya(&expression);

    return 0;
}
