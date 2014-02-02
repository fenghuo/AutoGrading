Sean Sullivan
4047460

I scanned the txt files into an array then went through the array checking all the values and when a tokens criteria was met I pushed a token to a new vector. If unexpected characters or set of characters appeared I return an error. Then the parse using the grammar checks each token, eating the token when it's found and pushes the each terminal and non-terminal state to the tree. I had a bit of trouble with the error detection in the parse and wasn't confident with it enough to make the C code.
