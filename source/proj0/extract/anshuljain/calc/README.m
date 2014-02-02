Anshul Jain - 5716543

The first thing I did was play with the grammar and redefine it such that it handled Precedence correctly and then made sure it was LL(1).
I checked the grammar's accuracy by playing with the Bison file given and the grammar I went with in the parser is represented by the bison file (calc_def.y).


Next was to implement the scanner. I tried a few different ways of implementing the scanner.

First I used the C++ based '<<' and strings, but found that that caused a segmentation fault with the Parser.
Next I tried implementing reading the entire file in character by character at the beginning, 
but I keeping track of the line numbers became sufficiently complicated that I 
decided to try reading line by line.

In reading line by line, the first way I tried was trying to only read in a line as I needed it, 
but I found that line lengths would vary based on whether the line was the last line of the file
or was followed by subsequent lines. So it became more effective to read everything in at the beginning
in the scanner constructor and add a new line character between lines. Thus, as I ate tokens, I 
checked for a newline character and would increment the line_number as needed.

Next, I implemented the parser, which was a pretty trivial task after coming up with the grammar.
I used switch cases for Nonterminals with a lot of possible rules, and for cases with only one or two,
I used an if statement for the sake of simplicity. I might want to switch to a switch case for consistency
though, as well as to allow for changing the grammar.

TODO: Implement C-code

While Implementing the C-code:
	Had to write a function to get string value of token (really only needed for number tokens, 
	but this was the cleanest way to write it)
	Also had to modify the grammar such that each Label had LabelStatements
	(which can only be Assignments and Prints) so that I can put that in a function.
	Then I ultimately chose to emit the c code to the error steam after parsing. By doing this, I could add functions to the top part of the file (defined as c_code_pre) and add the rest of the code to the main function, defined as c_code_main. I treated Label calls as functions in c-code, which meant I needed a way to differentiate between the two implementations. So I defined a string "current" that would hold the current piece of code that needed to be manipulated by Expressions.
	The next issue I faced was how to handle the power function. I realized there are 3 possibilities for the preceding statement when the power token is scanned: a number, a parenthetical expression, or another power statement. So I decided that whenever a token is hit, I would check the code backwards to see what is the preceding statement, and then put that inside the beginning of a power statement. 
       Then I realized that c has a goto and a label functionality built in, so I simplified my  code and stopped creating functions for labels and just labeled them #:.
