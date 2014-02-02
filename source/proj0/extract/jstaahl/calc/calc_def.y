%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start          : Statements 
               ;
               
Statements     : Statement StatementsTail

StatementsTail : Statement StatementsTail 
               | /*epsilon*/
               ;

Statement      : Label
               | Jump
               | Assignment
               | Print
               ;
              
Label          : 'l' 'n' ':'
               ;
              
Jump           : 'g' 'n' Jumpif
               ;

Jumpif         : 'i' Expression
               | /*epsilon*/
               ;
              
Assignment     : 'm' '[' Expression ']' '=' Expression
               ;
              
Print          : 'p' Expression
               ;
              
Expression     : Term Expression2
               ;

Expression2    : '+' Term Expression2
               | '-' Term Expression2
               | /*epsilon*/
               ;

Term           : Exponent Term2
               ;

Term2          : '*' Exponent Term2
               | '/' Exponent Term2
               | /*epsilon*/
               ;

Exponent       : Factor Exponent2
               ;

Exponent2      : 'e' Factor Exponent2
               | /*epsilon*/
               ;

Factor         : 'n'
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
