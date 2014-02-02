%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements
	      | /* epsilon */
              ;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' Jump_
              ;

Jump_	      : 'i' Expression
	      | /* epsilon */
	      ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : '(' Expression ')' Expression_
              | 'm' '[' Expression ']' Expression_
              | 'n' Expression_
              ;

Expression_   : '+' Expression
              | '-' Expression              
              | Expression__
              ;

Expression__  : '*' Expression
              | '/' Expression
              | Expression___
              ;
              
Expression___ : 'e' Expression
	      | /* epsilon */
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
