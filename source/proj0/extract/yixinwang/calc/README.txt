Author: Yixin Wang
Jan/22/2014
(code buddy : Ricky Martinez)
Scanner:
  This is the most difficult part for me i think. It is always hard to get started. I did not use additional method in scanner class. Just next_token() and eat_token(c). In next_token(), I just peek the next character and compare with the Terminal in our language. If a terminal has more than one character, I used getchar() and cin.putback() to scan next character I need to check without destroy the input. In eat_tokn()method, If the next token is valid, then I use getchar()as many times as I needed to "eat" the certain terminal. 
  I tested my scanner before I move on, so I believe it fully works.

Modify grammer:
 First of all, the grammer provided is ambiguous (Statements and Expression). So, I break up single Statements rule to two rules. As for Expression. I did the exact same way as the professor introduced in the lecture so that all the power and "()"are derived on the left,then "*" and "/" and then "+" and "-".  Then check if it is LL1 by bison.

List first sets and follow sets

Parser:
 First add every non-terminal I modified in the parser method.
I think parser is pretty easy once I figured out first sets and follow sets. 
For every non-terminal, I just follow the grammar rule to call each corresponding non-terminal method. For each non-terminal method, there are three possibilities. First: If the first set of the non-terminal contains terminal, then, I check if the next-token is that specific terminal ,if it is, then I eat the terminal and move on. if not, throw an error. Second: If the next_token is something in the follow set of the specific non-terminal, then I call parsetree.drawepsilon() to wait for later matching. Third: If the next_token is neither in the first set nor in the follow set, then it's a error.
I tested the test.good.calc and it successfully build the parsetree.pdf. So, I think my parser should also fully work.

Error: 
  I did all I could find to throw errors while I am not a hundred percent sure my error handling is all good.  
 
C code generation:
  I can not figure out this part. I am poor at reading input from file or writing output to file. Seems it is not trivial to generate C code. So, I can not make this part totally work.
