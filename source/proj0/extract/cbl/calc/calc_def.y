%{
        #include <stdio.h>
        int yylex(void);
        void yyerror(char *);
%}



%%

        
Start         : Statements
              ;
              
Statements    : Statement StatementsP
              ;

StatementsP   : Statement StatementsP
              | epsilon
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
              
Expression    : Term ExpressionP
              ;

ExpressionP   : '+' Term ExpressionP
              | '-' Term ExpressionP
              | epsilon
              ;

Term          : Factor TermP
              ;

TermP         : '*' Factor TermP
              | '/' Factor TermP
              | epsilon
              ;

Factor        : Alpha FactorP
              ;

FactorP       : 'e' Alpha FactorP
              | epsilon
              ;

Alpha         : '(' Expression ')'
              | 'm' '[' Expression ']'
              | 'n'
              ;

epsilon       :
%%

void yyerror(char *s) {
        fprintf(stderr, "%s\n", s);
        return;
}

int main(void) {
        yyparse();
        return 0;
}
