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
              
Jump          : 'g' 'n' JumpP
              ;

JumpP         : 'i' Expression
              |
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : Product ExpressionP
              ;

ExpressionP   : '+' Product ExpressionP
              | '-' Product ExpressionP
              |
              ;
              
Product       : Term ProductP
              ;

ProductP      : '*' Term ProductP
              | '/' Term ProductP
              |
              ;

Term          : Factor TermP
              ;
              
TermP         : 'e' Factor TermP
              |
              ;
              
Factor        : '(' Expression ')'
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
