#include <iostream>
#include <string>

#include "expression/Expression.h"

#define string std::string

#define cout std::cout
#define cin std::cin
#define endl std::endl

int main() {
    cout << "Enter an expression" << endl;
    string expression;
    std::getline(cin, expression);
    cout << expression << endl;

    // Evaluate and get result
    auto result = new Expression(expression);

    result->print();
    delete result;

    return 0;
}
