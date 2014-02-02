%{
	 #include <stdio.h>
	 int yylex(void);
	 void yyerror(char *);
%}

%%

Start	: Stats
	;

Stats	: Stat Stats
	| Stat
	;

Stat	: Label
	| Jump
	| Assign
	| Print
	;

Label	: 'l' 'n' ':'
	;

Jump	: 'g' 'n'
	| 'g' 'n' 'i' Exp1
	;

Assign	: 'm' '[' Exp1 ']' '=' Exp1
	;

Print	: 'p' Exp1
	;

Exp1	: Term1 Exp2
	;

Exp2	: '+' Term1 Exp2
	| '-' Term1 Exp2
	|
	;

Term1	: Factor Term2
	;

Term2	: '*' Factor Term2
	| '/' Factor Term2
	|
	;

Factor	: Expo 'e' Factor
	| Expo
	;

Expo	: '(' Exp1 ')'
	| 'm' '[' Exp1 ']'
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
