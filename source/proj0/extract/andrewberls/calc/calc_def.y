%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%

Start         : Statements
              ;

Statements    : Statement Statements
              | ;

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

Assignment    : MemAccess '=' Expression
              ;

Print         : 'p' Expression
              ;


Expression    : Term EAddSub
              ;

EAddSub       : '+' Term EAddSub
              : '-' Term EAddSub
              | ;


Term          : Factor TMultDiv
              | MemAccess
              ;

TMultDiv      : '*' Factor TMultDiv
              | '/' Factor TMultDiv
              | ;

MemAccess     : 'm' '[' Expression ']'
              ;


Factor        : NumParen Exponen
              ;

NumParen      : '(' Expression ')'
              | 'n'
              ;

Exponen       : 'e' NumParen Exponen
              | ;

%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
