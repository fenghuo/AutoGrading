%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    :  Statement Derivestate
              ;
Derivestate   : Statement Derivestate
              |
              ;
               
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' Secondjump
              ;
Secondjump    : 'i' Expression
              |
              ;          
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : T Expression1
              ; 
Expression1   : '+' T Expression1
              | '-' T Expression1
              | 
              ;
T             : F Expression2
              ;
Expression2   : '*' F Expression2
              | '/' F Expression2
              |
              ;
F             : Q F1
              ;
F1            : 'e' Q F1
              |
              ;
Q             : '(' Expression ')'
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
