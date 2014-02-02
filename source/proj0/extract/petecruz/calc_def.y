%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;

Statements    : Statement StatementP
              ;

StatementP    : Statement StatementP
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
              
Assignment    : MExp '=' Expression
              ;
              
Print         : 'p' MExp
              ;
              
Expression    : Term AddSub
              ;

AddSub        : '+' Term AddSub
              | '-' Term AddSub
              |
              ;

MultDiv       : '*' Factor MultDiv
              | '/' Factor MultDiv
              |
              ;

Term          : Factor MultDiv
              | MExp
              ;

Factor        : Parens EXP
              ;

Parens        : '(' Expression ')'
              | 'n'
              ;

MExp          : 'm' '[' Expression ']'
              ;

EXP           : 'e' Parens EXP
              |
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
