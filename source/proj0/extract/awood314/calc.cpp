#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25
#define MAX_INT 2147483647 // for checking range errors on scanned numbers (2^31 - 1)

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
		case T_eof:			strncpy(buffer,"eof",MAX_SYMBOL_NAME_SIZE); break;
		case T_num:			strncpy(buffer,"num",MAX_SYMBOL_NAME_SIZE); break;
		case T_plus:		strncpy(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
		case T_minus:		strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
		case T_times:		strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
		case T_divide:		strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
		case T_power:		strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
		case T_equals:		strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
		case T_openparen:	strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
		case T_closeparen:	strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
		case T_opensquare:	strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
		case T_closesquare:	strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
		case T_m:			strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
		case T_goto:		strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
		case T_if:			strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
		case T_print:		strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
		case T_label:		strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
		case T_colon:		strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
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
	NT_Start, // nonterminal for each line of code
	NT_Expression, // terms that can be added and subtracted
	NT_Term, // values that can be multiplied and divided with precedence
	NT_Value, // terms that can be squared, paren'd, and can address memory with precedence
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
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;	
		  case NT_Value: strncpy(buffer,"Value",MAX_SYMBOL_NAME_SIZE); break;	
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

	// return the last num eaten for emitting to c
	int get_num();

	//constructor 
	scanner_t();

  private:

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
	//error message and exit for num out of range [0 , 2^31 - 1]
	void range_error(long int num);
	
	// current line number
	int line_num;
	// current index being scanned
	int index;
	// value of last eaten num for emitting c code
	int current_num;
	// string of current line being scanned
	char current_line[256];
};

token_type scanner_t::next_token()
{
	// read first character to determine which token it is
	switch(current_line[index]) {
		// spaces are skipped
		case ' ':
			index++;
			return next_token();
		case '+':	return T_plus;
		case '-':	return T_minus; 
		case '*':		
			// check for ** power token
			if(current_line[index + 1] == '*')
				return T_power;
			else
				return T_times; 
		case '/':	return T_divide;
		case '=':	return T_equals;
		case '(':	return T_openparen;
	    case ')':	return T_closeparen;
	    case '[':	return T_opensquare;
	    case ']':	return T_closesquare;
	    case 'm':	return T_m;
	    case 'L':	return T_label;
	    case ':':	return T_colon;
	    // check more characters for longer tokens
	    case 'g':	
	    	if(current_line[index + 1] != 'o')
	    		scan_error(current_line[index + 1]);
	    	if(current_line[index + 2] != 't')
	    		scan_error(current_line[index + 2]);
	    	if(current_line[index + 3] != 'o')
	    		scan_error(current_line[index + 3]);
	     	return T_goto;	
	    case 'i':	
	    	if(current_line[index + 1] != 'f')
	    		scan_error(current_line[index + 1]);
    		return T_if;
	    case 'p':
	    	if(current_line[index + 1] != 'r')
	    		scan_error(current_line[index + 1]);
	    	if(current_line[index + 2] != 'i')
	    		scan_error(current_line[index + 2]);
	    	if(current_line[index + 3] != 'n')
	    		scan_error(current_line[index + 3]);
	    	if(current_line[index + 4] != 't')
	    		scan_error(current_line[index + 4]);	
    		return T_print;	
    	// next line
    	case '\n':
    		// check for eof
    		if(fgets(current_line, 256, stdin) == NULL)
    			return T_eof;
    		index = 0;
    		line_num++;
    		return next_token();
    	// eof
    	case NULL:
    		return T_eof;
	}
	if(current_line[index] >= '0' && current_line[index] <= '9')
		return T_num;
	// if none of the tokens match, erorr
	scan_error(current_line[index]);	
}

void scanner_t::eat_token(token_type c)
{
	token_type current_token = next_token();
	// check that it matches
	if(current_token != c)
		mismatch_error(c);

	int i = 0;
	char temp[256];
	long int num;
	// increment current index by the specific token's length to eat
	switch(current_token) {
		case T_goto:	index += 4; break;
		case T_if:		index += 2; break;
		case T_print:	index += 5; break;
		case T_power:	index += 2; break;
		case T_num:
			//check length of num and possible range error
			while(current_line[index] >= '0' && current_line[index] <= '9') {
				temp[i] = current_line[index];
				i++;
				index++;
			}
			temp[i] = '\n';
			num = atol(temp);
			if(num > MAX_INT)
				range_error(num);
			// store num so that it can be used when emitting c
			current_num = num;
			break;
		default:		index++; break;
	}
}

