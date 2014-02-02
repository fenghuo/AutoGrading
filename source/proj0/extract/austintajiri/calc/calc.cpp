#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sstream>
#include <ctype.h>
#include <stack>
#include <queue>

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
	NT_If,
	NT_Assignment,
	NT_Print,
	NT_Additive,
	NT_AdditiveP,
	NT_Multiplicative,
	NT_MultiplicativeP,
	NT_Exponential,
	NT_ExponentialP,
	NT_Factor
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
		  case NT_If: strncpy(buffer,"If",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Additive: strncpy(buffer,"Additive",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_AdditiveP: strncpy(buffer,"AdditiveP",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Multiplicative: strncpy(buffer,"Multiplicative",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_MultiplicativeP: strncpy(buffer,"MultiplicativeP",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Exponential: strncpy(buffer,"Exponential",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_ExponentialP: strncpy(buffer,"ExponentialP",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
		  default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
		}
	return buffer;
}

/*** Scanner Class ***********************************************/

class scanner_t {
  public:

	//eats the next token and prints an error if it is not of type c
	void eat_token(token_type c);

	//peeks at the lookahead token
	token_type next_token();

	//return line number for errors
	int get_line();

	int get_current_num();

	//constructor 
	scanner_t();

	void scan();

	void print_tokens();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  

	token_type current_token;
	bool have_token;
	int current_line;
	int current_num;

	void scan_alpha(const char c);
	void scan_num(const char c);
	void scan_punct(const char c);

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

};

token_type scanner_t::next_token()
{
	if (have_token) {
		return current_token;
	} else {
		scan();
		have_token = true;
		return current_token;
	}
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
	
	if (!have_token) {
		scan();
	}

	if (current_token == c) {
		have_token = false;
	} else {
		mismatch_error(c);
	}
}

scanner_t::scanner_t()
{
	current_line = 1;
	have_token = false;
}

void scanner_t::scan()
{
	char c = getchar();
	if (c == EOF) {
		current_token = T_eof;
	} else if (isalpha(c)) {
		scan_alpha(c);
	} else if (isdigit(c)) {
		scan_num(c);
	} else if (ispunct(c)) {
		scan_punct(c);
	} else if (isspace(c)) {
		if (c == '\n') {
			++current_line;
		}
		scan();
	}

	have_token = true;
}

void scanner_t::scan_alpha(const char first) {
	char c;
	switch (first) {
		case 'p': // should scan print
			if ((c = getchar()) == 'r' &&
					(c = getchar()) == 'i' &&
					(c = getchar()) == 'n' &&
					(c = getchar()) == 't' &&
					!isalpha( c = cin.peek() ) ) {
				current_token = T_print;
			} else {
				scan_error(c);
			}

			break;
		case 'm':
			if ( !isalpha( c = cin.peek() ) ) {
				current_token = T_m;
			} else {
				scan_error(c);
			}

			break;
		case 'g':
			if ((c = getchar()) == 'o' &&
					(c = getchar()) == 't' &&
					(c = getchar()) == 'o' &&
					!isalpha( c = cin.peek() ) ) {
				current_token = T_goto;
			} else {
				scan_error(c);
			}

			break;
		case 'i':
			if ((c = getchar()) == 'f' &&
					!isalpha( c = cin.peek() ) ) {
				current_token = T_if;
			} else {
				scan_error(c);
			}
			break;
		case 'l':
		case 'L':
			if ( !isalpha( c = cin.peek() ) ) {
				current_token = T_label;
			} else {
				scan_error(c);
			}

			break;
		default:
			scan_error(first);
	}
}

void scanner_t::scan_num(const char c) {
	string num = "";
	num.append(1, c);
	while (isdigit(cin.peek())) {
		num.append(1, getchar());
	}

	current_num = atoi(num.c_str());
	current_token = T_num;
}

void scanner_t::scan_punct(const char first) {
	switch (first) {
		case '[':
			current_token = T_opensquare;
			break;
		case ']':
			current_token = T_closesquare;
			break;
		case '(':
			current_token = T_openparen;
			break;
		case ')':
			current_token = T_closeparen;
			break;
		case ':':
			current_token = T_colon;
			break;
		case '=':
			current_token = T_equals;
			break;
		case '+':
			current_token = T_plus;
			break;
		case '-':
			current_token = T_minus;
			break;
		case '*':
			if (cin.peek() == '*') {
				getchar();
				current_token = T_power;
			} else {
				current_token = T_times;
			}
			break;
		case '/':
			current_token = T_divide;
			break;
		default:
			scan_error(first);
	}
}

int scanner_t::get_line()
{
	return current_line;
}

int scanner_t::get_current_num()
{
	return current_num;
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
	void Statements();
	void Statement();
	void Label();
	void Jump();
	void If();
	void Assignment();
	void Print();
	void Additive();
	void AdditiveP();
	void Multiplicative();
	void MultiplicativeP();
	void Exponential();
	void ExponentialP();
	void Factor();

	bool saw_if;
	bool saw_expo;
	stringstream holder;
	stringstream if_holder;
	stringstream expo_holder;
  public:	
	parser_t();

	void parse();
};

parser_t::parser_t()
{
	saw_if = false;
	saw_expo = false;
}

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

	cerr << "#include <stdio.h>" << endl
	     << endl
	     << "int main(void)" << endl
	     << "{" << endl
		 << "	int m[101];" << endl
		 << endl;

	switch( scanner.next_token() ) 
	{
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			Statements();
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Start);
			break;
	}

	cerr << "}" << endl;

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Statements()
{
	parsetree.push(NT_Statements);

	switch (scanner.next_token() )
	{
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			Statement();
			Statements();
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Statements);
			break;
	}

	parsetree.pop();
}

void parser_t::Statement()
{
	parsetree.push(NT_Statement);

	switch (scanner.next_token())
	{
		case T_label:
			Label();
			break;
		case T_goto:
			Jump();
			break;
		case T_m:
			Assignment();
			break;
		case T_print:
			Print();
			break;
		default:
			syntax_error(NT_Statement);
			break;
	}

	parsetree.pop();
}

void parser_t::Label()
{
	parsetree.push(NT_Label);

	eat_token(T_label);
	eat_token(T_num);
	eat_token(T_colon);

	cerr << "L" << scanner.get_current_num() << ":" << endl;


	parsetree.pop();
}

void parser_t::Jump()
{
	parsetree.push(NT_Jump);

	eat_token(T_goto);
	eat_token(T_num);

	holder.str("");
	holder << "\tgoto L" << scanner.get_current_num();

	If();

	if (saw_if) {
		cerr << "\tif (" << if_holder.str() << ")" << endl;
		cerr << "\t{" << endl;
		cerr << "\t" << holder.str() << ";"<< endl;
		cerr << "\t}" << endl;
	} else {
		cerr << holder.str() << ";" << endl;
	}

	holder.str("");
	if_holder.str("");
	saw_if = false;

	parsetree.pop();
}

void parser_t::If()
{
	parsetree.push(NT_If);

	switch (scanner.next_token())
	{
		case T_if:
			eat_token(T_if);

			saw_if = true;
			Additive();
			break;
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_If);
			break;
	}

	parsetree.pop();
}

void parser_t::Assignment()
{
	parsetree.push(NT_Assignment);

	eat_token(T_m);
	eat_token(T_opensquare);

	cerr << "	m[ ";

	Additive();

	eat_token(T_closesquare);
	eat_token(T_equals);

	cerr << " ] = ";

	Additive();
	 
	cerr << ";" << endl;

	parsetree.pop();
}

void parser_t::Print()
{
	parsetree.push(NT_Print);

	eat_token(T_print);

	cerr << "	printf(\"\%d\\n\", ";

	Additive();

	cerr << ");" << endl;

	parsetree.pop();
}

void parser_t::Additive()
{
	parsetree.push(NT_Additive);

	Multiplicative();
	AdditiveP();

	parsetree.pop();
}

void parser_t::AdditiveP()
{
	parsetree.push(NT_AdditiveP);

	switch (scanner.next_token())
	{
		case T_plus:
			eat_token(T_plus);
			
			if (saw_if) {
				if_holder << " + ";
			} else {
				cerr << " + ";
			}

			Multiplicative();
			AdditiveP();
			break;
		case T_minus:
			eat_token(T_minus);

			if (saw_if) {
				if_holder << " - ";
			} else {
				cerr << " - ";
			}

			Multiplicative();
			AdditiveP();
			break;
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
		case T_eof:
		case T_closesquare:
		case T_closeparen:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_AdditiveP);
			break;
	}

	parsetree.pop();
}

void parser_t::Multiplicative()
{
	parsetree.push(NT_Multiplicative);

	Exponential();
	MultiplicativeP();

	parsetree.pop();
}

void parser_t::MultiplicativeP()
{
	parsetree.push(NT_MultiplicativeP);

	switch (scanner.next_token())
	{
		case T_times:
			eat_token(T_times);

			if (saw_if) {
				if_holder << " * ";
			} else {
				cerr << " * ";
			}

			Exponential();
			MultiplicativeP();
			break;
		case T_divide:
			eat_token(T_divide);

			if (saw_if) {
				if_holder << " / ";
			} else {
				cerr << " / ";
			}

			Exponential();
			MultiplicativeP();
			break;
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
		case T_eof:
		case T_closesquare:
		case T_closeparen:
		case T_plus:
		case T_minus:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_MultiplicativeP);
			break;
	}

	parsetree.pop();
}

