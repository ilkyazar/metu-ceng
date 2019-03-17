INTRODUCTION
--------------------------------------------------------------------------------
The program calculates the probability distribution function.
If there is the chance to perform repetitive experimentation of considerable
amount, then the distribution can be determined by performing random generated
experimentation and the outcome for the function can be calculated.
It is done by ‘drawing’ values for each variable with probabilities set by
their associated probability distribution functions.
Then these values are sustituted for the variables and a calculation of the
formula that defines the function is done. The resulting value is recorded.
Repeating over and over again this task will give a histogram of the outcome
values.
Finally, the histogram is converted into a probability distribution by
normalizing it.

ABOUT THE INPUT
--------------------------------------------------------------------------------
An infix formula is given that defines the function.
The probability distribution function of it will be computed.
For each variable in the formula, an ordered set of n tuples will be given.
Each tuple is a value and the probability that value will occur.
The probabilities will be normalized (ie. they add up to unity).

Here is the BNF representation for the formula:
<formula> ::= <formula> <operator> <formula> |
              ( <formula> ) |
              <function> ( <formula> ) |
              <letter> | <unsigned number>
<operator> ::= + | - | * | / | ^
<function> ::= ~ | sin | cos | sqrt | ln
<letter> ::= A | B | . . . | Z
<unsigned number> ::= <any unsigned %lf readable number>

EXAMPLE RUN:
--------------------------------------------------------------------------------
gcc the1.c -lm -o the1
./the1 < 01.inp
>>>0.005906 199.973663 0.124916 0.250000 0.250000 0.141214 0.017488
