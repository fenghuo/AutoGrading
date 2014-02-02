%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements2
              ;
              
Statements2   : Statement Statements2
              |
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
              ;

Expression2   : '+' Term Expression2
              | '-' Term Expression2
              |
              ;

Term          : Factor Term2
              ;

Term2         : '*' Factor Term2
              | '/' Factor Term2
              | 
              ;

Factor        : Paren Factor2
              ;

Factor2       : 'e' Paren Factor2
              |
              ;

Paren         : '(' Expression ')'
              | Mem
              ;

Mem           : 'm' '[' Expression ']'
              | Number
              ;

Number        : 'n'
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
