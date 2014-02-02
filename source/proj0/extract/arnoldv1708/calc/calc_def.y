%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start         : Statements
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
              
Jump          : 'g' 'n' Jump1
              ;

Jump1         : 'i' Expression
              |  
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : T Expression2
              ;
           
Expression2   : '+'  T Expression2
              | '-' T Expression2
              |
              ;

T             : F T2
              ;

T2            : '/' F T2
              | '*' F T2
              |
              ;

F             : exp F1
              ;

F1            : 'e' exp F1
              |
              ;       

exp           : 'n'
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
