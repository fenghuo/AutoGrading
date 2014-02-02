%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : 'l' 'n' ':'
	      | 'g' 'n' Jump
	      | 'm' '[' Expression ']' '=' Expression
	      | 'p' Expression
	      | '\n' Statements
              ;
              
              
Jump          : 'i' Expression
              ;
              
              
Expression    : Expression '+' Factor
              | Expression '-' Factor
	      | Expression 'e' Factor
              | '(' Expression ')'
	      | 'm' '[' Expression ']'
	      | Factor

Factor        : Factor '*' Term
              | Factor '/' Term
              | Term
Term          : 'n'
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
