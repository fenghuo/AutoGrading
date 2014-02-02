%{
        #include <stdio.h>
        int yylex(void);
        void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement Statements
              | /*empty */
              ;
              
Statement     : Label
              | Jump
              | Assignment
              | Print
              ;
              
Label         : 'l' 'n' ':' LabelStatements
              ;

LabelStatements : LabelStatement LabelStatements
              | /* empty */

LabelStatement : Assignment
              | Print
              ;
              
Jump          : 'g' 'n'
              | 'g' 'n' 'i' Expression
              ;
              
Assignment    : 'm' '[' Expression ']' '=' Expression
              ;
              
Print         : 'p' Expression
              ;
              
Expression    : T E1
              ;

E1            : '+' T E1
              | '-' T E1
              | /* empty */
              ;

T             : P T1
              ;

T1            : '*' P T1
              | '/' P T1
              | /* empty */
              ;

P             : Pa P1
              ;

P1            : 'e' Pa P1
              | /* empty */
              ;

Pa            : '(' Expression ')'
              | F
              ;

F             : 'n'
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
