Name: Joey Dewan
Perm: 5262332

I completed the scanner and parser. Based on some testing I believe I got them correct. I had trouble
printing out the corrent line number fof error in the parser, however I got it to work in the scanner.
When implementing the scanner, I read in every line of input at one time and created an array to keep
track of the tokens. I think if I had just done one line at a time, it would have been easier to 
keep track of the error number and also print out the correct number value to stderr. Since I could
not think of an easy way to translate a num token to the actual number, I did not implement the C
code to stderr portion.

PS. I removed this line "./calc < test.bad.calc > test.bad.dot" from the test_parse segment of the 
makefile because I had issuesgetting the pdf file.