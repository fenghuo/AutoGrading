%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : S1 B1
              ;
              
S1            : Label
              | Jump
              | Assignment
              | Print
              ;

B1            : Label B1
	      | Jump 
              | Assignment B1
	      | Print B1
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
              
Expression    : E1 A1
              ;

E1            : E2 A2
              ;

E2            : E3 A3
              ;

E3            : 'm' '[' Expression ']'
	      | E4
              ;
E4            : '(' Expression ')'
	      | 'n'
              ;

A1            : '+' E1 A1
	      | '-' E1 A1
              | /* epsilon */
              ;

A2            : '*' E2 A2
	      | '/' E2 A2
              | /* epsilon */
              ;

A3            : 't' E3 A3
              | /* epsilon */
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
