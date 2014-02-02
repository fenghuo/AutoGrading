README
Zachary Swanson, 4650115 
978-505-3105
CS160 Project 0

type "make run" to run my proect. you can change the input by substituting for test.good.calc in the makefile.

I got all parts of the project working. my makefile is changed a little bit from the provided one (the original one doesnt correctly make the pdf for me, im not sure why. but you need to use the dot -Tpdf command to generate the pdf directly and not go to the .ps file inbetween). Also be sure to link the math library (-lm) when running the generated c code on a csil machine.

typing make run will run the following commands:
	./calc < test.good.calc > parsetree.dot 2> input.c
	dot -Tpdf < parsetree.dot > parsetree.pdf
	gcc -lm ccode.c
	./a.out

so the parse tree will go into test.good.dot which will then be converted into a  PDF
and the outpude ccode from stderr will go into ccode.c, which is then compiled with gcc (you MUST link the math library with -lm) and is then run with ./a.out

running this on a *.calc input with errors in it will obviously fail, if you want to see the error then dont redirect the output into a .dot file.

IMPORTANT: when converting the .dot file to a .pdf there will be a syntax error on the final semicolon on the last line of the .dot file, BUT the pdf will still be generated perfectly correctly. this final semicolon is included in the code by parsetree code that was provided to us so i didn't want to mess with it.


