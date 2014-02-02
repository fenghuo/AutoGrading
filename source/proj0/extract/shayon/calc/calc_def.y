%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements
              |
              ;

              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' Jumpprime
              ;
              
Jumpprime     : 'i' Expression
              | 

Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : Term Expressionp
              ;

Expressionp   : '+' Term Expressionp
              | '-' Term Expressionp
              | 
              ;

Term          : Power Termp
              ;

Termp         : '*' Power Termp
              | '/' Power Termp
              | 
              ;

Power         : Factor Powerp
              ;

Powerp        : 'e' Factor Powerp
              |
              ;

Factor        : 'n'
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
