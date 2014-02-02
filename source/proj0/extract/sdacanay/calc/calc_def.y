%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statements Statement
              | Statement
              ;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n'
              | 'g' 'n' 'i' Expression
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
		    
Parentheses   : '(' Exponent ')'
		    ;
		    
Exponent	    : Expression 'e' Expression
		    | Expression
		    ;
              
Expression    : Expression '+' Expression
              | Expression '-' Expression
              | Term
		    ;

Term		    : Expression '*' Expression
              | Expression '/' Expression
		    | Terminal
		    ;
		   
Terminal	    : 'm' '[' Expression ']'
              | 'n'
              ;
%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
