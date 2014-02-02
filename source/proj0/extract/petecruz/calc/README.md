Pete Cruz

Scanner:
    The scanner first reads the file and puts each string into a map<int, string> (with the function scan_input()) where the int is the current
    line that the string was on in the file. This makes it easier to grab the line number in which some error may have occurred.

    Once we have this data, we begin scanning to produce the tokens with tokenize_input(). So a vector of tokens is created (a Token type that
    I defined). vector<Token>

Parser:
    I build an AST where the base class is AST. Each production in my grammar has its own class. It subclasses the AST class
    so that they can be returned through recursion. Each subclass has its own emit() that will emit the C code that is inherited from the base
    class.

    Made sure to add the nonterminals to nonterm_type and add the cases to the switch statement of the function of nonterm_to_string().

    Next we define functions inside the parser_t class that correspond to the class productions except these are preceded by an "underscore".
    This is where we do the push() and pop() (for terminals and nonterminals) of the parsetree so that it can be built.

    When calling parse(), it in turn calls _Start() which will call the top level production's emit() so that we can generate the C code.
    Since each production has its own emit(), they will get called up the stack once we call the emit() function from the Start class.
