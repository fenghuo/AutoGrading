//Bryce Filler
//5039938

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>

using namespace std;

char char_array[1000];
int size, sizeT, setSize, linecount;
/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/
bool lexicalError = false;
#define MAX_SYMBOL_NAME_SIZE 25

//all of the terminals in the language
typedef enum {
	T_eof = 0,			// 0: end of file
	T_num = 1,					// 1: numbers
	T_plus = 2,				// 2: +
	T_minus = 3,				// 3: -
	T_times = 4,				// 4: *
	T_divide = 5,			// 5: /
	T_power = 6, 			// 6: **
	T_equals = 7,			// 7: =
	T_openparen = 8,		// 8: (
	T_closeparen = 9, 	// 9: )
	T_opensquare = 10,	// 10: [
	T_closesquare = 11,	// 11: ]
	T_m = 12,						// 12: m
	T_goto = 13,				// 13: goto
	T_if = 14,					// 14: if
	T_print = 15,				// 15: print
	T_label = 16,				// 16: L
	T_colon = 17,				// 17: :	
	T_NL = 20,
	T_epsilon = 21,
	T_Bad = 22
} token_type;

token_type token_array[1000];
token_type set[1000];

//this function returns a string for the token.  It is used in the parsetree_t
//class to actually dump the parsetree to a dot file (which can then be turned
//into a picture).  Note that the return char* is a reference to a local copy
//and it needs to be duplicated if you are a going require multiple instances
//simultaniously
char* token_to_string(token_type c) {
	static char buffer[MAX_SYMBOL_NAME_SIZE];
	switch( c ) {
	case T_NL:		strncpy(buffer,"NL",MAX_SYMBOL_NAME_SIZE); break;
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
    case T_Bad:         strncpy(buffer,"BadToken",MAX_SYMBOL_NAME_SIZE); break;

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
	NT_Start = 101,
	NT_Expression = 102,
	NT_Statements = 103,
	NT_Factor = 104,
	NT_Term = 105,
	NT_Jump = 106,
	NT_Power = 107,
	NT_NL = 108
	//WRITEME: add symbolic names for you non-terminals here
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
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statements: strncpy(buffer, "Statements",MAX_SYMBOL_NAME_SIZE);break;
		  case NT_Jump: strncpy(buffer, "Jump",MAX_SYMBOL_NAME_SIZE);break;
		  case NT_Term: strncpy(buffer, "Term",MAX_SYMBOL_NAME_SIZE);break;
		  case NT_Factor: strncpy(buffer, "Factor",MAX_SYMBOL_NAME_SIZE);break;
		  case NT_Power: strncpy(buffer, "Power",MAX_SYMBOL_NAME_SIZE);break;
		  case NT_NL: strncpy(buffer, "NL",MAX_SYMBOL_NAME_SIZE);break;

		  //WRITEME: add the other nonterminals you need here		
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

	//constructor 
	scanner_t();

	bool create_tokens(int size, char array[]);

	char char_array1[1000];
  private:
  
	int next;
	
	
	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  
	
	bool match(token_type c);

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

	

};


bool scanner_t::create_tokens(int size, char array[]){
	bool errors = false;
	int i = 0;
	for (int k = 0; k < size - 1; k++){
		//cout << array[k] << endl;
		if (array[k] == ' '){
			//cout << " " << endl;
			continue;
			}
		else if (array[k] == '\n'){
			//cout << "newline" << endl;
			token_array[i] = T_NL;
			i++;
			}
			
		else if (array[k] == '0' || array[k] == '1' || array[k] == '2' || array[k] == '3' 
				|| array[k] == '4' || array[k] == '5' || array[k] == '6' || array[k] == '7' 
				|| array[k] == '8' || array[k] == '9'){
			if (array[k+1] == '0' || array[k+1] == '1' || array[k+1] == '2' || array[k+1] == '3' 
				|| array[k+1] == '4' || array[k+1] == '5' || array[k+1] == '6' || array[k+1] == '7' 
				|| array[k+1] == '8' || array[k+1] == '9'){
			//cout << "num" << endl;
			continue;
			}
			else{
			//cout << "num" << endl;
			token_array[i] = T_num;
			i++;
				}
			}
		else if (array[k] == '+'){
			//cout << "+" << endl;
			token_array[i] = T_plus;
			i++;
			}
		else if (array[k] == '-'){
			//cout << "-" << endl;
			token_array[i] = T_minus;
			i++;
			}
		else if (array[k] == '*'){
			//cout << "*" << endl;
			if (array[k+1] != '*'){
			token_array[i] = T_times;
			i++;
			}
			else{
			//cout << "**" << endl;
			token_array[i] = T_power;
			i++;
			k = k + 1;
			}
			}
		else if (array[k] == '/'){
			token_array[i] = T_divide;
			i++;
			}
		else if (array[k] == '='){
			//cout << "=" << endl;
			token_array[i] = T_equals;
			i++;
			}
		else if (array[k] == '('){
			//cout << "openparen" << endl;
			token_array[i] = T_openparen;
			i++;
			}
		else if (array[k] == ')'){
			//cout << "openparen" << endl;
			token_array[i] = T_closeparen;
			i++;
			}
		else if (array[k] == '['){
			//cout << "openbrak" << endl;
			token_array[i] = T_opensquare;
			i++;
			}
		else if (array[k] == ']'){
			//cout << "closebrak" << endl;
			token_array[i] = T_closesquare;
			i++;
			}
		else if (array[k] == 'm'){
			//cout << "m" << endl;
			token_array[i] = T_m;
			i++;
			}
		else if (array[k] == 'g'){
			//cout << "goto" << endl;
			token_array[i] = T_goto;
			i++;
			k = k + 3;
			}
		else if (array[k] == 'i'){
			//cout << "if" << endl;
			token_array[i] = T_if;
			i++;
			k = k + 1;
			}
		else if (array[k] == 'p'){
			//cout << "print" << endl;
			token_array[i] = T_print;
			i++;
			k = k + 4;
			}
		else if (array[k] == 'L'){
			//cout << "label" << endl;
			token_array[i] = T_label;
			i++;
			}
		else if (array[k] == ':'){
			//cout << "colon" << endl;
			token_array[i] = T_colon;
			i++;
			}
		else{
			token_array[i] = T_Bad;
			cout << "character: ---> " << array[k] << " <--- can not form a mathematical sentence." << endl;
			errors = true;
			}
		
		}
		sizeT = i;
		return errors;
}

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
	

	
 return token_array[this->next];
}

