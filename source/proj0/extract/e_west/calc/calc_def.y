%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement S_
              ;

S_            : Statements
              | /*empty*/
              ;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' J_
              ;

J_            : 'i' Expression
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : Term E_
              ;

E_            : '+' Term Expression
              | '-' Term Expression
              | /*empty*/
              ;

Term          : Factor T_
              ;

T_            : '*' Factor T_
              | '/' Factor T_
              | /*empty*/
              ; 

Factor        : Base Exponent
              | '(' Expression ')'
              ;

Exponent      : '*' Base Exponent
              | /*empty*/
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
