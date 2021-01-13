#ifndef ALGLIB_UTILS_H
#define ALGLIB_UTILS_H

#include <string>
#include <map>

int opPrecedence(char op) ;

bool endOfString(int i, const std::string* s) ;

bool isOperator(char c) ;

bool isOperator(const std::string *s) ;

std::string formatDouble(double d, int maxPrecision) ;

std::string removeSpaces(const std::string* s) ;


#endif //ALGLIB_UTILS_H
