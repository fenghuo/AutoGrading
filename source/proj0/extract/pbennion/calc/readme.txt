Peter Bennion, 4173217

Project 0

This cross-compiler works fully for all test cases I attempted.
The C code will be output to "output.c". No error messages will be
printed to this file - all errors will still be directed to stderr.

Error messages report the line of the token that threw the error.
Therefore, a parenthesis that is not closed will usually throw an
error on the following line. I chose this method because it mirrors
how gcc reports errors for non-terminated lines.

These instructions:
m[0] = (1+1
print m[0]
will print an error on line 2. This is intentional.

I handled all scanning with a stateless function. Numeric chars are
sequenced into an int, and non-numeric chars are organized by a switch
statement. Tokens, token indices for line breaks, and numbers are
stored to a set of queues so they can be called up when needed. Line
numbers are advanced as the appropriate token gets popped off the
queue. Numbers are read and popped from their queue when the parser
encounters a number token.

The parser uses the grammar found in the new 'calc_def.y' file, though
I was unable to get yak to report any useful information. As
specified, the grammar preserves mathematical order of operations
and is LL(1). The structure of the grammar made it easy to
automatically detect and report syntax errors when the input has
a statement that isn't mathematically possible, like two operators
next to each other (ie +*)

Memory management for the C program was a little tricky. An address
storage variable 'addr' gets assigned inside the square brackets of
a memory operation to avoid seg faults. Spaces were added to the C
program after every token to ensure none will concatenate to each
other. Parentheses were added to exponent statements to enforce
proper order of operations.

Conversion from pd -> pdf does not work, though the pd file that dot
constructs appears to be correct.

