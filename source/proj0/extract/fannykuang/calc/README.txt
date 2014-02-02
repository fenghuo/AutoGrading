Grammar took a little while to figure out.  

Starting the project was also kind of hard.  Filling in code was pretty daunting since it was difficult to check progress step by step due to the amount of helper code already written.  I got going after a bit of nudging from my friends.

Translating grammar to code was immensely time consuming and also confusing.  Code was really messy and volatile until my friend pointed out that I should use eat_tokens() sparingly.  Revamped a lot of the initial code to make it cleaner and consistent. 

Ran into some glitchy scanner errors while doing the parsing part.  Had to go back to scanner and fix the next_token() to get the correct tokens for the tokens that were more than one character long. Did a lot of bug fixing (scanner problems and recursion structures in translating grammer->code) and confused myself for a long time when I thought 'e' represented T_power.

Ran into a make *** [test_parse] Error 1.  Unable to figure out how to fuly fix it. 
dot -Tpdf < testing.good.dot > parsetree.pdf 
generates a fully visable and functional parse tree.  However, the [test_parse] make command does not generate a good .ps file and subsequently .pdf.  Skipped over trying to resolve this bug since it didn't seem detrimental to project in general and got to work on outputting a .c file.

In generating the .c code, I borrowed a friend's suggestion to make use of cerr.  That helped out a ton.  Even though I anticpated this part to be the least intensive section, there are a few trips that required tweaking.  Firstly the goto if statement needed the if() to come before the goto.  Initially I had just printed out everything in the eat_token function of the scanner class.  This didn't work when I needed to make specific adjustments such as the goto.  So I tweaked it a bit and isntead kept track of some characters and appended them to a string to use in cerr AFTER the if() section.  I think, given a little bit more tedious-ness and finesse, I'd probably end up taking out most/all of the cerr statements in the eat_token function and put them around the parse functions to make specific cases and get maximum flexibility.

I'd like to use a late day. I underestimated the project and overestimated my ability.  Hopefully this will be a lesson to puzzle out the project long before touching any of the code.