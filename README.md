# C_Interpreter
Porting interpreter in python to C (Blog : https://ruslanspivak.com/lsbasi-part1/ )

This is an interpreter project. The reference project can be found in the link above.
<p align = "justify">
Our aim is to port the interpreter in the above project to C from python and add some new features. Unlike python, everything in C is a
structure and there are no classes. Therefore, we had to create some custom header files of our own. The list of important files in this repository are listed below.</p>

* calc1.c     - The main code which will be updated in the master branch
* str.h       - The header file containing custom structures and reference to codes of some universal functions
* str_fnt.c   - The file containing the implementation of the functions referenced by the "str.h" header file.
* UNIT_TESTS  - A Simple text file containing a list of unit test cases.
* makefile    - Builds the interpreter

<p align = "justify">
The compiler Library used is GCC. GCC stands for GNU C Compiler. It is a popular open source compiler for C and an alternative compiler to CLang.
</p>
The command used to compile is:

 ```
 gcc <filename> <additional_files> <options>
 ```

The command used to compile the interpreter is:

 ```
 gcc calc1.c str_fnt.c -o calc1 -g -F dwarf
 ```

where:<br>
&nbsp;&nbsp;&nbsp;&nbsp;-o indicates save output to file <br>
&nbsp;&nbsp;&nbsp;&nbsp;-g produces debugging information <br>
&nbsp;&nbsp;&nbsp;&nbsp;-F indicates the format of the debugging information <br>
&nbsp;&nbsp;&nbsp;&nbsp;The debugging information is used only with debuggers like GDB. It allows more finer control and view of the code while debugging.

## Concept and Theory

This implementation is processing the input token by token.<br><br>
Take this example: 1 + 1

Step 1: Convert into Tokens

Here 1 is one token, + is one more token, 1 is the last token.<br>
&nbsp;&nbsp;&nbsp;&nbsp;Token: 1, Token: +, Token: 1

The code also does checks whether more than one token is present in the input.
* If input is a single number, the output is same as the input

* If input is a random character, the output is parse error

* If input is 2 integers separated by space without an operation, the output is parse error

* All the spaces are ignored so spaces between tokens don't matter.

Step 2: Parse Tokens
<p align = "justify">
Once the tokens have been obtained, they are parsed. The parse() function compares the current_token with a type to see if the current
token is of the same type. If it is, parse() automatically goes to the next token else it returns an error in form of parse_error() function.
</p>
<p align = "justify">
In the above example, parse(1,"INTEGER") will compare if 1 is an integer or not.
Parse is interfaced through the interpret() function which takes one argument, the interpreter struct variable which processes the input expression. Parse will automatically cause the interpreter variable to proceed to the next token which is '+' here using the get_next_token().
</p>
Step 3: Evaluate Expressions
<p align = "justify">
Once all the tokens have been generated, the expression is evaluated. Before evaluation, each token is converted into its appropriate type. Each token is stored in a variable except the operator. The operators are compared and tokens are operated based on the input operator.
</p>
<p align = "justify">
In the above example, the '+' token which is a character is compared with + to find out if the operation to be done is addition or not. If true then the tokens(operands) are added and the result is the output.
</p>

## Features
  - Ignores spaces while tokenizing
  - Multiple operations on the same text line
  - Checking errors in input
  - Precedence checking ( * is given more priority than + )
  - Creation of custom tokens

## To do
  - Add support of parenthesis
  - Add support of stack

## Sample Outputs

1. calc> 1+1<br>
   2
2. calc> 1 + 1 + 3 + 4<br>
   9
3. calc> 1*100/100 - 100 + 1<br>
   -98

__NOTE :__ For more info about this project, click [this](https://github.com/Saurabh702/C_Interpreter/blob/master/Documentation.pdf)
