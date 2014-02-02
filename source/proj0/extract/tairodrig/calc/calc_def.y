%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : S1 A0
              ;
              
S1            : Label
              | Jump
              | Assignment
              | Print
              ;

A0            : S1 A0
              | 
              ;
              
Label         : 'l' 'n' ':'
              ;
              
Jump          : 'g' 'n' I
              ;

I             : 'i' Expression
              | /* epsilon */
              ;
               
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : E1 A1
              ;

A1            : '+' E1 A1
              | '-' E1 A1
              |
              ;

E1            : E2 A2
              ;

A2            : '*' E2 A2
              | '/' E2 A2
              |
              ;

E2            : E3 A3
              ;

A3            : 'e' E3 A3
              |
              ;

E3            : 'm' '[' Expression ']'
              | '(' Expression ')'
              | E4
              ;

E4            : 'n'
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
