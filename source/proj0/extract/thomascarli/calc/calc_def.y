%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statement
              ;
              
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n'
              | 'g' 'n' 'i' Expr
              ;
              
Assignment    : 'm' '[' Expr ']' '=' Expr
              ;
              
Print         : 'p' Expr
              ;

Expr         : Term_1 Expr_1

              
Expr_1        :  /*empty*/
              | '+' Expr 
              | '-' Expr

Term_1        : Factor Term_3
              
Term_2        : /* empty */
              | '*' Term_1
              | '/' Term_1

Term_3        : /* empty */
              | 'e' Term_2 

             
Factor        :
              | '(' Expr ')'
              | 'm' '[' Expr ']'
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
