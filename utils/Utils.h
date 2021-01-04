#ifndef ALGLIB_UTILS_H
#define ALGLIB_UTILS_H

#include <string>
#include <map>

const std::map<char, int> g_opsPrecedence = {
        {'+', 1},
        {'*', 2},
        {'/', 2},
        {'^', 3},
        {'(', 4},
        {')', 4}
};

bool endOfString(int i, const std::string* s) ;

bool isOperator(char c) ;

bool isOperator(const std::string *s) ;

std::string formatDouble(double d, int maxPrecision) ;

std::string removeSpaces(const std::string* s) ;


#endif //ALGLIB_UTILS_H
