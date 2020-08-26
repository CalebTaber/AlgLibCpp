#include <iostream>
#include <string>

struct Expression {

};

int main() {
    // Get input expression from user
    std::cout << "Enter an expression" << std::endl;
    std::string expression;
    std::getline(std::cin, expression);
    std::cout << expression << std::endl;
    return 0;
}
