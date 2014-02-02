%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement StateImouto
              ;
			  
StateImouto	: /* epsilon */
			| Statement Statements
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' JumpImouto
              ;
			  
JumpImouto	: 'i' Expression
			|
			;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : Furu ExpressionImouto
              ;
			
ExpressionImouto	: '+' Furu ExpressionImouto
					| '-' Furu ExpressionImouto
					|
					;
			  
Furu 		  : Exp FuruImouto
	          ;
			  
FuruImouto		: '*' Exp FuruImouto
				| '/' Exp FuruImouto
				|
				;
			  
Exp			: Numend ExpImouto
			;
			
ExpImouto	: 'e' Numend
			|
			;
			  
Numend		: '(' Expression ')'
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
