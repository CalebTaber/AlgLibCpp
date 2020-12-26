cmake_minimum_required(VERSION 3.17)
project(AlgLib)

set(CMAKE_CXX_STANDARD 11)

add_executable(AlgLib main.cpp term/Term.cpp term/Term.h expression/Expression.cpp expression/Expression.h math/Arithmetic.cpp math/Arithmetic.h utils/Utils.cpp utils/Utils.h)