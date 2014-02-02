#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25

//all of the terminals in the language
typedef enum {
	T_eof = 0,			// 0: end of file
	T_num,					// 1: numbers
	T_plus,				// 2: +
	T_minus,				// 3: -
	T_times,				// 4: *
	T_divide,			// 5: /
	T_power, 			// 6: **
	T_equals,			// 7: =
	T_openparen,		// 8: (
	T_closeparen, 	// 9: )
	T_opensquare,	// 10: [
	T_closesquare,	// 11: ]
	T_m,						// 12: m
	T_goto,				// 13: goto
	T_if,					// 14: if
	T_print,				// 15: print
	T_label,				// 16: L
	T_colon				// 17: :
} token_type;

//this function returns a string for the token.  It is used in the parsetree_t
//class to actually dump the parsetree to a dot file (which can then be turned
//into a picture).  Note that the return char* is a reference to a local copy
//and it needs to be duplicated if you are a going require multiple instances
//simultaniously
char* token_to_string(token_type c) {
	static char buffer[MAX_SYMBOL_NAME_SIZE];
	switch( c ) {
		case T_eof:		strncpy(buffer,"eof",MAX_SYMBOL_NAME_SIZE); break;
		case T_num:		strncpy(buffer,"num",MAX_SYMBOL_NAME_SIZE); break;
		case T_plus:		strncpy(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
		case T_minus:	strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
		case T_times:	strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
		case T_divide:	strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
		case T_power:	strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
		case T_equals:	strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
		case T_openparen:		strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
    case T_closeparen:		strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
    case T_opensquare:		strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
    case T_closesquare:	strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
    case T_m:			strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
    case T_goto:		strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
    case T_if:			strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
    case T_print:	strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
    case T_label:	strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
    case T_colon:	strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
		default: 			strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
	}
	return buffer;
}

//all of the non-terminals in the grammar (you need to add these in
//according to your grammar.. these are used for printing the thing out)
//please follow the convention we set up so that we can tell what the heck you
//are doing when we grade
typedef enum {
	epsilon = 100,
	NT_Start,
	NT_Statements,
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Assignment,
	NT_Print,
	NT_Expression1,
	NT_Expression2,
	NT_Term1,
	NT_Term2,
	NT_Factor,
	NT_Expo
} nonterm_type;

//this function returns a string for the non-terminals.  It is used in the parsetree_t
//class to actually dump the parsetree to a dot file (which can then be turned
//into a picture).  Note that the return char* is a reference to a local copy
//and it needs to be duplicated if you are a going require multiple instances
//simultaniously. 
char* nonterm_to_string(nonterm_type nt)
{
	static char buffer[MAX_SYMBOL_NAME_SIZE];
	switch( nt ) {
		  case epsilon: strncpy(buffer,"e",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Start: strncpy(buffer,"Start",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
 		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
 		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
 		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression1: strncpy(buffer,"Expression1",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression2: strncpy(buffer,"Expression2",MAX_SYMBOL_NAME_SIZE); break;
 		  case NT_Term1: strncpy(buffer,"Term1",MAX_SYMBOL_NAME_SIZE); break;
 		  case NT_Term2: strncpy(buffer,"Term2",MAX_SYMBOL_NAME_SIZE); break;
 		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
 		  case NT_Expo: strncpy(buffer,"Expo",MAX_SYMBOL_NAME_SIZE); break;
		  default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
		}
	return buffer;
}

/*** Scanner Class ***********************************************/

class scanner_t {
  public:
	void eat_token(token_type c);
	token_type next_token();
	int get_line();
	int getNumber();
	scanner_t();

  private:
	int lineNumber, pointer, numberPointer;
	string curLine;
	char line[];

	void scan_error(char x);
	void mismatch_error(token_type c);
	void range_error();
};

token_type scanner_t::next_token()
{
	//printf( "next_token()\n" );
	while( curLine[pointer] == 10 )
	{
		lineNumber++;
		pointer++;
	}
	while( curLine[pointer] == 32 )
		pointer++;
	if( curLine[pointer] == '*' && curLine[pointer+1] == '*' )
		return T_power;
	else if( curLine[pointer] == 'g' && curLine[pointer+1] == 'o' && curLine[pointer+2] == 't' && curLine[pointer+3] == 'o' )
		return T_goto;
	else if( curLine[pointer] == 'p' && curLine[pointer+1] == 'r' && curLine[pointer+2] == 'i' && curLine[pointer+3] == 'n' && curLine[pointer+4] == 't' )
		return T_print;
	else if( curLine[pointer] == 'i' && curLine[pointer+1] == 'f' )
		return T_if;
	else
	{
		switch( curLine[pointer] )
		{
			case '\0': return T_eof;
			case '1' : return T_num;
			case '2' : return T_num;
			case '3' : return T_num;
			case '4' : return T_num;
			case '5' : return T_num;
			case '6' : return T_num;
			case '7' : return T_num;
			case '8' : return T_num;
			case '9' : return T_num;
			case '0' : return T_num;
			case '+' : return T_plus;
			case '-' : return T_minus;
			case '*' : return T_times;
			case '/' : return T_divide;
			case '=' : return T_equals;
			case '(' : return T_openparen;
			case ')' : return T_closeparen;
			case '[' : return T_opensquare;
			case ']' : return T_closesquare;
			case 'm' : return T_m;
			case 'L' : return T_label;
			case ':' : return T_colon;
			default  : scan_error(curLine[pointer]);
		}
	}
}

void scanner_t::eat_token(token_type c)
{
	while( curLine[pointer] == 10 )
	{
		lineNumber++;
		//cout << "Line:" << lineNumber << endl;
		pointer++;
	}
	while( curLine[pointer] == 32 )
		pointer++;
	//cout << "EAT: " << token_to_string(c) << ": " << curLine[pointer] << " : " << curLine[pointer] - 0 << endl;
	if( curLine[pointer] == '*' && curLine[pointer+1] == '*' )
	{
		if( c != T_power )
			mismatch_error( c );
		pointer += 2;
	}
	else if( curLine[pointer] == 'g' && curLine[pointer+1] == 'o' && curLine[pointer+2] == 't' && curLine[pointer+3] == 'o' )
	{
		if( c != T_goto )
			mismatch_error( c );
		pointer += 4;
	}
	else if( curLine[pointer] == 'p' && curLine[pointer+1] == 'r' && curLine[pointer+2] == 'i' && curLine[pointer+3] == 'n' && curLine[pointer+4] == 't' )
	{
		if( c != T_print )
			mismatch_error( c );
		pointer += 5;
	}
	else if( curLine[pointer] == 'i' && curLine[pointer+1] == 'f' )
	{
		if( c != T_if )
			mismatch_error( c );
		pointer += 2;
	}
	else if( curLine[pointer] >= 48 && curLine[pointer] <= 57 )
	{
		numberPointer = pointer;
		if( c != T_num )
			mismatch_error( c );
		while( curLine[pointer] >= 48 && curLine[pointer] <= 57 )
			pointer++;
	}
	else
	{
		switch( curLine[pointer] )
		{
	 		case '\0': if( c != T_eof ) mismatch_error( c );
 			pointer++; break;
 			case '+' : if( c != T_plus ) mismatch_error( c );
 			pointer++; break;
 			case '-' : if( c != T_minus ) mismatch_error( c );
 			pointer++; break;
 			case '*' : if( c != T_times ) mismatch_error( c );
 			pointer++; break;
 			case '/' : if( c != T_divide ) mismatch_error( c );
 			pointer++; break;
 			case '=' : if( c != T_equals ) mismatch_error( c );
 			pointer++; break;
 			case '(' : if( c != T_openparen ) mismatch_error( c );
 			pointer++; break;
 			case ')' : if( c != T_closeparen ) mismatch_error( c );
 			pointer++; break;
 			case '[' : if( c != T_opensquare ) mismatch_error( c );
 			pointer++; break;
 			case ']' : if( c != T_closesquare ) mismatch_error( c );
 			pointer++; break;
 			case 'm' : if( c != T_m ) mismatch_error( c );
 			pointer++; break;
 			case 'L' : if( c != T_label ) mismatch_error( c );
 			pointer++; break;
 			case ':' : if( c != T_colon ) mismatch_error( c );
 			pointer++; break;
 			default  : mismatch_error( c );
		}
	}
}

scanner_t::scanner_t()
{
	lineNumber = 1;
	pointer = 0;
	int i = 0;
	char tempChar;
	while( cin.get(tempChar) )
		curLine[i++] = tempChar;
	curLine[i] = '\0';
	for( int j = 1; j <= 5; j++ )
		curLine[i+j] = '\0';
	/*token_type token = next_token();
	while( token != T_eof )
	{
		eat_token(token);
		token = next_token();
		cout << token_to_string( token ) << endl;
	}*/
}

int scanner_t::get_line()
{
	return lineNumber;
}

int scanner_t::getNumber()
{
	long long temp = 0;
	while( curLine[numberPointer] >= 48 && curLine[numberPointer] <= 57 )
	{
		temp = temp * 10l + ( curLine[numberPointer] - '0' );
		if( temp > 2147483647l )
			range_error();
		numberPointer++;
	}
	return (int)temp;
}

void scanner_t::scan_error (char x)
{
	printf("scan error: unrecognized character '%c' -line %d\n",x, get_line());
	exit(1);
}

void scanner_t::mismatch_error (token_type x)
{
	printf("syntax error: found %s ",token_to_string(next_token()) );
	printf("expecting %s - line %d\n", token_to_string(x), get_line());
	exit(1);
}

void scanner_t::range_error ()
{
	printf("range error: integer overflow -line %d\n", get_line());
	exit(1);
}

/*** ParseTree Class **********************************************/

//just dumps out the tree as a dot file.  The interface is described below
//on the actual methods.  This class is full and complete.  You should not
//have to touch a thing if everything goes according to plan.  While you don't
//have to modify it, you will have to call it from your recursive decent
//parser, so read about the interface below.
class parsetree_t {
  public:
	void push(token_type t);
	void push(nonterm_type nt);
	void pop();
	void drawepsilon();
	parsetree_t();
  private:
	enum stype_t{
		TERMINAL=1,
		NONTERMINAL=0,
		UNDEF=-1
	};

	struct stuple { 
		nonterm_type nt;
		token_type t;
		stype_t stype; 
		int uniq;
	};
	void printedge(stuple temp); //prints edge from TOS->temp
	stack<stuple> stuple_stack;
	char* stuple_to_string(const stuple& s); 
	int counter;
};


//the constructer just starts by initializing a counter (used to uniquely
//name all the parse tree nodes) and by printing out the necessary dot commands
parsetree_t::parsetree_t()
{
	counter = 0;
	printf("digraph G {\n");
}

//This push function takes a non terminal and keeps it on the parsetree
//stack.  The stack keeps trace of where we are in the parse tree as
//we walk it in a depth first way.  You should call push when you start
//expanding a symbol, and call pop when you are done.  The parsetree_t
//will keep track of everything, and draw the parse tree as you go.
//This particular function should be called if you are pushing a non-terminal
void parsetree_t::push(nonterm_type nt)
{
	counter ++;
	stuple temp;
	temp.nt = nt;
	temp.stype = NONTERMINAL;
	temp.uniq = counter;
	printedge( temp );
	stuple_stack.push( temp );
}

//same as above, but for terminals
void parsetree_t::push(token_type t)
{
	counter ++;
	stuple temp;
	temp.t = t;
	temp.stype = TERMINAL;
	temp.uniq = counter;
	printedge( temp );
	stuple_stack.push( temp );
}

//when you are done parsing a symbol, pop it.  That way the parsetree_t will
//know that you are now working on a higher part of the tree.
void parsetree_t::pop()
{
	if ( !stuple_stack.empty() ) {
		stuple_stack.pop();
	}

	if ( stuple_stack.empty() ) {
		printf( "};\n" );
	}
}

//draw an epsilon on the parse tree hanging off of the top of stack
void parsetree_t::drawepsilon()
{
	push(epsilon);
	pop();
}

// this private print function is called from push.  Basically it
// just prints out the command to draw an edge from the top of the stack (TOS)
// to the new symbol that was just pushed.  If it happens to be a terminal
// then it makes it a snazzy blue color so you can see your program on the leaves 
void parsetree_t::printedge(stuple temp)
{
	if ( temp.stype == TERMINAL ) {
		printf("\t\"%s%d\" [label=\"%s\",style=filled,fillcolor=powderblue]\n",
		  stuple_to_string(temp),
		  temp.uniq,
		  stuple_to_string(temp));
	} else {
		printf("\t\"%s%d\" [label=\"%s\"]\n",
		  stuple_to_string(temp),
		  temp.uniq,
		  stuple_to_string(temp));
	}

	//no edge to print if this is the first node
	if ( !stuple_stack.empty() ) {
		//print the edge
		printf( "\t\"%s%d\" ", stuple_to_string(stuple_stack.top()), stuple_stack.top().uniq ); 
		printf( "-> \"%s%d\"\n", stuple_to_string(temp), temp.uniq );
	}
}

//just a private utility for helping with the printing of the dot stuff
char* parsetree_t::stuple_to_string(const stuple& s) 
{
	static char buffer[MAX_SYMBOL_NAME_SIZE];
	if ( s.stype == TERMINAL ) {
		snprintf( buffer, MAX_SYMBOL_NAME_SIZE, "%s", token_to_string(s.t) );
	} else if ( s.stype == NONTERMINAL ) {
		snprintf( buffer, MAX_SYMBOL_NAME_SIZE, "%s", nonterm_to_string(s.nt) );
	} else {
		assert(0);
	}

	return buffer;
}


/*** Parser Class ***********************************************/

//the parser_t class handles everything.  It has an instance of scanner_t
//so it can peek at tokens and eat them up.  It also has access to the
//parsetree_t class so it can print out the parse tree as it figures it out.
//To make the recursive decent parser work, you will have to add some
//methods to this class.  The helper functions are described below

class parser_t {
  private:
	scanner_t scanner;
	parsetree_t parsetree;

	void eat_token(token_type t);
	void syntax_error(nonterm_type);
	void Start();
	void statements();
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

  public:	
	void parse();
};


//this function not only eats the token (moving the scanner forward one
//token), it also makes sure that token is drawn in the parse tree 
//properly by calling push and pop.
void parser_t::eat_token(token_type t)
{
	parsetree.push(t);
	scanner.eat_token(t);
	parsetree.pop();
}

//call this syntax error when you are trying to parse the
//non-terminal nt, but you fail to find a token that you need
//to make progress.  You should call this as soon as you can know
//there is a syntax_error. 
void parser_t::syntax_error(nonterm_type nt)
{
	printf("syntax error: found %s in parsing %s - line %d\n",
		token_to_string( scanner.next_token()),
		nonterm_to_string(nt),
		scanner.get_line() ); 
	exit(1); 
}


//One the recursive decent parser is set up, you simply call parse()
//to parse the entire input, all of which can be dirived from the start
//symbol
void parser_t::parse()
{
	Start();
}

//WRITEME: the Start() function is not quite right.  Right now
//it is made to parse the grammar:
//       Start -> '+' Start | EOF
//which is not a very interesting language.  It has been included
//so you can see the basics of how to structure your recursive 
//decent code.

//Here is an example

void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);
	cerr << "int main()\n{\nint m[101];\n";
	statements();

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	cerr << "return 0;\n}\n";
	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here

void parser_t::statements()
{
	parsetree.push(NT_Statements);
	
	if( scanner.next_token() == T_eof )
	{
		parsetree.pop();
		return;
	}
	statement();
	statements();
	
	parsetree.pop();
}

void parser_t::statement()
{
	parsetree.push(NT_Statement);
	
	switch( scanner.next_token() )
	{
		case T_eof:
			parsetree.drawepsilon();
			break;
		case T_m:
			assignment();
			break;
		case T_label:
			label();
			break;
		case T_goto:
			jump();
			break;
		case T_print:
			print();
			break;
		default:
			syntax_error(NT_Statement);
			break;
	}
	
	parsetree.pop();
}

void parser_t::assignment()
{
	parsetree.push(NT_Assignment);
	
	eat_token(T_m);
	cerr << "m";
	eat_token(T_opensquare);
	cerr << "[";
	expression1();
	eat_token(T_closesquare);
	cerr << "]";
	eat_token(T_equals);
	cerr << "=";
	expression1();
	cerr << ";\n";
	parsetree.pop();
}

void parser_t::label()
{
	parsetree.push(NT_Label);
	
	eat_token(T_label);
	eat_token(T_num);
	cerr << "a" << scanner.getNumber() << ": ";
	eat_token(T_colon);
	
	parsetree.pop();
}

void parser_t::jump()
{
	parsetree.push(NT_Jump);
	
	eat_token(T_goto);
	eat_token(T_num);
	int temp = scanner.getNumber();
	if( scanner.next_token() == T_if )
	{
		eat_token(T_if);
		cerr << "if( ";
		expression1();
		cerr << ")\n{\ngoto a" << temp << ";\n}\n";
	}
	else
	{
		cerr << "goto a" << temp << ";\n";
	}
	
	parsetree.pop();
}

void parser_t::print()
{
	parsetree.push(NT_Print);
	
	eat_token(T_print);
	cerr << "printf(\"%d\",";
	expression1();
	cerr << ");\n";
	parsetree.pop();
}

void parser_t::expression1()
{
	parsetree.push(NT_Expression1);
	
	//cout << "exp1" << endl;
	term1();
	expression2();
	
	parsetree.pop();
}

void parser_t::expression2()
{
	parsetree.push(NT_Expression2);
	
	//cout << "exp2" << endl;
	token_type t;
	switch( scanner.next_token() )
	{
		case T_plus:
			eat_token(T_plus);
			cerr << "+";
			t = scanner.next_token();
			if( t == T_plus || t == T_minus || t == T_times || t == T_divide )
				syntax_error(NT_Expression2);
			term1();
			expression2();
			break;
		case T_minus:
			eat_token(T_minus);
			cerr << "-";
			t = scanner.next_token();
			if( t == T_plus || t == T_minus || t == T_times || t == T_divide )
				syntax_error(NT_Expression2);
			term1();
			expression2();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	
	parsetree.pop();
}

void parser_t::term1()
{
	parsetree.push(NT_Term1);
	
	//cout << "term1" << endl;
	factor();
	term2();
	
	parsetree.pop();
}

void parser_t::term2()
{
	parsetree.push(NT_Term2);
	
	token_type t;
	//cout << "term2" << endl;
	switch( scanner.next_token() )
	{
		case T_times:
			eat_token(T_times);
			cerr << "*";
			t = scanner.next_token();
			if( t == T_plus || t == T_minus || t == T_times || t == T_divide )
				syntax_error(NT_Expression2);
			factor();
			term2();
			break;
		case T_divide:
			eat_token(T_divide);
			cerr << "/";
			t = scanner.next_token();
			if( t == T_plus || t == T_minus || t == T_times || t == T_divide )
				syntax_error(NT_Expression2);
			factor();
			term2();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	
	parsetree.pop();
}

void parser_t::factor()
{
	parsetree.push(NT_Factor);
	
	//cout << "factor" << endl;
	expo();
	if( scanner.next_token() == T_power )
	{
		eat_token(T_power);
		cerr << "^";
		factor();
	}
	
	parsetree.pop();
}

void parser_t::expo()
{
	parsetree.push(NT_Expo);
	
	//cout << "expo" << endl;
	switch( scanner.next_token() )
	{
		case T_openparen:
			eat_token(T_openparen);
			cerr << "(";
			expression1();
			eat_token(T_closeparen);
			cerr << ")";
			break;
		case T_m:
			eat_token(T_m);
			cerr << "m";
			eat_token(T_opensquare);
			cerr << "[";
			expression1();
			eat_token(T_closesquare);
			cerr << "]";
			break;
		case T_num:
			eat_token(T_num);
			cerr << scanner.getNumber();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	
	parsetree.pop();
}
/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();
	//scanner_t scanner;
	//system("pause");
	return 0;
}
