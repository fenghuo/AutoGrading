Daniel Tea
CS160
project0

in main.cpp there is a DEBUG boolean. if it is turned on, all input will redirected to
an instantiated scanner and the tokens will be printed out. if it is turned off, the program
is in Parsing mode.



Scanner:
    The scanner seems to be working for most cases. However, when I began to test my parser,
I realized the I was unable to "print num" correctly for any integers that are two digits or more.


Parser:
    I had a lot of trouble with the parser, mainly because of problems modifying the grammar. I
was able to get output (and images) for certain scenarios, which I have included as .pdf files. 
I had trouble making the grammar LL(1) so I decided to try to get some working cases instead since time was running low.

Errors:
    I was unable to get to this step. The errors printed out are mainly scanner errors that
are printed out to stdout. 

Output With C:
    I did not get this far.


generally, i spent too much time reading up on Flex and Bison when i should have been working
on the scanner. i did not realize that flex and bison files did not really need to be 
modified for the final program. i had a lot of confusion in the beginning stages but it
was mainly due to not paying attention to the details in the project description. 