int scanner_t::get_num()
{
	return current_num;
}
scanner_t::scanner_t()
{
	line_num = 1;
	index = 0;
	fgets(current_line, 256, stdin);
}

int scanner_t::get_line()
{
	return line_num;
}

void scanner_t::range_error(long int num)
{
	printf("range error: num '%d' out of range -line %d\n", num, get_line());
	exit(1);
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
	void Expression();
	void Term();
	void Value();

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
	// emit necessary includes and main function
	fprintf(stderr,"#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>\n\nint main() {\n\tint m[101];\n");
	Start();
	fprintf(stderr,"}\n");
}


void parser_t::Start()
{	
	// formatting each line with a tab for clarity
	fprintf(stderr,"\t");
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);

	int L_num; 
	switch( scanner.next_token() ) 
	{
		// Label
		case T_label:
			eat_token(T_label);
			fprintf(stderr,"L");
			eat_token(T_num);
			fprintf(stderr,"%d",scanner.get_num());
			eat_token(T_colon);
			fprintf(stderr,":\n");
			Start();
			break;
		// Print
		case T_print:
			eat_token(T_print);
			fprintf(stderr,"printf(\"%%d\\n\", ");
			Expression();
			fprintf(stderr,");\n");
			Start();
			break;
		// Assignment
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			fprintf(stderr,"m[");
			Expression();
			eat_token(T_closesquare);
			eat_token(T_equals);
			fprintf(stderr,"] = ");
			Expression();
			fprintf(stderr,";\n");
			Start();
			break;
		// Goto
		case T_goto:
			eat_token(T_goto);
			eat_token(T_num);
			// save num for "goto" line
			L_num = scanner.get_num();
			if(scanner.next_token() == T_if) {
				// "if" line comes before "goto" line
				eat_token(T_if);
				fprintf(stderr,"if(");
				Expression();
				fprintf(stderr,")\n\t\t");
			}
			fprintf(stderr,"goto L");
			fprintf(stderr,"%d",L_num);
			fprintf(stderr,";\n");
			Start();
			break;
		// EOF
		case T_eof:
			// end of main function
			fprintf(stderr,"return 0;\n");
			parsetree.drawepsilon();
			break;
		default:
			// grammar expects one of the above symbols, so error if anything else
			syntax_error(NT_Start);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Expression() {
	// Expression   : Term '+' Expression
	// 				| Term '-' Expression
	//			   	| Term

	parsetree.push(NT_Expression);
	Term();
	switch ( scanner.next_token() )
	{
		case T_plus:
			eat_token(T_plus);
			fprintf(stderr," + ");
			Expression();
			break;
		case T_minus:
			eat_token(T_minus);
			fprintf(stderr," - ");
			Expression();
			break;
	}

	parsetree.pop();
}

void parser_t::Term() {
	// Term		: Value '*' Term
	// 			| Value '/' Term
	//			| Value

	parsetree.push(NT_Term);
	Value();
	switch ( scanner.next_token() ) {
		case T_times:
			eat_token(T_times);
			fprintf(stderr," * ");
			Term();
			break;
		case T_divide:
			eat_token(T_divide);
			fprintf(stderr," / ");
			Term();
			break;
	}

	parsetree.pop();
}

void parser_t::Value() {
	// Value 	: 'num' '**' Value
	// 			| 'num'
	// 			| 'm' '[' Expression ']'
	//			| '(' Expression ')'

	parsetree.push(NT_Value);
	switch( scanner.next_token() ) {
		case T_num:
			eat_token(T_num);
			// num is either squared or not
			if( scanner.next_token() == T_power) {
				eat_token(T_power);
				fprintf(stderr,"pow(%d,",scanner.get_num());
				Value();
				fprintf(stderr,")");
			}
			else
				fprintf(stderr,"%d",scanner.get_num());
			break;
		case T_m:
			eat_token(T_m);
			fprintf(stderr,"m");
			eat_token(T_opensquare);
			fprintf(stderr,"[");
			Expression();
			eat_token(T_closesquare);
			fprintf(stderr,"]");
			break;
		case T_openparen:
			eat_token(T_openparen);
			fprintf(stderr,"(");
			Expression();
			eat_token(T_closeparen);
			fprintf(stderr,")");
			break;
		default:
			syntax_error(NT_Value);
			break;
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