void parser_t::Exponential()
{
	parsetree.push(NT_Exponential);

	Factor();
	ExponentialP();

	parsetree.pop();
}

void parser_t::ExponentialP()
{
	parsetree.push(NT_ExponentialP);

	switch (scanner.next_token())
	{
		case T_power:
			eat_token(T_power);

			if (saw_if) {
				if_holder << " ** ";
			} else {
				cerr << " ** ";
			}

			Factor();
			ExponentialP();
			break;
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
		case T_eof:
		case T_closesquare:
		case T_closeparen:
		case T_plus:
		case T_minus:
		case T_times:
		case T_divide:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_ExponentialP);
			break;
	}

	parsetree.pop();
}

void parser_t::Factor()
{
	parsetree.push(NT_Factor);

	switch (scanner.next_token())
	{
		case T_openparen:
			eat_token(T_openparen);

			if (saw_if) {
				if_holder << "(";
			} else {
				cerr << "(";
			}

			Additive();

			eat_token(T_closeparen);

			if (saw_if) {
				if_holder << ")";
			} else {
				cerr << ")";
			}

			break;
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);

			if (saw_if) {
				if_holder << "m[ ";
			} else {
				cerr << "m[ ";
			}
			Additive();
			eat_token(T_closesquare);

			if (saw_if) {
				if_holder << " ]";
			} else {
				cerr << " ]";
			}
			break;
		case T_num:
			eat_token(T_num);
			if (saw_if) {
				if_holder << scanner.get_current_num();
			} else {
				cerr << scanner.get_current_num();
			}
			break;
		default:
			syntax_error(NT_Factor);
	}

	parsetree.pop();
}

/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();
	return 0;
}
