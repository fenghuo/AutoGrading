%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statement Statements
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
              
Jump          : 'g' Forward
              ;

Forward       : 'n' AnotherForward
              ;

AnotherForward: 'i' Expression
              |
              ;   


Assignment     : 'm' '[' Expression ']' '=' Expression
               ;
              
Print          : 'p' Expression
               ;

Expression      : Term Expressionha
                ;

Expressionha    : '+' Term Expressionha
                | '-' Term Expressionha
                |
                ;

Term            : Exponent Termha
                ;

Exponent        : Factor Factorha
                ;

Factorha        : 'e' Exponent
                |
                ;


Termha          :'*' Factor Termha
                | '/' Factor Termha
                |
                ;


Factor        :'(' Expression ')'
                | 'm' '[' Expression ']'
                | 'n'
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

%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
