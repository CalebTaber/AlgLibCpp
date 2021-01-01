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

string formatDouble(double d, int maxPrecision) {
    string formatted;

    string toString = std::to_string(d);
    int decimal = toString.find('.');

    if (maxPrecision + decimal > toString.length()) formatted = toString;
    else formatted = toString.substr(0, decimal + maxPrecision + 1); // Add 1 so that maxPrecision starts on the right side of the decimal point

    // Shed unnecessary trailing zeroes
    for (int i = (int) formatted.length() - 1; i >= 0; i--) {
        if (formatted.at(i) != '0') break;
        else formatted = formatted.substr(0, formatted.length() - 1);
    }

    // If all zeroes following the decimal were superfluous, remove the decimal as well
    if (formatted.at(formatted.length() - 1) == '.') formatted = formatted.substr(0, formatted.length() - 1);

    return formatted;
}
