%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements
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

Expression    : Term ExpRight
              ;

ExpRight      : '+' Term ExpRight
              | '-' Term ExpRight
              |
              ;

Term          : ExpTerm TermRight
              ;

TermRight     : '*' ExpTerm TermRight
              | '/' ExpTerm TermRight
              |
              ;

ExpTerm       : Bracket 'e' ExpTerm
              | Bracket
              ;

Bracket       : '(' Expression ')'
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
