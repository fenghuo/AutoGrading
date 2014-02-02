%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Stmt Start
              | Stmt
              ;
              
              
Stmt     : Label
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
              
Expr          : Term Expr_prime
              ;

Expr_prime    : '+' Term Expr_prime
              | '-' Term Expr_prime
              |
              ;

Term          : Power Term_prime
              ;              

Term_prime    : '*' Power Term_prime
              | '/' Power Term_prime
              |
              ;

Power         : Factor Power_prime
              ;              

Power_prime   : 'e' Factor Power_prime
              |
              ;

Factor        : '(' Expr ')'
              | 'm' '[' Expr ']'
              | 'n'            

%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
