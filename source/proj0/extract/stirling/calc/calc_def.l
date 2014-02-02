%{
    #include <stdlib.h>
    void yyerror(char *);
    #include "y.tab.h"
%}

%%

[\+\-\*\/]      { return *yytext; }
[\(\)\[\]]      { return *yytext; }
[m=:]           { return *yytext; }
"**"            { return 'e';     }

"goto"          { return 'g';     }
"if"            { return 'i';     }
"print"         { return 'p';     }
[lL]            { return 'l';     }

[0-9][0-9]*     { return 'n';     }

[ \t\n]         ; /* skip whitespace */

.               yyerror("invalid character");

%%

int yywrap(void) {
	return 1;
}
