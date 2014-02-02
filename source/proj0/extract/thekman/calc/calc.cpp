
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>


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
	NT_Expression,
	NT_E1,
	NT_T,
	NT_T1,
	NT_F,
	NT_F1,
	NT_final
	
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
	case NT_E1: strncpy(buffer,"E1",MAX_SYMBOL_NAME_SIZE); break;
	case NT_T: strncpy(buffer,"T",MAX_SYMBOL_NAME_SIZE); break;
	case NT_T1: strncpy(buffer,"T1",MAX_SYMBOL_NAME_SIZE); break;
	case NT_F: strncpy(buffer,"F",MAX_SYMBOL_NAME_SIZE); break;
	case NT_F1: strncpy(buffer,"F1",MAX_SYMBOL_NAME_SIZE); break;
	case NT_final: strncpy(buffer,"final",MAX_SYMBOL_NAME_SIZE); break;
		  default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
		}
	return buffer;
}

/*** Scanner Class ***********************************************/

class scanner_t {
  public:

  vector<int> numStorage;

	//eats the next token and prints an error if it is not of type c
	void eat_token(token_type c);

        //checks if should throw mismatch error during parsing, triggers mismatch_error in parser
        int eat_checker(token_type c);

	//peeks at the lookahead token
	token_type next_token();

	//return line number for errors
	int get_line();

	//constructor 
	scanner_t();

  private:
  
  vector<char> inputStorage;
  vector<token_type> tokenStorage;
  
  vector<char> tempNumStorage;

  int lineNumber;
  string tempNum;
  long long int tempNumi;

  //WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  

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
	//multiple times without actually reading any more tokens

  return tokenStorage[0];
  
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

  if(c != tokenStorage[0]) {
    mismatch_error(c);
  }
  else
    tokenStorage.erase (tokenStorage.begin());

}

int scanner_t::eat_checker(token_type c)
{
  if(c != tokenStorage[0]) {
    return 1;
  }
  else return 0;
}

scanner_t::scanner_t()
{
  int c = 0;
  lineNumber = 1;
  char x;
  stringstream ss;

  while (c != -1) {
    c = getchar();
    inputStorage.push_back(c);
  }
  for (int i=0; i < inputStorage.size()-1; i++) {
    x = inputStorage[i];

    //##################################   INPUT -> TOKEN BLOCK   #############################
    if (x == '+') {tokenStorage.push_back(T_plus); continue;}
    if (x == '-') {tokenStorage.push_back(T_minus); continue;}
    if (x == '*') {
      if (inputStorage[i+1] == '*') {
	tokenStorage.push_back(T_power);
	i++;
	continue;
      }
      else {tokenStorage.push_back(T_times);}
      continue;
    }
    if (x == '/') {tokenStorage.push_back(T_divide); continue;}
    if (x == '=') {tokenStorage.push_back(T_equals); continue;}
    if (x == '(') {tokenStorage.push_back(T_openparen); continue;}
    if (x == ')') {tokenStorage.push_back(T_closeparen); continue;}
    if (x == '[') {tokenStorage.push_back(T_opensquare); continue;}
    if (x == ']') {tokenStorage.push_back(T_closesquare); continue;}
    if (x == 'm') {tokenStorage.push_back(T_m); continue;}
    if (x == 'g') {
      if (inputStorage[i+1] == 'o') {
	if (inputStorage[i+2] == 't') {
	  if (inputStorage[i+3] == 'o') { 
	    tokenStorage.push_back(T_goto);
	    i+=3;
	    continue;
	  }
	  else scan_error(inputStorage[i+3]);
	}
	else scan_error(inputStorage[i+2]);
      }
      else scan_error(inputStorage[i+1]);
    }
    if (x == 'i') {
      if (inputStorage[i+1] == 'f') {
	tokenStorage.push_back(T_if);
       	i++;
	continue;
      }
      else scan_error(inputStorage[i+1]);
    }
    if (x == 'p') {
      if (inputStorage[i+1] == 'r') {
	if (inputStorage[i+2] == 'i') {
	  if (inputStorage[i+3] == 'n') {
	    if (inputStorage[i+4] == 't') {
	      tokenStorage.push_back(T_print);
	      i = i + 4;
	      continue;
	    }
	    else scan_error(inputStorage[i+4]);
	  }
	  else scan_error(inputStorage[i+3]);
	}
	else scan_error(inputStorage[i+2]);
      }
      else scan_error(inputStorage[i+1]);
    }
    if (x == 'L') {tokenStorage.push_back(T_label); continue;}
    if (x == ':') {tokenStorage.push_back(T_colon); continue;}
    if (isdigit(x)) {
      tokenStorage.push_back(T_num);
      tempNumStorage.push_back(x);
      while(isdigit(inputStorage[i+1])) {
	tempNumStorage.push_back(x);
	i++;
      }
      tempNum = "";
      for (int i = 0; i < tempNumStorage.size(); i++) {
	tempNum += tempNumStorage[i];
      }
      tempNumStorage.clear();
      ss<<tempNum;
      ss>>tempNumi;
      ss.str("");
      ss.clear();
      if (tempNumi > 2147483647 || tempNumi < 0) {
	cout << "range error: input number must be within range 2^31-1.  Line " << get_line();
	cout << '\n';
	exit(1);
      }
      numStorage.push_back(tempNumi);
      continue;
    }
    if (x == '\n') {
      lineNumber++;
      continue;
    }
    if (x == ' ') {continue;}
    else scan_error(inputStorage[i]);
    
    //################################## END INPUT -> TOKEN BLOCK   #############################
  } //end input -> token for loop
  
  tokenStorage.push_back(T_eof);

}

