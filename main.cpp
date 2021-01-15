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
    testExpression("-3*(-2)", "6");

    // Order of operations tests
    testExpression("2 * 7 / 4 - 16 + (32 / 6.5) * 9", "31.807692");
    testExpression("-23 * -17 / 44 - 106 + (-32 / 6.5) * 22", "-205.421329");
    testExpression("-23 ^ -2 / 44 * 106 / (-32 / 6.5) * 22", "-0.020351");
    testExpression("(2 * (3 + 16 / (2.5*-0.125))) + 1", "-95.4");
    testExpression("(2^3)^-2.0", "0.015625");
    testExpression("2^(1 - (3*2))", "0.03125");
    testExpression("10-3^3/9", "7");
    testExpression("7^2/(4+3)", "7");
    testExpression("7*5-(2^2)", "31");
    testExpression("(6+2^2)*10", "100");
    testExpression("3*6+8^2", "82");
    testExpression("4^3-10/5", "62");
    testExpression("3^2*2-9", "9");
    testExpression("9*3^2-8", "73");
    testExpression("6^2/3-5", "7");
    testExpression("(9-5)^2/4", "4");
    testExpression("(2+(4/2))*3^2-5", "31");
    testExpression("((-10)-((-16)/(-8))^2)*(-7)+(-7)", "91");
    testExpression("((66/11)^3-6)*(-11)+(-11)", "-2321");
    testExpression("(((-4)^3+(-10))*(-4))-(-11)+(-10)", "297");
    testExpression("((-6)+((-10)/(-5)))*(-3)^2-(-8)", "-28");
    testExpression("((84/7)^3-9)*10+10", "17200");
    testExpression("((3^3+11)*3)-9+11", "116");
    testExpression("(((-4)^2+(-10))*(-4))-(-6)+10", "-8");
    testExpression("(10-(16/8)^3)*3+3", "9");
    testExpression("8-11*(2-(2^2+11))", "151");
    testExpression("((-2)-((-3)^3-(-2)))*((-3)-(-5))", "46");
    testExpression("10-(-11)*(5-(5^3+11))", "-1431");

    // Basic Algebra
    testExpression("(11-(18x^(2)/-9x^(2))^3)*(-10x^(1))", "-190x^(1)");
    testExpression("((-11x^(1))+((-18)/(-9))*x^(1))*(-3)^3-(-7x^(1))", "250x^(1)");
    testExpression("(10x^(2)-(16x^(1)/8)^2)*5x^(-1)", "30x^(1)");
    testExpression("(5x^(1)+(9/3)*x^(1))*3^3-10x^(1)", "206x^(1)");
    testExpression("(((-8x^(1))/(-4x^(1)))^2-(-4))*(-7)*x^(1)", "-56x^(1)");
    testExpression("((15x^(1)/5x^(1))^2-9)*3x^(1)+4x^(1)", "4x^(1)");
    testExpression("(11x^(2)-(18x^(1)/9)^2)*-3x^(-1)", "-21x^(1)");
    testExpression("((5^3+7)*x^(1)-5x^(1))-3x^(1)+3x^(1)", "127x^(1)");
    testExpression("((2^2+4)*x^(1)-5x^(1))-6x^(1)", "-3x^(1)");
    testExpression("4x^(1)-8*(9x^(1)-((-2)^2+7)*x^(1))", "20x^(1)");
    testExpression("((-6)^3)*x^(1)-((-3x^(1))*((-2)-(-8)))", "-198x^(1)");
    testExpression("((-10)-((-16)*x^(2)/8x^(2))^2)*2x^(1)+(-11x^(1))", "-39x^(1)");
}

int main() {
    // testExpressions();

    // TODO need function to determine what the input is
    // i.e. Expression, Equation, Inequality, System, ...

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
