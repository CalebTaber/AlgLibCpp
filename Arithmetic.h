#ifndef ALGLIB_ARITHMETIC_H
#define ALGLIB_ARITHMETIC_H

#include <string>
#include <map>

#include "Term.h"

bool canAdd(Term* one, Term* two) ;

bool operable(Term* one, Term* two, const std::string* op) ;

std::map<char, double> operateOnVarsMaps(Term *one, Term *two, bool add) ;

Term *operate(Term *one, Term *two, const std::string *op) ;

Term* add(Term* one, Term* two) ;

Term* subtract(Term* one, Term* two) ;

Term* multiply(Term* one, Term* two) ;

Term* divide(Term* one, Term* two) ;

Term* pow(Term* one, Term* two) ;

#endif //ALGLIB_ARITHMETIC_H
