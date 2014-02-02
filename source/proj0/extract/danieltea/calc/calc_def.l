%{
    #include <stdlib.h>
    void yyerror(char *);
    #include "y.tab.h"

    // for printing
    #include<stdio.h>
%}

%%

[\+\-\*\/]      { 
	                //printf("operator\n");
	                return *yytext; }
[\(\)\[\]]      { 
	                //printf("bracket or parenthesis\n");
	                return *yytext; }
[m=:]           { 
	                //printf("m or = or :\n");
	                return *yytext; }
"**"            { return 'e';     }

"goto"          { return 'g';     }
"if"            { return 'i';     }
"print"         { 
	                //printf("print\n");
                    return 'p';     }
[lL]            { 
	                //printf("label\n");
	                return 'l';     }

[0-9][0-9]*     { 
	                //printf("a number\n");
	                return 'n';     }

[ \t\n]         ; /* skip whitespace */

.               yyerror("invalid character");

%%

int yywrap(void) {
	return 1;
}
