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
              
Jump          : 'g' 'n' Jump2
              ;

Jump2		: 'i' Expression
		| 
		;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;


Expression    : Term Expression2
		;

Expression2	: '+' Term Expression2
		| '-' Term Expression2
		|
		;


Term		: Next1 Term2
		;

Term2		: '*' Next1 Term2
		| '/' Next1 Term2
		| 
		;

Next1		: Next2 Next12
		;

Next12		: 'e' Next2 Next12
		| 
		;

Next2		: '(' Expression ')'
		| Next3
		;

Next3		: 'm' '[' Expression ']'
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
