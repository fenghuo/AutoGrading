%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start           : Statements
                ;
                
Statements      : Statement Statements
                |  
                ;
                
Statement       : Label
                | Jump
                | Assignment
                | Print
                ;
                
Label           : 'l' 'n' ':'
                ;
                
Jump            : 'g' 'n' If
                ;

If              : 'i' Additive
			    |  
                ;
                
Assignment      : 'm' '[' Additive ']' '=' Additive
                ;
                
Print           : 'p' Additive
                ;
                
Additive        : Multiplicative AdditiveP

AdditiveP       : '+' Multiplicative AdditiveP
			    | '-' Multiplicative AdditiveP
                |  
                ;

Multiplicative  : Exponential MultiplicativeP
                ;

MultiplicativeP : '*' Exponential MultiplicativeP
				| '/' Exponential MultiplicativeP
                |  
                ;

Exponential     : Factor ExponentialP
                ;

ExponentialP    : 'e' Factor ExponentialP
				|  
                ;

Factor          : '(' Additive ')'
                | 'm' '[' Additive ']'
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
