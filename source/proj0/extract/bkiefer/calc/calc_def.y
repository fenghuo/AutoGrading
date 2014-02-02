%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;

Statements    : List
              |
              ;
              
List          : Statement Statements
              ;
              
Statement     : 'l' Label
              | 'g' 'n' Jump
              | 'm' Assignment
              | 'p' Print
              ;
              
Label         : 'n' ':'
              ;
              
Jump         : 'i' Expression
              | 
              ;
              
Assignment    : '[' Expression ']' '=' Expression
              ;
              
Print         : Expression
              ;

Expression    : Expr ExpressionP
              ;

ExpressionP   : '+' Expr ExpressionP
	      | '-' Expr ExpressionP
	      | 
	      ;

Expr          : Term ExprP
              ;

ExprP 	      : '*' Term ExprP
	      | '/' Term ExprP
	      | 
	      ;

Term          : Factor TermP
              ;

TermP	      : 'e' Term
	      | 
	      ;

Factor	      : '(' Expression ')'
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
