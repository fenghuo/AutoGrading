#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stack>
#include <vector>
#include <iostream>
#include <queue>
#include <string>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25
#define MAX_NUMBER ((2L << 31) - 1)

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
	NT_Expression,
	NT_ExpressionRight,
	NT_Term,
	NT_TermRight,
	NT_ExponentialTerm,
	NT_BracketTerm
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
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_ExpressionRight: strncpy(buffer,"Right of Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_TermRight: strncpy(buffer,"Right of Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_ExponentialTerm: strncpy(buffer,"Exponential Term",MAX_SYMBOL_NAME_SIZE); break;	
		  case NT_BracketTerm: strncpy(buffer,"Bracket Term",MAX_SYMBOL_NAME_SIZE); break;	
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

	// return next number representing underlying T_num token
	string get_number();

	//constructor 
	scanner_t();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
	//error message and exit for range error
	void range_error(string number_str);

	// queue of line numbers of each token
	queue<int> line_numbers;

	int current_line_number=0;

	// queue of tokens
	queue<token_type> tokens;

	// queue of numbers underlying T_num
	queue<string> numbers;

};

token_type scanner_t::next_token()
{
	return this->tokens.empty() ? T_eof : this->tokens.front();
}

void scanner_t::eat_token(token_type c)
{
	token_type next_token = this->next_token();
	if (c != next_token) {
		// token mismatch
		this->mismatch_error(c);
	} else {
		this->tokens.pop();
		this->line_numbers.pop();
	}
}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	string line;
	while (!cin.eof()) {
		getline(cin, line);
		// DEBUG
		cout << "Read line " << line << endl;
		this->current_line_number++;
		int i = 0, length = line.length();
		while (i < length) {
			token_type token;
			// DEBUG
			cout << "line["<<i<<"]=\""<<line[i]<<"\", line[i..i+5]=\""<<line.substr(i,6)<<"\"\n";
			if ('0' <= line[i] && line[i] <= '9') {
				// number
				// Read in all digits after line[i]
				int j = i+1;
				while (j < length && '0' <= line[j] && line[j] <= '9') ++j;
				// Check whether the number is from 0 - 2**31-1
				string number_str = line.substr(i, j-i);
				// DEBUG
				cout << "i="<<i<<", j="<<j<<", str=" << number_str <<"\n";
				errno = 0;
				long int num = strtol(number_str.c_str(), NULL, 10);
				if (errno == ERANGE || num < 0 || num > MAX_NUMBER) {
					range_error(number_str);
				}
				token = T_num;
				this->numbers.push(number_str);
				i = j-1;
			}
			else if (line[i] == '+') { token = T_plus; }
			else if (line[i] == '-') { token = T_minus; }
			else if (line[i] == '*') {
				++i;
				if (i < length && line[i] == '*') token = T_power;
				else { token = T_times; --i; }
			}
			else if (line[i] == '/') { token = T_divide; }
			else if (line[i] == '=') { token = T_equals; }
			else if (line[i] == '(') { token = T_openparen; }
			else if (line[i] == ')') { token = T_closeparen; }
			else if (line[i] == '[') { token = T_opensquare; }
			else if (line[i] == ']') { token = T_closesquare; }
			else if (line[i] == 'm') { token = T_m; }
			else if (line.substr(i, 5) == "goto ") { token = T_goto; i+=3; }
			else if (line.substr(i, 3) == "if ") { token = T_if; i++; }
			else if (line.substr(i, 6) == "print ") { token = T_print; i+=4; }
			else if (line[i] == 'L') { token = T_label; }
			else if (line[i] == ':') { token = T_colon; }
			else if (line[i] != ' ' && line[i] != '\t') {
				// unknown token, report token
				// DEBUG
				cout << "Character " << line[i] << " failed to parse\n";
				this->scan_error(line[i]);
			}
			if (line[i] != ' ' && line[i] != '\t') {
				this->tokens.push(token);
				this->line_numbers.push(this->current_line_number);
				// DEBUG
				cout << "Parsed token " << token_to_string(token) << " at line " << this->current_line_number << ", i=" << i << endl;
			}
			++i;
		}
	}
	this->tokens.push(T_eof);
	this->line_numbers.push(this->current_line_number);
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
	return this->line_numbers.empty() ? this->current_line_number : this->line_numbers.front();
}

