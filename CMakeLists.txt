cmake_minimum_required(VERSION 3.17)
project(AlgLib)

set(CMAKE_CXX_STANDARD 11)

add_executable(AlgLib main.cpp Term.cpp Term.h VariableList.cpp VariableList.h Expression.cpp Expression.h Arithmetic.cpp Arithmetic.h Utils.cpp Utils.h Processing.cpp Processing.h)