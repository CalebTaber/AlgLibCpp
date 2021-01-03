#ifndef ALGLIB_UTILS_H
#define ALGLIB_UTILS_H

#include <string>

const std::string g_OPERATORS = "+*/^()";

bool atEndOfString(int i, const std::string* s) ;

bool isOperator(char c) ;

bool isOperator(const std::string *s) ;

std::string formatDouble(double d, int maxPrecision) ;

#endif //ALGLIB_UTILS_H
