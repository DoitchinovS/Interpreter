# Interpreter for Custom-C++ language
  This project is an interpreter for a customed language, which is based on C++ syntax.
  The language supports variables, arrays, condition statements, loops and labels for integer data type and simulates entering/exiting different scopes
## Supported Language Features
 ** Reserved Keywords : **
LABEL, GOTO, LET, READ, PRINT, IF, ENDIF, ELSE, WHILE, DONE

** Operators: **
=, ==, !=, < , <=, > , >=, +, *, /, -, %, &&, ||, !, ()

** Semantics of reversed keywords **
GOTO <label_name> redirects the code towards the line where <label_name> is declared.
LET is used for variable or array declaration.
READ serves as an initializer from the program console
PRINT is a command that prints a certain value (variable or literal) on the console
IF-[ELSE]-ENDIF is a standard condition statement similar to C++ if-else statement
WHILE-DONE is a loop with the C++ while loop semantics

## Syntax Examples
### Variable/Array declaration
LET x  

LET arr[5]
### Input/Output commands
READ count  

PRINT count
### Conditional statement
IF count < 0  

PRINT 4  

[ELSE  

PRINT 5  

]
ENDIF
### Loop command
WHILE count > 0  

PRINT 6  

count = count - 1  

DONE
### GOTO label
IF count < 0  

GOTO End  

ENDIF  

count = count + 1  

LABEL End
## Example Program: Find the sum of n numbers
LET count  

READ count  

IF count < 0  

GOTO end  

LET sum  

sum = 0  

WHILE count > 0  

	LET x  
	
	READ x  
	
	sum = sum + x  
	
	count = count + 1  
	
DONE  

PRINT sum  


LABEL end  


## Error Handling
- Invalid identifiers of variables/labels
- Incorrect expressions that are to be evaluated
- Duplicate indentifiers
- Assigning values to variables that are not declared/not present in the current scope
- Invalid WHILE-DONE and IF-ELSE-ENDIF syntax
- Undefined operation executions (division and modulo 0)
## Project Structure
  Interpreter.cpp, 
  ProgramInterpreter.cpp, 
  ProgramInterpreter.h, 
  ExpressionEvaluater.cpp, 
  ExpressionEvaluater.h, 
  IdentifierValidation.cpp, 
  IdentifierValidation.h, 
  ProgramLabels.cpp, 
  ProgramLabels.h, 
  ProgramVariables.cpp, 
  ProgramVariables.h, 
## Testing Files
   ExpressionEvaluationTest.cpp, 
   IdentifiersTesting.cpp, 
   LabelsTesting.cpp, 
   VariablesTest.cpp, 
## Future improvements
   - Enhancing the WHILE-DONE and IF-ELSE-ENDIF validation
   - Improve RLE version

   - Make GOTO automatically change scope when called with a label in another scope.


