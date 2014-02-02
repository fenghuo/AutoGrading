%{
	 #include <stdio.h>
  #include <stdlib.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
   Start         : Statement Statements 
              ;
              
Statements    : Statement Statements
              | 
              ;
              
Statement     : 'l' 'n' ':'
              | 'g' 'n' Jump
              | 'm' '[' Expression ']' '=' Expression
              | 'p' Expression
              ;
                          
Jump          : 'i' Expression
              |
              ;
                            
Expression    : Term ExpressionD
              ;

ExpressionD   : '+' Term ExpressionD
              | '-' Term ExpressionD
              |
              ;

Term          : Factor TermD
              ;

TermD         : '*' Factor TermD
              | '/' Factor TermD
              | 'e' Factor TermD
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
