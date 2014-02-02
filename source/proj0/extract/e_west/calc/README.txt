//===============================================
README for CS160 proj0
//===============================================
Evan West
//===============================================
//===============================================
//===============================================

I began this project be manipulating the grammar
we had been provided. The main things that nedded
modification were the Expression parsing elements,
as they did not support proper operator precedence.
To fix that, I modified the grammar by adding new 
non-terminals for each level of precedence: at the
lowest precedence level, which is the first and
highest level in the tree we have +/-, then *//,
then **, then () and m[]. Once these were created, 
it was then a simple matter of factoring and
eliminating left-recursion to make the grammar LL1.

Once I had an LL1 grammar, I began working on the
parsing element. I decided that next_token would
simply make a best guess at the next token using 
the next two characters, even if it coul easily
check the whole token. It would not eat the rest
of the characters from the stream and in the process
check the validity of the token until eat_token was
called, but would eat the intervening whitespace. 
One tricky point was that the scanner would
have to gobble up this whitespace begtween tokens without
it affecting next_token's return, but it would have
to keep track of newlines at the same time. To do
this, we simply keep track a counter of the number of 
newlines we have read. Another trick is that get_line
should return the value of the current eaten token,
not of the next token, which is where the stream
characters have actually been eaten to. TO do this, 
we keep two counters, one for current position and one
for number of newlines until next token.

I next wrote the parsing element of the program. To do
this easily, I needed the first and follow sets of each
nonterminal. Once these were calculated, actually writing 
the parser is very simple. If a transition has multiple 
options, we simply look at the start set of each of those
options and select the one that contains the next token. 
If it has the option of going to epsilon, we look
at the follow set, and if the next ctoken is in that set
we make an epsilon.

I id not have time to write the C-producing portion of the code.
