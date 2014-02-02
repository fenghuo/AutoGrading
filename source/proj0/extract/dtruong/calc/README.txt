CS160 Project 0
David Truong #5843115

Quick Guide:
Compile with "make test_parse" with a file called "test.good.calc" as the input file in the same directory. Error messages are printed to stdout and are located inside the .dot file at the very bottom of the file.  This is because the generated .dot information and the error messages are printed to the same stream (stdout).  C++ Code is outputted to the stderr which can be redirect to a file called "CCode.cpp" by calling "make generate_code". Parse tree is created as .pdf file also. For the input files, the eof must be on the same line as last line of code.  If the eof is on a line by itself, the program will generate an extra semicolon at the end of the code and then it wont compile.  

 
***************************************
The warning "warning: <stdin>: syntax error in line _ near ';'" appears when I compile the code. I believe this error is due to the way the .dot files and parsetree are created.  If you get the warning, you will notice that the line number that the warning gives corresponds to the last line of the produced .dot file where the semincolon comes after the bracket. I dont believe that this warning affects the program in any major way as my program still scans, parser, etc. I couldnt figure out what the warning was about.
***************************************

Compiling and Runnin:
Compile with "make test_parse" to create a ".dot" and ".pdf" of the parse tree.(rename the outputted file in the makefile if you wish). I edited the makefile to only handle one input file at a time and you can see my changes in the makefile that I submit with this project.  Error messages are outputted to stdout which is the .dot file.  CCode is run with "make generate_code" and the C++ code is in the specified .cpp file in the makefile.

Error Handling:
My errors will print out to the specified ".dot" file in the last line of the file. The class provided errors messages print to stdout and that is where the dot data file is also outputted to so they share the same file.  I couldnt output the error messages to stderr because that is where I am outputting my generated c++ code. If there is a mismatch/syntax error, it will print out the message and line number at the bottom of the ".dot" file and the name is specified is the makefile.

Parse Tree:
The parse tree that is created is a ".pdf".  If you edit the makefile, you can name it whatever you want.  For my makefile, I edited it so that the pdf wouldnt get truncated when the tree got too large.  The default arguments in the makefile for creating the pdf wouldnt allow the parse tree to fit fully on the page. The parse tree handles multiple statements in a given input file.  The "start" node denotes the start of a new statement and the subtrees of that node are the statement itself. The parse tree shows the non-terminal derivations of the grammar and the leaf nodes at the bottom are the terminals of the statement. The parse tree is created when calling "make test_parse". This probably goes without saying but if the tree is too big, it does do some weird stuff but that is handled by the code the professor gave us.  You should probably use an input file with fewer statements if you want to be able to read the parse tree clearly and correctly.

Scanner:
In calc.cpp, in the "main()" function, I have commented out a section of code that can be used to test that my scanner is working by itself.  Uncomment the block of code and comment out the code that tests the parser to only test the scanner.  The scanner worked for all of my test cases so if for some reason, it doesnt scan at all, please take a look at the scanner_t class. If you are testing the scanner alone, the scanner will output to standard output stream which of course shares the same location with the parse tree creation .dot file.  The outputted tokens from testing the scanner will therefore be located at the very bottom of the .dot file.

Parser and Grammar:
In calc_def.y, my grammar is defined and it is unambigous, LL(1), and handles precedence as well as the other cases defined in the language.  In calc.cpp, I implemented the grammar rules with respect to the first and follow sets of the non-terminals.  In calc.cpp, parser_t class, if the next token is not handled by the cases that can be encountered, then the token is not in the first or follow set and therefore is a syntax error which is outputted with line number and description.

Code Generation:
When calling "make test_parse", the c++ code will be outputted to console.  If you really wanted to, you could send that output to a file by calling "make generate_code".  If you do decide to redirect the code to a file, ignore the console output because that is the output from stdout and not stderr anymore so it is not relevant.  My cerr statements are in main() and throughout the scanner class. 
