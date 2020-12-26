#include "Utils.h"

#define string std::string

bool isOperator(const char c) {
    return g_OPERATORS.find(c) != string::npos;
}

bool isOperator(const string *s) {
    return isOperator(s->at(0));
}

bool atEndOfString(int i, const string* s) {
    return (i == s->length() - 1);
}
