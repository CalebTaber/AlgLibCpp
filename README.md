# AlgebraLibraryCpp
An advanced math library, written in C++

## Goal
The goal of this project is to create a well-written and understandable library that is capable of performing complex algebraic and trigonometric calculations. Basically, I hope to implement functionality for all math up to, but not including, calculus.

## Current Status (As of 12/25/2020)
I have just finished a major reorganization of my code to position myself for more complex feature implementations.  
The library can perform basic arithmetic with these operators: +, -, \*, /, (, ), ^  
It also has limited functionality with variables and basic algebra. The finer details are in the process of being worked out.

## General Organization
With this project, I'm taking an object-oriented approach to math. The most basic element is a `Term`. Then comes the `Expression`, which is a vector of `Term` objects. Then would come the `Equation` and `System` (of equations) units, which are successive compositions of their predecessors. This structure, I believe, is the most efficient for implementing more and more complex mathematical concepts.  

As for organizing the actual mathematical operations, I currently (as of 12/25/2020) am planning to put each level of math in its own file. For example, right now basic arithmetic functions are contained in the file math/Arithmetic.cpp. In doing this, I am trying to apply the Agile methodology of separating data structures from functions that manipulate them. So a `Term` would have a coefficient and a map of variables and their exponents, but it would not have the ability, for example, to perform addition on itself and another `Term`. An outside function from Arithmetic.cpp would need to be called on both `Term` objects, returning a new `Term` that contains the sum.  

This separation of data from their manipulating functions, I have found, allows for a smoother implementation of new mathematical units and is just generally more logical.
