Author: Iris-Eleni Moridis

Code Buddy: Arnold Villatoro

CS160, Winter 2014

- The scanner takes all of the input and stores it, then calls "tokenize()" which uses multiple helper functions in order to tokenize all elements, as well as determines incorrect input. This was done using vectors: tokens are saved to the tokens vector and numbers are saved to the m vector.

- The parser follows the grammar and draws the tree according to the grammar. If there is a problem, it will throw an error. From the parser, we push to cGen which allows us to generate the C code.

- C Code is all made using the parser: elements are pushed as their tokens are eaten in the parser.



Important things and problems:

- each call to print will print on a new line when running C program

- when PDF file is made, a Warning is called for the end of the .dot file. This is due to the "};" that was included in our code, but I wasn't sure if we were allowed to change it. Despite this Warning, the PDF file is created and works.

-a difficulty on this project was making exponents to compile in GCC. This is a problem because if, for example, we have m[1]=9**3, my code eats each token sequentially. This means that, by the time the T_power token was called, we already had printed the number 9 to the C code. This was a problem because C needs it to be formatted like "pow(9,3)". 

