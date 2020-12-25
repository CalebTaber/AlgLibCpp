#ifndef ALGLIB_PROCESSING_H
#define ALGLIB_PROCESSING_H

#include <queue>
#include <string>
#include <vector>
#include <map>
#include <stack>

const std::map<char, int> g_precedence = {
        {'+', 1},
        {'-', 1},
        {'*', 2},
        {'/', 2},
        {'^', 3},
        {'(', 4},
        {')', 4}
};

std::queue<std::string> infixToPostfix(std::vector<std::string> *tokens);

std::vector<std::string> tokenizeExpression(const std::string *expression);

std::string preprocess(const std::string* input);

#endif //ALGLIB_PROCESSING_H
