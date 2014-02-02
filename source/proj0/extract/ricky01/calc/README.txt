Ricky Martinez

for scanner i used implemented a scan() function
this will read in chars until a token can be mode, or an error will be sent.
in this function i use peek as a lookahead to do things like know when a number has ended 

was unable two finish overflow problem. mostly finished but ran out of time;

goto and power gave me trouble since the ordering goto in the input files was 
backwards. i added some functions two help with solve the problem. same with power

i made a peeked bool variable. i will save a what scan returns and use peeked to determin if i have read from the stream or not

parse tree was untouched

parser I made a a function for each nonterminal. applying my grammer rules approprietly.


program writes c code with cerr throught the code; should run unless an overflow occures. 
power nesting was not thouroughly tested example. 4e9e7
no mention was made for precision for division when printing, the section slides
 only used%d so i used that



i am able two compile my code with the lines

./calc < test.good.calc > parsetree.dot 2> input.c
dot -Tpdf < parsetree.dot > parsetree.pdf

I would use this over the given makefile.

all code was created on geddit text editor,
problems with indentation might arise with different editors

program will sucessfully parse given test.good file

also i changed the test file and make files, but it should compile with the two lines that provided earlier

i
