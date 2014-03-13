#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <vector>

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
	NT_Statements_,
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Jump_,
	NT_Assignment,
	NT_Print,
	NT_Expression,
	NT_Expression_,
	NT_Expression__,
	NT_EOF
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
		  case NT_Statements: strncpy(buffer,"Statements", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statements_: strncpy(buffer,"Statements'", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statement: strncpy(buffer,"Statement", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump_: strncpy(buffer,"Jump'", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression_: strncpy(buffer,"Expression'",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression__: strncpy(buffer,"Expression''",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_EOF: strncpy(buffer,"EOF",MAX_SYMBOL_NAME_SIZE); break;
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

	char* scan_num();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.

	// data structure to buffer the input stream
	vector<int> buffer;

	// the current number of new lines consumed
	int newlines;

	// returns the character in the buffer, or loads it
	// from stdin and stores it into the buffer
	int get(unsigned int index);

	void eat(unsigned int amount);

	token_type times_next_token(int index);
	void scan_string(std::string string);

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in

	int index;
	for (index = 0; get(index) == ' ' || get(index) == '\n'; index++);
	int currentChar = get(index);

	switch(currentChar) {
		case EOF:
			return T_eof;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': scan_num(); return T_num;
		case '+': return T_plus;
		case '-': return T_minus;
		case '*': return times_next_token(index + 1);
		case '/': return T_divide;
		case '=': return T_equals;
		case '(': return T_openparen;
		case ')': return T_closeparen;
		case '[': return T_opensquare;
		case ']': return T_closesquare;
		case 'm': return T_m;
		case 'g': scan_string("goto"); return T_goto;
		case 'i': scan_string("if"); return T_if;
		case 'p': scan_string("print"); return T_print;
		case 'L': return T_label;
		case ':': return T_colon;
		default:
			scan_error(currentChar);
			return T_eof;
	}
}

char* scanner_t::scan_num()
{
	int offset;
	for (offset = 0; get(offset) == ' ' || get(offset) == '\n'; offset++);
	int digits = 0;
	int val = 0;
	for (digits = 0; get(digits + offset) >= '0' && get(digits + offset) <= '9'; digits++) val = (val * 10) + (get(digits + offset) - '0');

	if (digits == 10) {
		const int max[10] = {2,1,4,7,4,8,3,6,4,7};
		for (int i = 0; i < 10; i++) {
			if ((get(offset + i) - '0') > max[i])
				scan_error(get(offset + i));
		}
	} else if (digits > 10) scan_error(get(offset + 10));

	char* result = new char[25];
	sprintf(result, "%d", val);

	return result;
}

void scanner_t::scan_string(std::string string)
{
	int offset;
	for (offset = 0; get(offset) == ' ' || get(offset) == '\n'; offset++);
	for (int i = 0; string[i] != '\0'; i++) {
		if (string[i] != get(i + offset)) scan_error(get(i + offset));
	}
}

token_type scanner_t::times_next_token(int index)
{
	int nextIndex;
	for (nextIndex = index; get(nextIndex) == '\n'; nextIndex++);
	int nextChar = get(nextIndex);
	switch(nextChar) {
		case '*': return T_power;
		case 'm':
		case ' ':
		case '(':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': return T_times;
		default:
			scan_error(nextChar);
			return T_eof;
	}
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a
	//mismatch error ( call - mismatch_error(c) )

	if (c != next_token()) mismatch_error(c);

	int startIndex = 0;
	int nextChar;
	while ((nextChar = get(startIndex)) == ' ' || nextChar == '\n' || nextChar == '\r') startIndex++;

	//WRITEME: cut this bogus stuff out and implement eat_token
	switch (c) {
		case T_eof:
			// TODO: I don't know what to do when this comes around
			break;
		case T_num:
			int nextChar;
			while ((nextChar = get(startIndex)) >= '0' && nextChar <= '9') startIndex++;
			// Erase from the begining to the last character
			// excluding the last character because it doesn't belong
			eat(startIndex);
			break;
		case T_plus:
		case T_minus:
		case T_times:
		case T_divide:
		case T_equals:
		case T_openparen:
		case T_closeparen:
		case T_opensquare:
		case T_closesquare:
		case T_m:
		case T_label:
		case T_colon:
			eat(1 + startIndex);
			break;
		case T_power:
		case T_if:
			eat(2 + startIndex);
			break;
		case T_goto:
			eat(4 + startIndex);
			break;
		case T_print:
			eat(5 + startIndex);
			break;
	}
}

scanner_t::scanner_t()
{
	newlines = 1;
	//WRITEME: this is the scanner constructor
}

int scanner_t::get_line()
{
	//this function should return the current line number (used
	//for error reporting
	return newlines;
}


int scanner_t::get(unsigned int index)
{
	// Load more characters into the buffer until we get the desired index
	while (buffer.size() <= index) {
		int character = getchar();
		if (character == '\n') newlines++;
		if (character == EOF) return character;
		else buffer.push_back(character);
	}

	// Return the character at the desired index
	return buffer.at(index);
}

void scanner_t::eat(unsigned int amount)
{
	if (amount < 1) return;

	get(amount - 1);

	// erase(int, int) includes the first but not the last element for deletion
	buffer.erase(buffer.begin(), buffer.begin() + amount);
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
	void Statements_();
	void Statement();
	void Label();
	void Jump();
	void Jump_(std::string label_name);
	void Assignment();
	void Print();
	std::string Expression();
	std::string Expression_(std::string);
	std::string Expression__(std::string);
	std::string Expression___(std::string);
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
	fprintf(stderr, "#include <math.h>\n");
	fprintf(stderr, "#include <string.h>\n");
	fprintf(stderr, "#include <stdio.h>\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
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

	fprintf(stderr, "int main()\n");
	fprintf(stderr, "{\n");
	fprintf(stderr, "\tint m[101];\n");
	fprintf(stderr, "\n");


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

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
	fprintf(stderr, "}\n");
}

//WRITEME: you will need to put the rest of the procedures here

void parser_t::Statements()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Statements);

	bool thereIsMore = true;
		switch( scanner.next_token() )
		{
			case T_label:
			case T_goto:
			case T_m:
			case T_print:
				Statement();
				Statements();
				break;
			case T_eof:
				thereIsMore = false;
				break;
			default:
				syntax_error(NT_Statements);
				break;
		}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Statement()
{
	parsetree.push(NT_Statement);

	switch( scanner.next_token() )
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
	}
	
	fprintf( stderr, "\n");

	parsetree.pop();
}

void parser_t::Label()
{
	parsetree.push(NT_Label);

	eat_token(T_label);
	char* label_name = scanner.scan_num();
	eat_token(T_num);
	eat_token(T_colon);

	fprintf(stderr, "\tlabel%s:", label_name);

	parsetree.pop();
}

// Jump -> "goto" number Jump_
void parser_t::Jump()
{
	parsetree.push(NT_Jump);

	eat_token(T_goto);
	char* label_name = scanner.scan_num();
	eat_token(T_num);

	Jump_(label_name);

	parsetree.pop();
}

// Jump_ -> "if" Expression | epsilon
void parser_t::Jump_(std::string label_name)
{
	std::string exp;
	parsetree.push(NT_Jump_);
	switch(scanner.next_token()) {
		case T_if:
			// conditional jump
			eat_token(T_if);
			exp = Expression();
			fprintf(stderr, "\tif (%s) goto label%s;", exp.c_str(), label_name.c_str());
			break;
		case T_m:
		case T_label:
		case T_goto:
		case T_equals:
		case T_print:
		case T_eof:
			fprintf(stderr, "\tgoto label%s;", label_name.c_str());
			parsetree.drawepsilon();
			// From follow set
			break;
		default:
			syntax_error(NT_Jump_);
	}

	parsetree.pop();
}

// Assignment -> "m" "[" Expression "]" "=" Expression
void parser_t::Assignment()
{
	parsetree.push(NT_Assignment);

	eat_token(T_m);
	eat_token(T_opensquare);
	std::string firstExp = Expression();
	eat_token(T_closesquare);
	eat_token(T_equals);
	std::string secondExp = Expression();

	fprintf(stderr, "\tm[%s] = %s;", firstExp.c_str(), secondExp.c_str());

	parsetree.pop();
}

// Print -> "print" Expression
void parser_t::Print()
{
	parsetree.push(NT_Print);

	eat_token(T_print);
	fprintf(stderr, "\tprintf(\"%%d\\n\", %s);", Expression().c_str());

	parsetree.pop();
}

// Expression -> Expression Expression_
//	       | "(" Expression ")"
//	       | "m" "[" Expression "]"
//	       | number

std::string parser_t::Expression()
{
	std::string firstExpression("");
	std::string result("");
	std::ostringstream stream;
	parsetree.push(NT_Expression);
	switch (scanner.next_token()) {
		case T_openparen:
			eat_token(T_openparen);
			firstExpression = Expression();
			if (scanner.next_token() != T_closeparen) syntax_error(NT_Expression);
			eat_token(T_closeparen);
			stream << "(" << firstExpression << ")";
			result = stream.str();
			break;
		case T_m:
			eat_token(T_m);
			if (scanner.next_token() != T_opensquare) syntax_error(NT_Expression);
			eat_token(T_opensquare);
			firstExpression = Expression();
			if (scanner.next_token() != T_closesquare) syntax_error(NT_Expression);
			eat_token(T_closesquare);
			stream << "m[" << firstExpression << "]";
			result = stream.str();
			break;
		case T_num:
			result = scanner.scan_num();
			eat_token(T_num);
			break;
		default:
			syntax_error(NT_Expression);
			break;
	}

	result = Expression_(result);

	parsetree.pop();

	return result;
}

// Expression_ -> "+" Expression
//		| "-" Expression
//		| Expression__
//		| epsilon

std::string parser_t::Expression_(std::string firstExpression)
{
	std::string result("");
	std::string secondExpression("");
	std::ostringstream stream;
	parsetree.push(NT_Expression_);

	switch(scanner.next_token())
	{
		case T_plus:
			eat_token(T_plus);
			secondExpression = Expression();
			stream << firstExpression << " + " << secondExpression;
			result = stream.str();
			break;
		case T_minus:
			eat_token(T_minus);
			secondExpression = Expression();
			stream << firstExpression << " - " << secondExpression;
			result = stream.str();
			break;
		case T_times:
		case T_divide:
		case T_power:
			// Times and divide and power
			result = Expression__(firstExpression);
			break;
		default:
			result = firstExpression;
			parsetree.drawepsilon();
			// Follow set
	}

	parsetree.pop();

	string s(result);

	return s;
}

// Expression__ -> "*" Expression
//		 | "**" Expression
//		 | "/" Expression

std::string parser_t::Expression__(std::string firstExpression)
{
	std::string result;
	std::string secondExpression;
	std::ostringstream stream;
	parsetree.push(NT_Expression__);

	switch( scanner.next_token() )
	{
		case T_times:
			eat_token(T_times);
			secondExpression = Expression();
			stream << firstExpression << " * " << secondExpression;
			result = stream.str();
			break;
		case T_divide:
			eat_token(T_divide);
			secondExpression = Expression();
			stream << firstExpression << " / " << secondExpression;
			result = stream.str();
			break;
		case T_power:
			eat_token(T_power);
			secondExpression = Expression();
			stream << "pow(" << firstExpression << ", " << secondExpression << ")";
			result = stream.str();
			break;
		default:
			syntax_error(NT_Expression__);
	}

	parsetree.pop();

	return result;
}

/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();
	return 0;
}

