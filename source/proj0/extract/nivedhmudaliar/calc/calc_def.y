%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement S 
              ;
S             : Statement S 
              |;  //epsilon
              

              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' A
              ;
A             : 'i' Expression
              |;       //epsilon
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : Term C
              ;
C             : '+' Expression
              | '-' Expression
              |;       //epsilon

Term          : Factor D
              ;
D             : '*' Term
              | '/' Term 
              |;     //epsilon
              ;

Factor        : Val F
              ;
F             : 'e' Factor
              |; //epsilon

Val           : '(' Expression ')'
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
