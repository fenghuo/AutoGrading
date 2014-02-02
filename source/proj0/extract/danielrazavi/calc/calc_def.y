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

/*
Statements    : Statement Statements1
              ;

Statements1   : Statement
              | "epsilon"
              ;
*/          
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
/*
Jump          : 'g' 'n'
              | 'g' 'n' 'i' Expression
              ;
  */

Jump          : 'g' 'n' Jump1
              ;

Jump1         : 'i' Expression
              | "epsilon"
              ;
            
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
/*              
Expression    : Expression '+' Expression
              | Expression '-' Expression
              | Expression '*' Expression
              | Expression '/' Expression
              | Expression 'e' Expression
              | '(' Expression ')'
              | 'm' '[' Expression ']'
              | 'n'
              ;
*/

Expression    : Term Expression1
              | Paren
              ;

Expression1   : '+' Term Expression1
              | '-' Term Expression1
              | "epsilon"
              ;

Term          : Expo Term1
              ;

Term1         : '*' Expo Term1
              | '/' Expo Term1
              | "epsilon" 
              ;

Paren         :'(' Expression ')'
              ;
/*
Expo          : Paren Expo1
              ;

Expo1         : 'e' Paren Expo1
              | "epsilon"
              ;

Paren         : Factor Paren1
              ;

Paren1        : '(' Expression ')'
              | "epsilon"
              ;
*/

Expo          : Factor Expo1
              ;

Expo1         : 'e' Factor Expo1
              | "epsilon"
              ;

Factor        : 'm' '[' Expression ']'
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
