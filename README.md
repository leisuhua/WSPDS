# WSPDS
Reachability_tree_algorithm.cpp implements the reachability-tree based method for checking the termination of a WSPDS.

Post*-automata_algorithm.cpp implements the post*-automata based method for checking the termination of a WSPDS.

WSPDS_Generator.cpp implements a random generator of test cases with states and stack symbols both vectors. 

>>Note that by setting the dimension1(dimension of vectors of state) and dimension2(dimension of vectors of stack symbols) to different numbers in WSPDS_Generator.cpp, we can get different kinds of instances of WSPDS.

The generator works this way:
User firstly set the dimensions of vectors of states and stack symbols.

Then the generator randomly generate the number of rules of an instance.

Then for each rule, randomly generate its tag, which indicates what kind of rule it is:

>> 0 represents internal rule: u,v -> u + b, v + c

>> 1 represents pop rule: u, v -> u + b, \epsilon

>> 2 represents nonstardard pop rule: u, v1 v2 -> u + c, v1 + v2 + d

>> 3 represents push rule: u, v -> u + b, (v + d) (v + f)

>> 4 represents simple-push rule: u, \epsilon -> u + b, a

Then generate a random rule of this tag:

>> b, c, d, f are random vectors of integer numbers with each dimension from [-10, 10].

Initial configuration is generated like (p, a b).

>> p, a, b are random vectors of natural numbers with each dimension from [0, 15].

The left symbol is the top element of the stack.
