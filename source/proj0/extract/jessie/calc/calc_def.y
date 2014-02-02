%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;

Statements    : Statement StatementsP
              ; 
 
StatementsP   : Statement StatementsP//
              |
              ;

Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' To
	      ;

To            : 'i' Expression
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

Term	      : Factor TermP
              ;

TermP	      : '*' Factor TermP
              | '/' Factor TermP
              |
              ;

Factor	      : Thing FactorP
              ;

FactorP	      : 'e' Thing FactorP//
              | 
              ;

Thing	      : '(' Expression ')'
              | 'm' '[' Expression ']' //
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