int scanner_t::get_line()
{
  return lineNumber;
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
  void mismatch_error(token_type x);

	void Start();
  void Statements();
  void Statement();
  void Expression();
  void E1();
  void T();
  void T1();
  void F();
  void F1();
  void final();
  
  int tempInt;
  int lineNumber;

  public:	
	void parse();
};


//this function not only eats the token (moving the scanner forward one
//token), it also makes sure that token is drawn in the parse tree 
//properly by calling push and pop.
void parser_t::eat_token(token_type t)
{
	parsetree.push(t);
	if (scanner.eat_checker(t) == 1) {
	  mismatch_error(t);
	}
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
	       //scanner.get_line() ); 
	       lineNumber);
	exit(1); 
}

void parser_t::mismatch_error (token_type x)
{
	printf("syntax error: found %s ",token_to_string(scanner.next_token()) );
	printf("expecting %s - line %d\n", token_to_string(x), lineNumber);
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
//descent code.

//Here is an example
void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);
	lineNumber = 0;
	Statements();

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Statements()
{
  parsetree.push(NT_Statements);
  if (scanner.next_token() == T_eof) {
    parsetree.drawepsilon();
  }
  else {
    //push maybe here
    Statement();
    Statements();
  }
  parsetree.pop();
}

void parser_t::Statement() {
  parsetree.push(NT_Statement);
  lineNumber++;
  switch(scanner.next_token()) {
  case T_label:
    eat_token(T_label);
    cerr << "L";
    eat_token(T_num);
    cerr << scanner.numStorage[0];
    scanner.numStorage.erase(scanner.numStorage.begin());
    eat_token(T_colon);
    cerr << ";";
    break;
  case T_goto:
    eat_token(T_goto);
    eat_token(T_num);
    tempInt = scanner.numStorage[0];
    scanner.numStorage.erase(scanner.numStorage.begin());
    if (scanner.next_token() == T_if) {
      eat_token(T_if);
      cerr << "if(";
      Expression();
      cerr << ")";
      cerr << "goto " << tempInt;
      break;
    }
    cerr << "goto " << tempInt << ";";
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
    Expression();
    break;
  default:
    syntax_error(NT_Statement);
    break;
  }
  parsetree.pop();
}

void parser_t::Expression() {
  parsetree.push(NT_Expression);
  if (scanner.next_token() != T_openparen && scanner.next_token() != T_m && scanner.next_token() != T_num){
    syntax_error(NT_Expression);
  }
  T();
  E1();

  parsetree.pop();
}

void parser_t::E1() {
  parsetree.push(NT_E1);

  switch(scanner.next_token()) {
  case T_plus:
    eat_token(T_plus);
    T();
    E1();
    break;
  case T_minus:
    eat_token(T_minus);
    T();
    E1();
    break;
  
  }
  parsetree.drawepsilon();
  parsetree.pop();
}

void parser_t::T() {
  parsetree.push(NT_T);
  if (scanner.next_token() != T_openparen && scanner.next_token() != T_m && scanner.next_token() != T_num){
    syntax_error(NT_T);
  }
  F();
  T1();

  parsetree.pop();
}

void parser_t::T1() {
  parsetree.push(NT_E1);
  
  switch(scanner.next_token()) {
  case T_times:
    eat_token(T_times);
    F();
    T1();
    break;
  case T_divide:
    eat_token(T_divide);
    F();
    T1();
    break;
  }
  parsetree.drawepsilon();
  parsetree.pop();
}

void parser_t::F() {
  parsetree.push(NT_F);
  if (scanner.next_token() != T_openparen && scanner.next_token() != T_m && scanner.next_token() != T_num){
    syntax_error(NT_F);
  }
  final();
  F1();

  parsetree.pop();
}

void parser_t::F1() {
  parsetree.push(NT_F1);
  
  if (scanner.next_token() == T_power) {
    eat_token(T_power);
    final();
    F1();
  }
  parsetree.drawepsilon();
  parsetree.pop();
}

void parser_t::final() {
  parsetree.push(NT_final);
  
  switch(scanner.next_token()) {
  case T_openparen:
    eat_token(T_openparen);
    Expression();
    eat_token(T_closeparen);
    break;
  case T_m:
    eat_token(T_m);
    eat_token(T_opensquare);
    Expression();
    eat_token(T_closesquare);
    break;
  case T_num:
    eat_token(T_num);
    break;
  default:
    syntax_error(NT_final);
    break;
  }
  
  parsetree.pop();
}
    
  



//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

int main()
{
  parser_t parser;
  parser.parse();
  return 0;
}
