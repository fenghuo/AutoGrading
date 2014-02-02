%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statement Statements
              ;
              
Statements    : Statements Statement
              | 
              ;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' Goto
              ;
              
Goto          : 'n' Num
              ;

Num           : 'i' Expression
              |
              ;

Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : Term ExpressionP
              ;

ExpressionP   : '+' Term ExpressionP
              | '-' Term ExpressionP
              |
              ;

Term          : Factor TermP
              ;

TermP         : '*' Pow TermP
              | '/' Pow TermP
              |
              ;

Factor        : Pow PowP
              ;

PowP          : 'e' Factor 
              |
              ;


Pow           : '(' Expression ')'
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
