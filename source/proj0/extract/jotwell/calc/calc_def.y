%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Sp
              ;

Sp			: Statement Sp
			| 
			;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' Jp
              ;

Jp			: 'i' E
			| 
			;
              
Assignment    : 'm' '[' E ']' '=' E
              ;
              
Print         : 'p' E
              ;
              
E			: M Ep
			;

Ep			: '+' M Ep
			| '-' M Ep
			| 
			;

M			: X Mp
			;

Mp			: '*' X Mp
			| '/' X Mp
			| 
			;

X			: P Xp
			;

Xp			: 'e' P Xp
			| 
			;

P			: '(' E ')'
			| id
			;
              
id			: 'm' '[' E ']'
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
