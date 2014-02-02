%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statements Statement
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
              
Expression    : T  Expression2
              ;
                
Expression2   : '+' T Expression2
              | '-' T Expression2
              |
              ;

T             : F T2
              ;

T2            : '/' F
              | '*' F
              |
              ;

F             : b F2
              ;

F2            : 'e' b F2
              |
              ;

b             : 'n'
              | '(' Expression ')'
              | 'm' '[' Expression ']' 
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
