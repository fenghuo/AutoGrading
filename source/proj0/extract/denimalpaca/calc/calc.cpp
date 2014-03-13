#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 50
#define MAX_STRING_LEN 10
#define NUM_OF_TOKES 18

const char toke_dict [] [MAX_STRING_LEN] = {
	"eof",
	"num",
	"+",
	"-",
	"*",
	"/",
	"**",
	"=",
	"(",
	")",
	"[",
	"]",
	"m",
	"goto",
	"if",
	"print",
	"L",
	":"
};

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

token_type string_to_toke(const char* c) {
	token_type t;
	if(strcmp("eof", c) == 0) t = T_eof;
	else if(strcmp("num", c) == 0) t = T_num;
	else if(strcmp("+", c) == 0) t = T_plus;
	else if(strcmp("-", c) == 0) t = T_minus;
	else if(strcmp("*", c) == 0) t = T_times;
	else if(strcmp("/", c) == 0) t = T_divide;
	else if(strcmp("**", c) == 0) t = T_power;
	else if(strcmp("=", c) == 0) t = T_equals;
	else if(strcmp("(", c) == 0) t = T_openparen;
	else if(strcmp(")", c) == 0) t = T_closeparen;
	else if(strcmp("[", c) == 0) t = T_opensquare;
	else if(strcmp("]", c) == 0) t = T_closesquare;
	else if(strcmp("m", c) == 0) t = T_m;
	else if(strcmp("goto", c) == 0) t = T_goto;
	else if(strcmp("if", c) == 0) t = T_if;
	else if(strcmp("print", c) == 0) t = T_print;
	else if(strcmp("L", c) == 0) t = T_label;
	else if(strcmp(":", c) == 0) t = T_colon;
	
	return t;
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
	NT_Assignment,
	NT_Label,
	NT_Jump,
	NT_Jump_p,
	NT_Print,
	NT_Expression,
	NT_Expression_p,
	NT_Term,
	NT_Term_p,
	NT_Factor,
	NT_Power
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
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression_p: strncpy(buffer,"Expression_p",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump_p: strncpy(buffer,"Jump_p",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term_p: strncpy(buffer,"Term_p",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Power: strncpy(buffer,"Power",MAX_SYMBOL_NAME_SIZE); break;
		  default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
		}
	return buffer;
}

struct toke_list {
	token_type c;
	toke_list * next;
	int myint;
};

/*** Scanner Class ***********************************************/

class scanner_t {
  public:

	//eats the next token and prints an error if it is not of type c
	void eat_token(token_type c);

	//peeks at the lookahead token
	token_type next_token();

	//return line number for errors
	int get_line();

	//put token to be eaten in a linked list
	void add_to_list(token_type c);

	int check_newline(int* bufptr);

	int buffer [MAX_SYMBOL_NAME_SIZE];
	int* bufptr;
	int line_counter;
	int holdint;
	toke_list * tokelist;
	
	//constructor 
	scanner_t();

	//destructor
	~scanner_t();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  

	token_type check_goto(int* bufptr);
	token_type check_if(int* bufptr);
	token_type check_print(int* bufptr);

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
};

int scanner_t::check_newline(int* bufptr) {
	if(*bufptr == 10) return 1;
	else return 0;
}

token_type scanner_t::check_goto(int* bufptr) {
	if(*bufptr == 'g' && *(bufptr+1) == 'o' && *(bufptr+2) == 't' && *(bufptr+3) == 'o')
		return T_goto;
	else scan_error(*bufptr);
}

token_type scanner_t::check_if(int* bufptr) {
	if(*bufptr == 'i' && *(bufptr+1) == 'f')
		return T_if;
	else scan_error(*bufptr);
}

token_type scanner_t::check_print(int* bufptr) {
	if(*bufptr == 'p' && *(bufptr+1) == 'r' && *(bufptr+2) == 'i' && *(bufptr+3) == 'n'&&
		*(bufptr+4) == 't') return T_print;
	else scan_error(*bufptr);
}

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
	int i; int check;
	token_type toke;
	check = 0;
	holdint = -1;

	//check for whitespace, eat it
	if(*bufptr == ' ') {
		while(*bufptr == ' '){
			bufptr = bufptr + 1;
		}
	}
	if(*bufptr == '\n') return T_eof;

	if (*bufptr == EOF) toke = T_eof;
	else if (*bufptr >= '0' && *bufptr <= '9'){
		toke = T_num;
		holdint = *bufptr - 48;
	}
	else if (*bufptr == 'm') toke = T_m;
	else if (*bufptr == 'g') toke = check_goto(bufptr);
	else if (*bufptr == 'i') toke = check_if(bufptr);
	else if (*bufptr == 'p') toke = check_print(bufptr);
	else if (*bufptr == 'L') toke = T_label;
	else check = 1;

	if(check == 1){
		for(i = 0; i < NUM_OF_TOKES; i++) {
			if(*bufptr == *toke_dict[i]){
				toke = string_to_toke(toke_dict[i]);
				if(strcmp(toke_dict[i], "*") == 0){
					if(*(bufptr+1) == '*'){
						toke = T_power;
					}
				}
				check = 0;
				break;
			}
		}
	}

	if(check == 1)
		scan_error(*bufptr);
	return toke;
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
	token_type toke;
	char* strtoke; char* strc;
	toke = next_token();
	strtoke = token_to_string(toke);
	strc = token_to_string(c);
	if(strcmp(strc, strtoke) == 0){
		add_to_list(c);
		switch(c) {
			case T_print:
				bufptr = bufptr + 5;
				break;
			case T_goto:
				bufptr = bufptr + 4;
				break;
			case T_eof:
				break;
			case T_if:
				bufptr = bufptr + 2;
				break;
			case T_power:
				bufptr = bufptr + 2;
				break;
			default:
				bufptr = bufptr + 1;
		}
		if(*bufptr == ' ') {
			while(*bufptr == ' '){
				bufptr = bufptr + 1;
			}
		}
	}
	else
		mismatch_error(c);
}

void scanner_t::add_to_list(token_type c){
	int keepint;
	toke_list * newtoke = new toke_list();
	toke_list * ptrtoke;
	newtoke->c = c;

	keepint = holdint;
	if(holdint != -1) {
		//Below ifs and for check to see if negative number
		if(tokelist != NULL && tokelist->next != NULL) {
			for(ptrtoke = tokelist; ptrtoke->next != NULL; ptrtoke = ptrtoke->next) {
				if(ptrtoke->next->next == NULL && ptrtoke->next->c == T_minus && ptrtoke->myint == 0){
					printf("Range error: negative number\n");
					exit(1);
				}
			}
		}
		bufptr = bufptr + 1;
		if(next_token() == T_num){
			while(next_token() == T_num) {
				keepint = keepint * 10;
				keepint = keepint + holdint;
				bufptr = bufptr + 1;
			}
			bufptr = bufptr - 1;
		}
		else bufptr = bufptr - 1;
		holdint = keepint;
		if(holdint > 2147483647) {
			printf("Range error: Integer greater than 2^31-1\n", holdint);
			exit(1);
		}
	}
	
	newtoke->myint = holdint;
	newtoke->next = NULL;

	if(tokelist == NULL){
		tokelist = newtoke;
	}
	else {
		toke_list* movetoke;
		movetoke = tokelist;
		while(movetoke->next != NULL){
			movetoke = movetoke->next;
		}
		movetoke->next = newtoke;
	}
}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	bufptr = NULL;
	line_counter = 1;
	holdint = -1;

	tokelist = NULL;
}

