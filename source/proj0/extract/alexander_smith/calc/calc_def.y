%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements
              | /*epsilon*/
              ;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' Jumpagain
              ;
Jumpagain     : 'i' Expression
	      | /*epsilon*/
	      ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression     
              : '(' Expression ')' ExpressionPrime
              | 'm' '[' Expression ']' ExpressionPrime
              | 'n'ExpressionPrime
              ;
ExpressionPrime	: '+' Expression
		| '-' Expression
		| ExpressionPrimePrime
		;

ExpressionPrimePrime	: '*' ExpressionPrimePrimePrime
			| '/' Expression
			| /*epsilon*/
			;

ExpressionPrimePrimePrime	: '*' Expression
				| Expression
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
