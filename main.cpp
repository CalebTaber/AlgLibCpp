#include <iostream>
#include <string>

#include "expression/Expression.h"

#define string std::string

#define cout std::cout
#define cin std::cin
#define endl std::endl

void testExpression(string input, string expectedResult) {
    auto e = new Expression(input);

    if (e->toString() != expectedResult) cout << "FAILURE ON INPUT: " << input << "\nEXPECTED: " << expectedResult << "\nACTUAL: " << e->toString() << "\n" << endl;
    else cout << "PASSED: " << input << " = " << expectedResult << "\n" << endl;

    delete e;
}

void testExpressions() {
    // Basic Arithmetic
    testExpression("3+2", "5");
    testExpression("12-94", "-82");
    testExpression("19*4.5", "85.5");
    testExpression("25/7", "3.571429");
    testExpression("4^5", "1024");

    testExpression("2 * 7 / 4 - 16 + (32 / 6.5) * 9", "31.807692");
    testExpression("-23 * -17 / 44 - 106 + (-32 / 6.5) * 22", "-205.421329");
    testExpression("-23 ^ -2 / 44 * 106 / (-32 / 6.5) * 22", "-0.020351");
    testExpression("(2 * (3 + 16 / (2.5*-0.125))) + 1", "-95.4");
}

int main() {
    testExpressions();

    /*
    cout << "Enter an expression" << endl;
    string expression;
    std::getline(cin, expression);
    cout << expression << endl;

    // Evaluate and get result
    auto result = new Expression(expression);

    result->print();
    delete result;
    */

    return 0;
}
