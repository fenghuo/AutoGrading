%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statement
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

Expression    : Term Expression2

              
Expression2   :  /*empty*/
              | '+' Expression 
              | '-' Expression

Term          : Factor Term3
              
Term2         : /* empty */
              | '*' Term
              | '/' Term

Term3         : /* empty */
              | 'e' Term2 

             
Factor        :
              | '(' Expression ')'
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
