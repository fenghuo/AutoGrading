%{
        #include <stdio.h>
        int yylex(void);
        void yyerror(char *);
%}

%%
        
Start         : Statements
              ;
              
Statements    : Statement R
              ;

R             : Statement R
              |
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
              
Expression    : Expression '+' Term
              | Expression '-' Term
              | Term
              | '(' Expression ')'
              | 'm' '[' Expression ']'
              | 'n'
              ;


Term          : Term '*' Factor
              | Term '/' Factor
              | Factor
              ;

Factor        : 'n'
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
