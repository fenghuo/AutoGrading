#include <queue>
#include <string>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <math.h>

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
	NT_Expression,
	//WRITEME: add symbolic names for you non-terminals here
	NT_Statements,
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Jump1,
	NT_Assignment,
	NT_Print,
	NT_Expression1,
	NT_Multdiv,
	NT_Multdiv1,
	NT_Exp,
	NT_Exp1,
	NT_Par,
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
		  case NT_Expression1: strncpy(buffer,"Expression1",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump1: strncpy(buffer,"Jump1",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Multdiv: strncpy(buffer,"HELLO",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Multdiv1: strncpy(buffer,"HELLOPLEASE",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Exp: strncpy(buffer,"Exp",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Exp1: strncpy(buffer,"Exp1",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Par: strncpy(buffer,"Par",MAX_SYMBOL_NAME_SIZE); break;
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
	
	// queue stuff
	int pop();

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

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	token_type bogo_token;
	
	void range_error(int n);
	int currLine;
	int toggle;
	token_type currToken;
	queue<int> numQueue;
	queue<char> opQueue;
};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
	char c, tmp;
	if( toggle == 0 ) 
	{
		c = getchar();
		if (c == '\n')
		{
			currLine++;
			//cerr << '\n';
			return next_token();
			//c = getchar();
		}
		else if ( c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' )
		{
			int num = c - '0';
			c = cin.peek();
			while ( c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' )
			{
				c = getchar();
				int tmp = c - '0';
				num = num*10 + tmp;
				c = cin.peek();
			}
			if (num < 0 || num > pow(2, 31)-1)
				range_error(num);
			else
			{
				numQueue.push(num);
				//cerr << num;
				currToken = T_num;
			}
		}
		else if (c == '+')
			currToken = T_plus;
		else if (c == '-')
			currToken = T_minus;
		else if (c == '*')
		{
			if( cin.peek() == '*')
			{
				getchar();
				currToken = T_power;
			}
			else
				currToken = T_times;
		}
		else if (c == '/')
			currToken = T_divide;
		else if (c == '=')
			currToken = T_equals;
		else if (c == '(')
			currToken = T_openparen;
		else if (c == ')')
			currToken = T_closeparen;
		else if (c == '[')
			currToken = T_opensquare;
		else if (c == ']')
			currToken = T_closesquare;
		else if (c == 'm')
			currToken = T_m;
		else if (c == 'g')
		{
			//tmp = getchar();
			if (getchar() != 'o')
				scan_error(tmp);
			tmp = getchar();
			if (tmp != 't')
				scan_error(tmp);
			tmp = getchar();
			if (tmp != 'o')
				scan_error(tmp);
			currToken = T_goto;
		}
		else if (c == 'i')
		{
			tmp = getchar();
			if (tmp != 'f')
				scan_error(tmp);
			currToken = T_if;
		}
		else if (c == 'p')
		{
			tmp = getchar();
			if (tmp != 'r')
				scan_error(tmp);
			tmp = getchar();
			if (tmp != 'i')
				scan_error(tmp);
			tmp = getchar();
			if (tmp != 'n')
				scan_error(tmp);
			tmp = getchar();
			if (tmp != 't')
				scan_error(tmp);
			currToken = T_print;
		}
		else if (c == 'L')
			currToken = T_label;
		else if (c == ':')
			currToken = T_colon;
		else if (c == EOF)
			currToken = T_eof;
		else if (c == ' ')
			return next_token();
        else
		    scan_error(c);
		toggle = 1;
	}
	return currToken;
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eat_token
	if (toggle == 1)
	{
		//cout << "EATING TOKEN " << token_to_string(currToken) << endl;
		if (c != currToken)
			mismatch_error(c);
		else
			toggle = 0;
	}
	else
	{
		//cout << "EATING TOKEN " << token_to_string(next_token()) << endl;
		token_type ch = next_token();
		if (ch != c)
			mismatch_error(c);
		else
			toggle = 0;
	}
}

scanner_t::scanner_t()
{
  //WRITEME: this is the scanner constructor
  currLine = 1;
  toggle = 0; 
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting	
	return currLine;
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

void scanner_t::range_error (int n)
{
	printf("range error: %d out of the range 0 to 2^31-1", n);
	exit(1);
}

int scanner_t::pop()
{
	int n = numQueue.front();
	numQueue.pop();
	return n;
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
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)
	string Statements();
	string Statements1();
	string Statement();
	string Label();
	string Jump();
	string Jump1(string str);
	string Assignment();
	string Print();
	string Expression();
	string Expression1();
	string Multdiv();
	string Multdiv1();
	string Exp();
	string Exp1();
	string Par();
	queue<token_type> opQueue;
	string int_to_string(int num);
  public:	
	void parse();
	void emitCode();
};

void parser_t::emitCode()
{
	//cerr << "not in while loop";
	token_type t = opQueue.front();
	while (t != T_eof)
	{
		t = opQueue.front();
		//cerr << token_to_string(op); 
		token_type tmp;
		switch(t)
		{
			case T_num: // need to check for power or if
				opQueue.pop(); // pop the num
				tmp = opQueue.front(); // check for pow
				if (tmp == T_power)
				{
					cerr << "pow(" << scanner.pop() << "," << scanner.pop() << ");";
				}
				else
					cerr << scanner.pop();
				break;
			case T_plus:
				cerr << scanner.pop() << "+";
				break;
			case T_minus:
				cerr << scanner.pop() << "-";
				break;
			case T_times:
				cerr << scanner.pop() << "*";
				break;
			case T_divide:
				cerr << scanner.pop();
				break;
			case T_label:
				cerr << "L" << scanner.pop();
				break;
			
		}
		opQueue.pop();
	}
}


//this function not only eats the token (moving the scanner forward one
//token), it also makes sure that token is drawn in the parse tree 
//properly by calling push and pop.
void parser_t::eat_token(token_type t)
{
	parsetree.push(t);
	scanner.eat_token(t);
	if (t != T_eof)
		opQueue.push(t);
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
	parsetree.push(NT_Start);
	Statements();
	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here
string parser_t::Statements()
{
	//cout << "IN STATEMENTS" << endl;
	parsetree.push(NT_Statements);
	switch (scanner.next_token())
	{
		case T_label: 
		case T_goto: 
		case T_m:
		case T_print:
			cerr << Statement();
			cerr << "\n";
			Statements();
			break;
		case T_eof: parsetree.drawepsilon(); break;
		default: syntax_error(NT_Statements); break;
	}
	parsetree.pop();
	return "";
}

string parser_t::Statement()
{
	//cout << "IN STATEMENT" << endl;
	string s;
	parsetree.push(NT_Statement);
	switch(scanner.next_token())
	{
		case T_label: s += Label(); break;
		case T_goto: s += Jump()+ ";"; break;
		case T_m: s += Assignment() + ";"; break;
		case T_print: s += Print() + ";"; break;
		default: syntax_error(NT_Statement); break;
	}
	parsetree.pop();
	return s;
}

string parser_t::Label()
{	
	//cout << "IN LABEL" << endl;
	string s;
	parsetree.push(NT_Label);
	if (scanner.next_token() != T_label)
		syntax_error(NT_Label);
	eat_token(T_label);
	eat_token(T_num);
	eat_token(T_colon);
	parsetree.pop();
	return "L"+int_to_string(scanner.pop())+":";
}

string parser_t::Jump()
{
	//cout << "IN JUMP" << endl;
	string s;
	parsetree.push(NT_Jump);
	if (scanner.next_token() != T_goto)
		syntax_error(NT_Jump);
	eat_token(T_goto);
	eat_token(T_num);
	s += int_to_string(scanner.pop());
	parsetree.pop();
	return Jump1(s);
}

string parser_t::Jump1(string str)
{
	//cout << "IN JUMP1" << endl;
	parsetree.push(NT_Jump1);
	string s = "goto L" + str;
	switch(scanner.next_token())
	{
		case T_if:
			eat_token(T_if);
			s = "";
			s += "if (";
			s += Expression() +")";
			s += " goto L";
			s += str;
			break;
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			break;
		case T_eof: parsetree.drawepsilon(); break;
		default: syntax_error(NT_Jump1); break;
	}
	parsetree.pop();
	return s;
}

string parser_t::Assignment()
{
	//cout << "IN ASSIGNMENT" << endl;
	string s;
	parsetree.push(NT_Assignment);
	if (scanner.next_token() != T_m)
		syntax_error(NT_Assignment);
	eat_token(T_m);
	eat_token(T_opensquare);
	s += "m[";
	s += Expression();
	eat_token(T_closesquare);
	eat_token(T_equals);
	s += "] = ";
	s += Expression();
	parsetree.pop();
	return s;
}

string parser_t::Print()
{
	//cout << "IN PRINT" << endl;
	string s;
	if (scanner.next_token() != T_print)
		syntax_error(NT_Print);
	eat_token(T_print);
	s += "printf(\"%d\", ";
	s += Expression();
	s += ")";
	parsetree.pop();
	return s;
}

string parser_t::Expression()
{
	//cout << "IN EXPRESSION" << endl;
	string s;
	parsetree.push(NT_Expression);
	s += Multdiv();
	s += Expression1();
	parsetree.pop();
	return s;
}

string parser_t::Expression1()
{
	//cout << "IN EXPRESSION1" << endl;
	string s = "";
	parsetree.push(NT_Expression1);
	switch (scanner.next_token())
	{
		case T_plus:
			eat_token(T_plus);
			s += "+";
			s += Multdiv();
			s += Expression1();
			break;
		case T_minus:
			eat_token(T_minus);
			s += "-";
			s += Multdiv();
			s += Expression1();
			break;
		case T_closeparen:
		case T_closesquare:
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			break;
		case T_eof: parsetree.drawepsilon(); break;
		default: syntax_error(NT_Expression1); break;
	}
	parsetree.pop();
	return s;
}

string parser_t::Multdiv()
{
	//cout << "IN MULTDIV" << endl;
	string s = "";
	parsetree.push(NT_Multdiv);
	s += Exp();
	s += Multdiv1();
	parsetree.pop();
	return s;
}

string parser_t::Multdiv1()
{
	//cout << "IN MULTDIV1" << endl;
	string s = "";
	parsetree.push(NT_Multdiv1);
	switch (scanner.next_token())
	{
		case T_times:
			eat_token(T_times);
			s += "*";
			s += Exp();
			s += Multdiv1();
			break;
		case T_divide:
			eat_token(T_divide);
			s += "/";
			s += Exp();
			s += Multdiv1();
			break;
		case T_plus:
		case T_minus:
		case T_closeparen:
		case T_closesquare:
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			break;
		case T_eof: parsetree.drawepsilon(); break;
		default: syntax_error(NT_Multdiv1); break;
	}
	parsetree.pop();
	return s;
}

string parser_t::Exp()
{
	//cout << "IN EXP" << endl;
	string s = "";
	parsetree.push(NT_Exp);
	s += Par();
	s += Exp1();
	parsetree.pop();
	return s;
}

string parser_t::Exp1()
{
	//cout << "IN EXP1" << endl;
	string s = "";
	parsetree.push(NT_Exp1);
	switch (scanner.next_token())
	{
		case T_power:
			eat_token(T_power);
			s += Par();
			s += Exp1();
			s += ")";
			break;
		case T_times:
		case T_divide:
		case T_plus:
		case T_minus:
		case T_closeparen:
		case T_closesquare:
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			break;
		case T_eof: parsetree.drawepsilon(); break;
		default: syntax_error(NT_Exp1); break;
	}
	parsetree.pop();
	return s;
}

string parser_t::Par()
{
	//cout << "IN PAR" << endl;
	string s="";
	parsetree.push(NT_Par);
	switch (scanner.next_token())
	{
		case T_openparen:
			eat_token(T_openparen);
			s += "(";
			s += Expression();
			eat_token(T_closeparen);
			s += ")";
			if (scanner.next_token() == T_power)
			{
				s = "pow(" + s +",";
			}
			break;
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			s += "m[";
			s += Expression();
			eat_token(T_closesquare);
			s += "]";
			if (scanner.next_token() == T_power)
			{
					s = "pow(" + s +",";
			}
			break;
		case T_num: 
			eat_token(T_num);
			s += int_to_string(scanner.pop());
			if (scanner.next_token() == T_power)
			{
					s = "pow(" + s +",";
			}
			break;
		default: syntax_error(NT_Par); break;
	}
	parsetree.pop();
	return s;
}

string parser_t::int_to_string(int num)
{
	stringstream stream;
	stream << num;
	return stream.str();
}

/*** Main ***********************************************/

int main()
{
	cerr << "#include <stdio.h>\n#include <math.h>\n\nint main() {\n\n";
	cerr << "int m[101];\n";
	parser_t parser;
	parser.parse();
	//parser.emitCode();
	cerr << "\nreturn 0; \n }\n";
	return 0;
}
