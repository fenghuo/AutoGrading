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
              
Expression    : Expression '+' Expression
              | Expression '-' Expression
              | Expression '*' Expression
              | Expression '/' Expression
              | Expression 'e' Expression
              | '(' Expression ')'
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
