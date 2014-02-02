Project 0 - Andrew Berls (andrew.berls@gmail.com)

The code closely follows the conventions laid out in the provided skeleton -
the `parser_t::parse()` procedure kicks off a series of recursive descent
procedures, which parse tokens and emit C code to the stderr stream inline.

The scanner first reads in the program in its entirety from stdin, and saves
it into a string program buffer, allowing for easier traversal. Tokenizing proceeds
one character at a time, and is generally straightforward. Any alphabetic
character triggers the `tokenize_keyword` procedure, which reads an arbitrary
string keyword and returns tokens as appropriate (T_print, etc) or fails.

The scanner maintains the last read numeric value in its `value` field. `get_value()`
must be called carefully, as it may return a stale value depending on the location
in the input stream.

The parser executes by looking one token ahead and making a decision from there.
C code is first emitted to a string line buffer, a hack implemented for
the T_power operator instead of writing a proper AST. This buffer is then flushed
to stderr as appropriate.

