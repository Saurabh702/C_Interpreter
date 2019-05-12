# C_Interpreter
Porting interpreter in python to C (Blog : https://ruslanspivak.com/lsbasi-part1/ )

This is a interpreter project. The reference project can be found in the link above.

Our aim is to port the interpreter in the above project to C from python and add some new features. Unlike python, everything in C is a
structure and there are no classes. Therefore we have had to create some custom header files of our own. The list of important files in this repository are listed below.

calc1.c     - The main code which will be updated in the master branch
calc1.bak.c - The code where changes are tested before reflecting them in the main code
str.h       - The header file containing custom structures and reference to codes of some universal functions
str_fnt.c   - The file containing the implementation of the functions referenced by the "str.h" header file.
UNIT_TESTS  - A Simple text file containing a list of unit test cases.

The compiler Library used is GCC. GCC stands for GNU C Compiler. It is a popular open source compiler for C and an alternative compiler to CLang.
command to compile is:
  gcc <filename> <additional_files> <options>
  here: gcc calc1.c str_fnt.c -o calc1 -g -F dwarf
  where: -o indicates save output to file
         -g produces debugging information
         -F indicates the format of the debugging information
          The debugging information are used only with debuggers like GDB. It allows more finer control and view of the code while debugging.

Concept and Theory:

This implementation is processing the input token by token.
Take this example: 1 + 1

Step 1: Convert into Tokens.

here 1 is one token, + is one more token, 1 is the last token.
  Token: 1, Token: +, Token: 1
the code also does checking of whether more than one token has been input or not.
  If input is a single number, The same is output again
  If input is a random character, Error is output
  If input is 2 integers seperated by space without an operation, an error is output.
  All the spaces are ignored so spaces between tokens don't matter.
 
Step 2: Parse Tokens
Once the tokens have been obtained they are parsed. the parse() function compares the current_token with a type to see if the current
token is of the same type. if it is, parse() automatically goes to the next token else it returns an error in form of parse_error() function.

in the above example, parse(1,"INTEGER") will compare if 1 is an integer or not.
parse is interfaced through the interpret() function which takes one argument, the interpreter struct variable which processes the the input expression.
parse will automatically cause the interpreter variable to proceed to + here.

Step 3: Evaluate Expressions

Once all the tokens have been generated, the expression is evaluated. Before evaluation, each token is converted into its appropriate type.
each token is stored in a variable except the operator.
the operators are compared and tokens are operated based on the input operator.

in the above example the '+' which is character is compared with + to find out if the operation to be done is addition or not.
if true then the tokens are added and the result is output.


Features:
  - Ignores spaces while tokenizing
  - Multiple operations on the same text line
  - Error without operator
  - Precedence checking (* is given more priority than +)
  - creation of custom tokens
  
To do:
  - Add support of paranthesis
  - Eliminate unneeded code
  - Document the code
  - add support of stack

Sample Outputs:

1. calc> 1+1
   2
2. calc> 1 + 1 + 3 + 4
   9
3. calc> 1*100/100 - 100 + 1
   -98


