%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements
	      |
	      ;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' Jump1
	      ;

Jump1         : 'i' Expression
              |
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : Term Expression1
	      ;
Expression1   :'+' Term Expression1
              |'-' Term Expression1
	      |
              ;

Term	      : Factor Term1
	      ;

Term1 	      : '*' Factor Term1
	      | '/' Factor Term1
	      |
	      ;

Factor        : Base Factor1
	      ;
Factor1	      : 'e' Base Factor1
              |
	      ;

Base	      : '(' Expression ')'
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
