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
              
Jump          : 'g' 'n' JumpCheck
              ;
              
JumpCheck     : 'i' Expression
              | 
              ;

Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;

//////////
Expression    : Term Expression2
              ;

Expression2   : '+' Expression
              | '-' Expression
              | 
              ;

Term          : Power Term2
              ;

Term2         : '*' Term
              | '/' Term
              |
              ;

Power         : Factor Power2
              ;

Power2        : 'e' Power
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
