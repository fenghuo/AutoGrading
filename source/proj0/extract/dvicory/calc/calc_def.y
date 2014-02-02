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

Jump          : 'g' GotoZ
              ;

GotoZ         : 'n' NumZ
              ;

NumZ          : 'i' Expression
              |
              ;

Assignment    : 'm' '[' Expression ']' '=' Expression
              ;

Print         : 'p' Expression
              ;

Expression    : Term ExpressionP
              ;

ExpressionP   : '+' Term ExpressionP
              | '-' Term ExpressionP
              |
              ;

Term          : Factor TermP
              ;

TermP         : '*' Power TermP
              | '/' Power TermP
              |
              ;

Factor        : Power PowerP
              ;

PowerP        : 'e' Factor
              |
              ;

Power         : '(' Expression ')'
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
