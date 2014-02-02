Arlene Budiardjono
5509914

Notes:

IMPORTANT: for some reason, "make input" won't compile my c code correctly EVEN THOUGH I include math.h at the top. using "gcc input.c -o input -lm" to SPECIFY that the math library must be linked works though.

Not sure which files I was supposed to turn in, so I just turned them all in.

Values of numbers were stored in a queue as they appeared for in-order and easy access in the parser.

I just used getchar() for the scanner and scanned for the subsequent correct inputs. It was much easier to it that way.

The c code is printed to standard err in the parser. All the functions return a string and they build upon each other to have the correct format. The Statements function prints out the Statement function to stdr.

The Non_terminal enums caused me quite a bit of trouble. It completely messed up my tree JUST by changing the string that was in strncpy(). I have NO idea why this happened. It was very frustrating and I spent hours trying to debug a problem that I would never in a million years have guessed existed.