scanner_t::~scanner_t()
{
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
	return line_counter;
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
		printf( "}\n" );
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
	void Assignment();
	void Label();
	void Jump();
	void Jump_p();
	void Print();
	void Expression();
	void Expression_p();
	void Term();
	void Term_p();
	void Factor();
	void Power();

  public:	
	void parse();
	void write_c();
	toke_list* print_c(toke_list* ptr);
	toke_list* if_c(toke_list* ptr);
	toke_list* goto_c(toke_list* ptr);
	toke_list* label_c(toke_list* ptr);
	toke_list* num_c(toke_list* ptr);
	toke_list* paren_c(toke_list* ptr);
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

void parser_t::Assignment() {
	parsetree.push(NT_Assignment);
	int i;
	i = scanner.check_newline(scanner.bufptr);
	if(i == 1){
		parsetree.pop();
		Statements();
		return;
	}
	switch( scanner.next_token() )
	{
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			Expression();
			eat_token(T_closesquare);
			eat_token(T_equals);
			Expression();
			break;
		default:
			syntax_error(NT_Assignment);
			break;
	}
	parsetree.pop();
}

void parser_t::Label() {
	parsetree.push(NT_Label);
	int i;
	i = scanner.check_newline(scanner.bufptr);
	if(i == 1){
		parsetree.pop();
		Statements();
		return;
	}
	switch( scanner.next_token() )
	{
		case T_label:
			eat_token(T_label);
			eat_token(T_num);
			eat_token(T_colon);
			break;
		default:
			syntax_error(NT_Label);
			break;
	}
	parsetree.pop();
}

void parser_t::Jump() {
	parsetree.push(NT_Jump);
	int i;
	i = scanner.check_newline(scanner.bufptr);
	if(i == 1){
		parsetree.pop();
		Statements();
		return;
	}
	switch( scanner.next_token() )
	{
		case T_goto:
			eat_token(T_goto);
			eat_token(T_num);
			Jump_p();
			break;
		default:
			syntax_error(NT_Jump);
			break;
	}
	parsetree.pop();
}

void parser_t::Jump_p() {
	parsetree.push(NT_Jump_p);
	switch( scanner.next_token() )
	{
		case T_if:
			eat_token(T_if);
			Expression();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}

void parser_t::Print() {
	parsetree.push(NT_Print);
	int i;
	i = scanner.check_newline(scanner.bufptr);
	if(i == 1){
		parsetree.pop();
		Statements();
		return;
	}
	switch( scanner.next_token() )
	{
		case T_print:
			eat_token(T_print);
			Expression();
			break;
		default:
			syntax_error(NT_Print);
			break;
	}
	parsetree.pop();
}

void parser_t::Expression() {
	parsetree.push(NT_Expression);
	int i;
	i = scanner.check_newline(scanner.bufptr);
	if(i == 1){
		parsetree.pop();
		return;
	}

	Term();
	Expression_p();

	parsetree.pop();
}

void parser_t::Expression_p() {
	parsetree.push(NT_Expression_p);
	switch( scanner.next_token() )
	{
		case T_plus:
			eat_token(T_plus);
			Expression();
			break;
		case T_minus:
			eat_token(T_minus);
			Expression();
			break;
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			Expression();
			eat_token(T_closesquare);
			Expression();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}

void parser_t::Term() {
	parsetree.push(NT_Term);

	Factor();
	Term_p();

	parsetree.pop();
}

void parser_t::Term_p() {
	parsetree.push(NT_Term_p);
	switch( scanner.next_token() ) {
		case T_times:
			eat_token(T_times);
			Term();
			break;
		case T_divide:
			eat_token(T_divide);
			Term();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}

void parser_t::Factor() {
	parsetree.push(NT_Factor);
	
	switch( scanner.next_token() ) {
		case T_num:
			eat_token(T_num);
			Power();
			break;
		case T_openparen:
			eat_token(T_openparen);
			Expression();
			eat_token(T_closeparen);
			Power();
			break;
		case T_opensquare:
			eat_token(T_opensquare);
			Expression();
			eat_token(T_closesquare);
			Power();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}

void parser_t::Power() {
	parsetree.push(NT_Power);
	switch( scanner.next_token() ) {
		case T_power:
			eat_token(T_power);
			Factor();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}

//Here is an example
void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);
	Statements();
	parsetree.pop();
}

void parser_t::Statement()
{
	parsetree.push(NT_Statement);
	switch( scanner.next_token() ) 
	{
		case T_m:
			Assignment();
			break;
		case T_goto:
			Jump();
			break;
		case T_print:
			Print();
			break;
		case T_label:
			Label();
			break;
		case T_eof:
			break;
		default:
			syntax_error(NT_Statement);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Statements()
{
	parsetree.push(NT_Statements);

	int i;
	int c;
	for(i = 0; i < MAX_SYMBOL_NAME_SIZE; i++) {
		scanner.buffer[i] = 0;
	}

	for(i = 0; i < MAX_SYMBOL_NAME_SIZE; i++) {
		c = getchar();
		if(c == EOF) {
			break;
		}
		scanner.buffer[i] = c;
		if(scanner.buffer[i] == 10) {
			break;
		}
	}
	scanner.bufptr = scanner.buffer;

	while(*(scanner.bufptr) != EOF) {
	if(scanner.line_counter > 1) {
		for(i = 0; i < MAX_SYMBOL_NAME_SIZE; i++) {
			scanner.buffer[i] = 0;
		}

		for(i = 0; i < MAX_SYMBOL_NAME_SIZE; i++) {
			c = getchar();
			scanner.buffer[i] = c;
			if(scanner.buffer[i] == 10) {
				break;
			}
		}
		scanner.bufptr = scanner.buffer;
	}

		scanner.eat_token(T_eof);
		Statement();
		scanner.line_counter ++;
	}

	eat_token(T_eof);
	parsetree.pop();
	write_c();
}

void parser_t::write_c() {
	toke_list* ptr;
	ptr = scanner.tokelist;
	fprintf(stderr, "#include <stdio.h>\n\nint power(int a, int b) {\n\tif(b==0) return 1;\n\telse if (b==1) return a;\n\telse return(a * power(a,b-1));\n }\n\nint main() {\n\tint m[101];\n\t");

	for(ptr=ptr->next; ptr->next != NULL; ptr = ptr->next) {
		if(ptr->c == T_eof && ptr->myint == -1) fprintf(stderr, ";\n\t");
		else if(ptr->c == T_label) ptr = label_c(ptr);
		else if(ptr->c == T_colon) fprintf(stderr, ":");
		else if(ptr->c == T_print) ptr = print_c(ptr);
		else if(ptr->c == T_if) ptr = if_c(ptr);
		else if(ptr->c == T_goto) ptr = goto_c(ptr);
		else if(ptr->c == T_plus) fprintf(stderr, "+ ");
		else if(ptr->c == T_minus) fprintf(stderr, "- ");
		else if(ptr->c == T_times) fprintf(stderr, "* ");
		else if(ptr->c == T_divide) fprintf(stderr, "/ ");
		else if(ptr->c == T_m) fprintf(stderr, "m");
		else if(ptr->c == T_opensquare) fprintf(stderr, "[ ");
		else if(ptr->c == T_closesquare) fprintf(stderr, "] ");
		else if(ptr->c == T_openparen) ptr = paren_c(ptr);
		else if(ptr->c == T_closeparen) fprintf(stderr, ") ");
		else if(ptr->c == T_equals) fprintf(stderr, "= ");
		else if(ptr->c == T_num) ptr = num_c(ptr);
	}
	fprintf(stderr, "\n\treturn 0;\n}");
}

toke_list* parser_t::paren_c(toke_list* ptr) {
	toke_list* nptr;
	for(nptr = ptr->next; nptr->c != T_closeparen; nptr = nptr->next) {}
	if(nptr->next->c == T_power) {
		fprintf(stderr, "power(");
		for(nptr = ptr; nptr->c != T_power; nptr = nptr->next) {
			if(nptr->c == T_openparen) fprintf(stderr, "( ");
			else if(nptr->c == T_closeparen) fprintf(stderr, ") ");
			else if(nptr->c == T_plus) fprintf(stderr, "+ ");
			else if(nptr->c == T_minus) fprintf(stderr, "- ");
			else if(nptr->c == T_times) fprintf(stderr, "* ");
			else if(nptr->c == T_divide) fprintf(stderr, "/ ");
			else if(nptr->c == T_num) nptr = num_c(nptr);
		}
		fprintf(stderr, " , ");
		nptr = nptr->next->next;
		nptr = num_c(nptr);
		fprintf(stderr, ") ");
		ptr = nptr;
	}
	else fprintf(stderr, "( ");
	return ptr;
}

toke_list* parser_t::num_c(toke_list* ptr) {
	toke_list* nptr;
	nptr = ptr->next;
	if(nptr->c == T_power) {
		fprintf(stderr,"power(%d,%d) ", ptr->myint, nptr->next->next->myint);
		ptr = nptr->next->next;
	}
	else fprintf(stderr,"%d ", ptr->myint);
	return ptr;
}

toke_list* parser_t::label_c(toke_list* ptr) {
	for(ptr=ptr->next; ptr->next!=NULL; ptr=ptr->next){
		if(ptr->c == T_eof) break;
		else if(ptr->c == T_num) {
			fprintf(stderr, "L%d:\n\t", ptr->myint);
		}
	}
	return ptr;
}

toke_list* parser_t::goto_c(toke_list* ptr) {
	int thisint;
	for(ptr = ptr->next; ptr->next != NULL; ptr = ptr->next) {
		if(ptr->c == T_eof && ptr->myint == -1) {
			break;
		}
		else if(ptr->c == T_num) thisint = ptr->myint;
		else if(ptr->c == T_if) {
			ptr = if_c(ptr);
			break;
		}
	}
	fprintf(stderr, " goto L%d;\n\t", thisint);
	return ptr;
}

toke_list* parser_t::if_c(toke_list* ptr) {
	fprintf(stderr, "if(");
	for(ptr = ptr->next; ptr->next != NULL; ptr = ptr->next) {
		if(ptr->c == T_eof && ptr->myint == -1) {
			fprintf(stderr, ")");
			break;
		}
		else if(ptr->c == T_plus) fprintf(stderr, "+ ");
		else if(ptr->c == T_minus) fprintf(stderr, "- ");
		else if(ptr->c == T_times) fprintf(stderr, "* ");
		else if(ptr->c == T_divide) fprintf(stderr, "/ ");
		else if(ptr->c == T_m) fprintf(stderr, "m ");
		else if(ptr->c == T_opensquare) fprintf(stderr, "[ ");
		else if(ptr->c == T_closesquare) fprintf(stderr, "] ");
		else if(ptr->c == T_openparen) fprintf(stderr, "( ");
		else if(ptr->c == T_closeparen) fprintf(stderr, ") ");
		else if(ptr->c == T_num) ptr = num_c(ptr);
	}
	return ptr;
}

toke_list* parser_t::print_c(toke_list* ptr) {
	int vars;
	fprintf(stderr, "printf(\"");
	for(; ptr->next != NULL; ptr = ptr->next) {
		if(ptr->c == T_eof) {
			fprintf(stderr, "\\n\", m[ %d ]);\n\t", vars);
			break;
		}
		else if(ptr->c == T_num) {
			fprintf(stderr, "%%d");
			vars = ptr->myint;
		}
	}
	return ptr;
}
/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();
	return 0;
}
