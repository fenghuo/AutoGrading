%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
             
Start         : Start Statement
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
              
Expression    : Term '+' Expression
              | Term '-' Expression
              | Term
              ;

Term          : Value '*' Term
              | Value '/' Term
              | Value
              ;

Value         : 'n'
              | 'n' 'e' Value
              | '(' Expression ')'
              | 'm' '[' Expression ']'
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
