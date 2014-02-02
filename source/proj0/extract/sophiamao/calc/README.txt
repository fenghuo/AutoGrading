************README for PROJECT 0**************

assignment: winter 2014 CS160 PROJ0
Author: Sophia Mao
PERM: 5193826

Analysis of Scanner:
The scanner is the first step of reading in the input file *.calc. I used the character by character approach to scanning by using nested if statements to scan through the chain of characters such as "print" and "goto". The scanner provided does error detection and outputs the correct tokens.


***Grammar to unambiguous:

Start -> Statements

Statements -> Statements Statement
	  | Statement

Statement -> Label
	  | Jump
	  | Assignment
	  | Print

Label -> "L" num ";"

Jump -> "goto" num
     	   | "goto" num "if" Expression

Assignment -> "m" "[" Factor "]" "=" Expression

Print -> "print" Expression

Expression -> Expression "+" Term
	   | Expression "-" Term

Term -> Term "*" Power
     	   | Term "/" Power

Power ->  Term "**" Factor
      	   | Factor

Factor -> "(" Expression ")"
       	   | "m" "[" Expression "]"
           | num






**** to LL(1)
Start -> Statements

Statements -> Statements Statement
	  | Statement

Statement -> Label
	  | Jump
	  | Assignment
	  | Print

Label -> "L" num ";"

Jump -> "goto" num
     	   | "goto" num "if" Expression

Assignment1 -> "m" "[" Factor Assignment2

Assignment2 -> "]" "=" Expression

Print -> "print" Expression

Expression -> Term Expression2

Expression2 -> "+" Term Expression2
	    | "-" Term Expression2

Term -> Power Term2

Term2 -> "*" Factor Term2
      	   | "/" Factor Term2

Power -> Term Power1
      | Factor

Power1 -> "**" Factor Power1

Factor -> "("Expression")"
       	  | "m" "[" Expression "]"
       	  | num


Analysis of Parser:
In the process of implementing the parser. Ran into ambiguity and precedence problems in the grammar before implementing parser and ran out of time. 
