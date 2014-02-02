%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements
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
              
Expression    : Term RExpression
              ;

RExpression   : '+' Expression
              | '-' Expression
              |
              ;



Power         : 'e' Factor
              |
              ;

Factor        : '(' Expression ')' Power
              | 'm' '[' Expression ']' Power
              | 'n' Power
              ;

Term          : Factor RTerm
              ;

RTerm         : '*' Term
              | '/' Term
              |
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
