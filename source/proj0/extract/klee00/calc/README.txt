Grammar
Non terminals included in the grammar:
NT_Start
NT_Statements
NT_Statement
NT_Label
NT_Jump
NT_Assignment
NT_Print
NT_Expression1
NT_Expression2
NT_Term1
NT_Term2
NT_Factor
NT_Expo

And the grammar of the program is as follows:

Start			: Stats;
Statements	: Stat Stats | Stat ;
Statement		: Label | Jump | Assignment | Print ;
Label		: 'l' 'n' ':' ;
Jump		: 'g' 'n' | 'g' 'n' 'i' Expression1;
Assignment	: 'm' '[' Exp1 ']' '=' Expression1;
Print			: 'p' Expression1;
Expression1	: Term1 Expression2 ;
Expression2	: '+' Term1 Exp2 | '-' Term1 Expression2 | ;
Term1		: Factor Term2 ;
Term2		: '*' Factor Term2 | '/' Factor Term2 | ;
Factor		: Expo 'e' Factor | Expo ;
Expo		: '(' Expression1 ')' | 'm' '[' Expression1 ']' | 'n' ;

***************************************************************************

Scanner
In the program, the functions that I have added or modified in the class Scanner are:
void eat_token(token_type c): This is the function to ¡§eat¡¨ the next token. I use if-else and switch structures to check if the token_type is equal to the type of token that the function is going to ¡§eat¡¨. If not, error message will be prompted. New lines and spaces will be ignored.
token_type next_token(): This is the function to check the next token. I use if-else and switch structures to check the type of the token and return. New lines and spaces will be ignored.
int get_line(): Return the line number which is already stored in the class. Line number will increase when a ¡§\n¡¨ is read from input.
int getNumber(): Return the integer value of the current token, which must be a ¡§T_num¡¨.
scanner_t(): Constructor which read all input and store in one single string. Pointer is used to point to the next character
void range_error(): Output message for range error.


***************************************************************************

Parser
In the program, the functions that I have added or modified in the class Parser are:
void Start();
void statements(): 
void statement();
void label();
void jump();
void assignment();
void print();
void expression1();
void expression2();
void term1();
void term2();
void factor();
void expo();
All above functions follow the rules in the grammar and will generate C code at the same time.
