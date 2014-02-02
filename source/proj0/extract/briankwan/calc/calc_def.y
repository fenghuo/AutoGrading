%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statements Statement
              | Statement
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
              
Expression    : A B
			  ;
			  
B             : '+' A B
			  | '-' A B
			  | 
              ;

A			  : C D
			  ;

D			  : '*' C D
			  | '/' C D
			  | 
			  ;

C			  : F G
			  ;

G 			  : 'e' F G
			  | 
			  ;
			  
			  
F			  : 'n'
			  | '(' Expression ')'
			  | 'm' '[' Expression ']'
			  

%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
