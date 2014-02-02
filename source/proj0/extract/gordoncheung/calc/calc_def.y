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
              
Expression    : T Exp
              ;

Exp           : '+' T Exp 
              | '-' T Exp 
              | 
              ;

T             : M Tn
              ;

Tn            : '*' M Tn
              | '/' M Tn
              | 
              ;

M             : P Mn
              ;

Mn            : 'e' P Mn
              | 
              ;

P             : '(' Expression ')'
              | 'm' '[' Expression ']'
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
