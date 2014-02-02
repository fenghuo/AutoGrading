Name: Jessie Lin
6832166

Scanner:
scanner_t contains private variables line_num and curr_tok which keep track of the current line number and the current token respectively.
next_token reads in from file the token and matches it up with one of the terminals
eat_token calls next_token first to set the line number and current token, it then checks to see if the token matches its argument token. if not it gives a mismatch error.
scanner_t constructor sets the first line num to 1 and curr_tok to null

Nontermials are made into nonterm_type and contain to_string functions

Parser_t:
parse calls Start() which then calls statments() and continues through the rest of the grammar. At each nontermal that the program goes to the name is pushed into the parsetree at the beginning of the function then popped off at the end. this sets the tree up so that the last function called is furthest down on the tree (implemented first).

