CS 160 Winter 2014
Project 0 - calc

Student: Shayon Javadizadeh
Perm: 5643515

WHAT I DID:
I built a scanner that I believe works completely.  I modified the given grammar so that it is not ambiguous and LL(1). I have built a parse that correctly navigates input and throws erros when it should.  My parser creates C code that can be run on CSIL*(SEE NOTES BELOW)

NOTES:
Currently, I have not been able to create the C code for exponentiation so power is not working properly, specifically it will not work if there is a quantity that you are trying to power. So 5 ** 6 works, but (5+1) ** 6 does not work.  I reccommend the grader tests my code agaisnt inputs that do not contain the ** operator so that he/she may see that my compiler will work in the case without **.  