void scanner_t::eat_token(token_type c)
{
	if (token_array[this->next] == 20){
		linecount++;
	}

	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
	bool inSet = false;
	//WRITEME: cut this bogus stuff out and implement eat_token
	
	if (c != this->next_token()){
		mismatch_error(c);
	}	
	else{
		next++;
	}


}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	next = 0;
	linecount = 1;
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
	return linecount;
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
	void Expression();
	void Jump();
	void Term();
	void Factor();
	void Exp();
	void NL();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)

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
	Statements();
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Statements()
{
parsetree.push(NT_Statements);
switch(scanner.next_token()){
	case T_label: 
			eat_token(T_label);
			eat_token(T_num);
			eat_token(T_colon);
			NL();
			break;
	case T_goto:
			eat_token(T_goto);
			eat_token(T_num);
			Jump();
			break;
	case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			Expression();
			eat_token(T_closesquare);
			eat_token(T_equals);
			Expression();
			break;
	case T_print:
			eat_token(T_print);
			Statements();
			break;
	case T_NL:
			eat_token(T_NL);
			Statements();
	case T_epsilon:
			eat_token(T_epsilon);
			break;
	default:
			cout << "Bad token in Statements: " << "Line " << linecount << endl;

			break;
	}
parsetree.pop();
}