string scanner_t::get_number() {
	if (this->numbers.empty()) {
		return "-1";
	} else {
		string num = this->numbers.front();
		this->numbers.pop();
		return num;
	}
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

void scanner_t::range_error(string x) {
	printf("range error: number %s not within 0 and %ld - line %d\n", x.c_str(), MAX_NUMBER, get_line());
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

	vector<string> cerr_buffer;

	void Start();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)
	void Statements();
	void Statement();
	void Label();
	void Jump();
	void Assignment();
	void Print();
	void Expression();
	void ExpressionRight();
	void Term();
	void TermRight();
	void ExponentialTerm();
	void BracketTerm();

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

//Here is an example
void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);

	cerr_buffer.push_back("#include <stdio.h>\n");
	cerr_buffer.push_back("#include <math.h>\n");
	cerr_buffer.push_back("int main() {\n");
	cerr_buffer.push_back("int m[101] = {};\n");
	Statements();
	cerr_buffer.push_back("return 0;\n");
	cerr_buffer.push_back("}\n");
	for (vector<string>::iterator itr = cerr_buffer.begin(); itr != cerr_buffer.end(); itr++) {
		cerr << *itr;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here
void parser_t::Statements() {
	parsetree.push(NT_Statements);

	Statement();
	if (scanner.next_token() != T_eof) {
		Statements();
	} else {
		scanner.eat_token(T_eof);
		parsetree.drawepsilon();
	}

	parsetree.pop();
}

void parser_t::Statement() {
	parsetree.push(NT_Statement);

	switch (scanner.next_token()) {
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
	}

	parsetree.pop();
}

void parser_t::Label() {
	parsetree.push(NT_Label);

	scanner.eat_token(T_label);
	parsetree.push(T_label);
	parsetree.pop();
	scanner.eat_token(T_num);
	parsetree.push(T_num);
	parsetree.pop();
	scanner.eat_token(T_colon);
	parsetree.push(T_colon);
	parsetree.pop();
	cerr_buffer.push_back("L" + scanner.get_number() + ":\n");

	parsetree.pop();
}

void parser_t::Jump() {
	parsetree.push(NT_Jump);

	scanner.eat_token(T_goto);
	parsetree.push(T_goto);
	parsetree.pop();
	scanner.eat_token(T_num);
	parsetree.push(T_num);
	parsetree.pop();
	cerr_buffer.push_back("goto L" + scanner.get_number() + ";\n");
	if (scanner.next_token() == T_if) {
		scanner.eat_token(T_if);
		parsetree.push(T_if);
		parsetree.pop();
		// Insert if statement before goto
		string goto_str = cerr_buffer.back();
		cerr_buffer.pop_back();
		cerr_buffer.push_back("if (");
		Expression();
		cerr_buffer.push_back(") {\n");
		cerr_buffer.push_back(goto_str);
		cerr_buffer.push_back("}\n");
	}

	parsetree.pop();
}

void parser_t::Assignment() {
	parsetree.push(NT_Assignment);

	scanner.eat_token(T_m);
	parsetree.push(T_m);
	parsetree.pop();
	scanner.eat_token(T_opensquare);
	parsetree.push(T_opensquare);
	parsetree.pop();
	cerr_buffer.push_back("m[");
	Expression();
	scanner.eat_token(T_closesquare);
	parsetree.push(T_closesquare);
	parsetree.pop();
	scanner.eat_token(T_equals);
	parsetree.push(T_equals);
	parsetree.pop();
	cerr_buffer.push_back("]=");
	Expression();
	cerr_buffer.push_back(";\n");

	parsetree.pop();
}

void parser_t::Print() {
	parsetree.push(NT_Print);

	scanner.eat_token(T_print);
	parsetree.push(T_print);
	parsetree.pop();
	cerr_buffer.push_back("printf(\"%d\",");
	Expression();
	cerr_buffer.push_back(");\n");

	parsetree.pop();
}

void parser_t::Expression() {
	parsetree.push(NT_Expression);

	Term();
	ExpressionRight();

	parsetree.pop();
}

void parser_t::ExpressionRight() {
	parsetree.push(NT_ExpressionRight);

	switch (scanner.next_token()) {
		case T_plus:
			scanner.eat_token(T_plus);
			parsetree.push(T_plus);
			parsetree.pop();
			cerr_buffer.push_back("+");
			Term();
			ExpressionRight();
			break;
		case T_minus:
			scanner.eat_token(T_minus);
			parsetree.push(T_minus);
			parsetree.pop();
			cerr_buffer.push_back("-");
			Term();
			ExpressionRight();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}

	parsetree.pop();
}

void parser_t::Term() {
	parsetree.push(NT_Term);

	ExponentialTerm();
	TermRight();

	parsetree.pop();
}

void parser_t::TermRight() {
	parsetree.push(NT_TermRight);

	switch (scanner.next_token()) {
		case T_times:
			scanner.eat_token(T_times);
			parsetree.push(T_times);
			parsetree.pop();
			cerr_buffer.push_back("*");
			ExponentialTerm();
			TermRight();
			break;
		case T_divide:
			scanner.eat_token(T_divide);
			parsetree.push(T_divide);
			parsetree.pop();
			cerr_buffer.push_back("/");
			ExponentialTerm();
			TermRight();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}

	parsetree.pop();
}

void parser_t::ExponentialTerm() {
	parsetree.push(NT_ExponentialTerm);

	cerr_buffer.push_back("pow(");
	BracketTerm();
	if (scanner.next_token() == T_power) {
		scanner.eat_token(T_power);
		parsetree.push(T_power);
		parsetree.pop();
		cerr_buffer.push_back(",");
		ExponentialTerm();
		cerr_buffer.push_back(")");
	} else {
		for (vector<string>::reverse_iterator itr = cerr_buffer.rbegin(); itr != cerr_buffer.rend(); itr++) {
			if (*itr == "pow(") {
				cerr_buffer.erase(--(itr.base()));
				break;
			}
		}
	}

	parsetree.pop();
}

void parser_t::BracketTerm() {
	parsetree.push(NT_BracketTerm);

	switch (scanner.next_token()) {
		case T_openparen:
			scanner.eat_token(T_openparen);
			parsetree.push(T_openparen);
			parsetree.pop();
			cerr_buffer.push_back("(");
			Expression();
			scanner.eat_token(T_closeparen);
			parsetree.push(T_closeparen);
			parsetree.pop();
			cerr_buffer.push_back(")");
			break;
		case T_m:
			scanner.eat_token(T_m);
			parsetree.push(T_m);
			parsetree.pop();
			scanner.eat_token(T_opensquare);
			parsetree.push(T_opensquare);
			parsetree.pop();
			cerr_buffer.push_back("m[");
			Expression();
			scanner.eat_token(T_closesquare);
			parsetree.push(T_closesquare);
			parsetree.pop();
			cerr_buffer.push_back("]");
			break;
		case T_num:
			scanner.eat_token(T_num);
			parsetree.push(T_num);
			parsetree.pop();
			cerr_buffer.push_back(scanner.get_number());
			break;
		default:
			syntax_error(NT_BracketTerm);
	}

	parsetree.pop();
}

/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();
	// scanner_t scanner;
	return 0;
}
