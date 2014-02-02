%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements
              | ''
              ;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' If
              ;

If            : 'i' Expression
              | ''
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;

Expression    : Term ExpressionP
              ;

ExpressionP   : '+' Term ExpressionP
              | '-' Term ExpressionP
              | ''
              ;

Term          : Factor TermP
              ;

TermP         : '*' Factor TermP
              | '/' Factor TermP
              |''
              ;

Factor        : Base Exponent
              ;

Exponent      : 'e' Base Exponent
              | ''
              ;

Base          : 'n'
              | 'm' '[' Expression ']'
              | '(' Expression ')'
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
