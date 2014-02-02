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
              
Jump          : 'g' 'n' Jump_p
              ;

Jump_p        : 'i' Expression
              | epsilon
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : Expression Expression_p
              | '(' Expression ')'
              | 'm' '[' Expression ']'
              | 'n'
              ;

Expression_p  : '+' Expression_p
              | '-' Expression_p
              | '*' Expression_p
              | '/' Expression_p
              | 'e' Expression_p
              | epsilon
              ;

epsilon       : ;
%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
