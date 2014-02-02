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
              
Expression    : T E1
              ;

E1            : "+" T E1
              | "-" T E1
              |
              ;

T             : F T1
              ;

T1            : "*" F T1
              | "/" F T1
              |
              ;

F             : final F1
              ;

F1            : "e" final F1
              |
              ;

final         : "(" Expression ")"
              | "m" "[" Expression "]"
              | "n"
%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
