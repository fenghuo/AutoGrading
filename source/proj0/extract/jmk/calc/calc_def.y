%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement MoreStatement
              ;

MoreStatement : Statement MoreStatement
              | epsilon
              
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
              
Expression    : Term ExpressionP
              ;

ExpressionP   : '+' Term ExpressionP
              | '-' Term ExpressionP
              | epsilon
              ;

Term          : Factor TermP
              ;

TermP         : '*' Factor TermP
              | '/' Factor TermP
              | epsilon
              ;

Factor        : Exp FactorP

FactorP       : 'e' Exp FactorP
              | epsilon

Exp           : '(' Expression ')'
              | 'm' '[' Expression ']'
              | 'n'
              ;

epsilon       :

%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
