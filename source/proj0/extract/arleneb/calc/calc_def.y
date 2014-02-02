%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%
        
Start : Statements;
Statements : Statement Statements1;
Statements1 : Statement Statements1 | /*epsilon*/;
Statement : Label | Jump | Assignment | Print;
Label : 'l' 'n' ':';
Jump : 'g' 'n' Jump1;
Jump1 : 'i' Expression | /*epsilon*/;
Assignment : 'm' '[' Expression ']' '=' Expression;
Print : 'p' Expression;
Expression : Multdiv Expression1;
Expression1 : '+' Multdiv Expression1 | '-' Multdiv Expression1 | /*epsilon*/;
Multdiv : Exp Multdiv1;
Multdiv1 : '*' Exp Multdiv1 | '/' Exp Multdiv1 | /*epsilon*/;
Exp : Par Exp1;
Exp1 : 'e' Par | /*epsilon*/;
Par : '(' Expression ')'| 'm' '[' Expression ']' | 'n';
%%

void yyerror(char *s) {
	 fprintf(stderr, "%s\n", s);
	 return;
}

int main(void) {
	 yyparse();
	 return 0;
}
