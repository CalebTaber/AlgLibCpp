#ifndef ALGLIB_VARIABLELIST_H
#define ALGLIB_VARIABLELIST_H

#include <map>

#define map std::map

class VariableList {
private:
    map<char, double> variables;

public:
    VariableList(map<char, double> variables);
    ~VariableList();
    bool equals(VariableList list1, VariableList list2);
    VariableList addVariables(VariableList list1, VariableList list2);

};


#endif //ALGLIB_VARIABLELIST_H
