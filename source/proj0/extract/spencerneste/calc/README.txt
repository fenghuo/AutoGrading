Spencer Neste 4828026
Code Buddy: Thomas Carli 4692877

The scanner and parser should work without problems. One problem that I ran into was the power token. Reading in multiple characters for one token was a little tricky but I ultimately got it to work. One useful function was c.unget() to replace the token after I used c.get(). This ifxed many of the problems i had with scanning. 

Parsing caused many problems along the lines of seeing a token in a variable method when that token was not expected. After revising my grammar to reflect an unambiguous and LL1 grammar, i was able to successfully parse the test files. I successfully implemented precedence of operations. Bison was useful in determing if my grammar was unambiguous. This step was probably the most time consuming. After modifying the grammar, I added numerous methods to the parser_t class to take care of the Non Terminals. 

The errors thrown should reflect the correct line number onwhich they occurred. 

C code generation was a difficult step that I was not able to successfully implement. I was able to print some of the header files and the return value. 

To run this program:
make clean
make all
./calc < test.good.calc 