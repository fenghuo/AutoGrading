%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;

Statements    : Statement More
              ;
              
More          : Statement More
              |
              ;
               
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' JumpExp
              ;
              
JumpExp       : 'i' Expression
              |
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;

Expression    : Term AD
              ;
              
AD            : '+' Term AD
              | '-' Term AD
              |
              ;              

Term          : Pow MD
              ;

MD            : '*' Pow MD
              | '/' Pow MD
              |
              ;
              
Pow           : Fact Exponent
              ;
              
Exponent      : 'e' Fact Exponent
              |
              ;

Fact          : '(' Expression ')'
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