void parser_t::Jump()
{
parsetree.push(NT_Jump);
switch (scanner.next_token()){
	case T_if:
		eat_token(T_if);
		break;
	case T_epsilon:
		eat_token(T_epsilon);
		break;
	case T_NL:
		eat_token(T_NL);
		Statements();
	default:
		cout << "Bad token in Jump: " << "Line " << linecount << endl;

		break;
	}
parsetree.pop();
}

void parser_t::Expression()
{
parsetree.push(NT_Expression);
switch (scanner.next_token()){
	case T_plus:
		Expression();
		eat_token(T_plus);
		Factor();
		break;
	case T_minus:
		Expression();
		eat_token(T_minus);
		Factor();
		break;
	case T_openparen:
		eat_token(T_openparen);
		Expression();
		eat_token(T_closeparen);
		NL();
		break;
	case T_m:
		eat_token(T_m);
		eat_token(T_opensquare);
		Expression();
		eat_token(T_closesquare);
		NL();
		break;
	case (T_times || T_divide || T_power || T_num || T_epsilon):
		Factor();
		break;
	case T_NL:
		eat_token(T_NL);
		Statements();
	default:
		cout << "Bad token in Expression: " << "Line " << linecount << endl;
		break;
	}
parsetree.pop();
}

void parser_t::Factor()
{
parsetree.push(NT_Factor);
	switch(scanner.next_token()){
		case T_times:
			Factor();
			eat_token(T_times);
			Exp();
			break;
		case T_divide:
			Factor();
			eat_token(T_divide);
			Exp();
			break;
		case (T_num || T_power):
			Exp();
			break;
		case T_NL:
			eat_token(T_NL);
			Statements();
		default:
			cout << "Bad token in Factor: " << "Line " << linecount << endl;
			break;
		}
parsetree.pop();
}

void parser_t::Term()
{
parsetree.push(NT_Term);
	switch (scanner.next_token()){
		case T_num:
			eat_token(T_num);
			NL();
			break;
		case T_epsilon:
			eat_token(T_epsilon);
			break;
		case T_NL:
			eat_token(T_NL);
			Statements();
		default:
			cout << "Bad token in Term: " << "Line " << linecount << endl;
			break;
		}
parsetree.pop();
}

void parser_t::Exp()
{
parsetree.push(NT_Power);
	switch (scanner.next_token()){
		case T_power:
			Exp();
			eat_token(T_power);
			Expression();
			break;
		case (T_num || T_epsilon):
			Term();
			break;
		case T_NL:
			eat_token(T_NL);
			Statements();
		default:
			cout << "Bad token in Exp: " << "Line " << linecount << endl;
			break;
		}
parsetree.pop();
}

void parser_t::NL()
{
parsetree.push(NT_NL);
	switch(scanner.next_token()){
		case T_NL:
			eat_token(T_NL);
			Start();
			break;
		case T_eof:
			cout << "made it to end of file" << endl;
			break;

		default:
			break;
	}
parsetree.pop();

}
//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

int main()
{	
	char c;
	int i = 0;
	scanner_t scanner;
	do {
	c = getchar();
	//handle tokens here
	//switch(c)
	//
		//case (c 
	char_array[i] = c;
	i++;
	// << c;
	}
	while (c != EOF);
	size = i;
	lexicalError = scanner.create_tokens(size, char_array);
	/*
	parser_t parser;
	parser.parse();
	*/
	
	/*
	//scanner.create_tokens(char_array);
	for (int j = 0; j < sizeT; j++){
		cout << token_to_string(token_array[j]) << endl;
	}
	*/
	if (lexicalError == true){
		cout << "Scanner errors were found. Please use proper operators and numbers" << endl;
	}
	parser_t parser;
	parser.parse();
		
	/*
	for (int h = 0; h < sizeT; h++){
	cout << token_to_string(token_array[h]) << endl;
	}
	*/
	return 0;
}
