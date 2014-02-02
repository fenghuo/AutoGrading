%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Label Statement
              | Jump Statement
              | Assignment Statement
              | Print Statement
              ;
              
Statement     : Label Statement
              | Jump Statement
              | Assignment Statement
              | Print Statement
              |
              ;

              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' Jump_prime
              ;

Jump_prime    :'i' Exp
              |
              ;
              
Assignment    : 'm' '[' Exp ']' '=' Exp
              ;
              
Print         : 'p' Exp
              ;
              
Exp           : Term Exp_prime
              ;


Exp_prime     : '+' Term Exp_prime
              | '-' Term Exp_prime
              | 
              ;

Term          : Factor Term_prime
              ;

Term_prime    : '*' Factor Term_prime
              | '/' Factor Term_prime
              | 
              ;

Factor        : B Factor_prime
              ;

Factor_prime  : 'e' B Factor_prime
              |
              ;

B             : '(' Exp ')'
              | 'm' '[' Exp ']'
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
