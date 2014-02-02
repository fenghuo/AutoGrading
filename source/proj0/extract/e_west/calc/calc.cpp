#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <cmath>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/
//================================================================
//================================================================
//================================================================
//================================================================

#define MAX_SYMBOL_NAME_SIZE 25
#define MAX_NUMBER_VALUE 2147483646

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
	NT_S_prime,
	NT_Label,
	NT_Jump,
	NT_J_prime,
	NT_Assignment,
	NT_Print,
	NT_Expression,
	NT_E_prime,
	NT_Term,
	NT_T_prime,
	NT_Factor,
	NT_Exponent,
	NT_Base
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
	case epsilon: 
	  strncpy(buffer,"e",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Start: 
	  strncpy(buffer,"Start",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Statements:
	  strncpy(buffer, "Statements", MAX_SYMBOL_NAME_SIZE); break; 
	case NT_Statement:
	  strncpy(buffer, "Statement", MAX_SYMBOL_NAME_SIZE); break;
	case NT_S_prime:
	  strncpy(buffer, "S_prime", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Label:
	  strncpy(buffer, "Label", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Jump:
	  strncpy(buffer, "Jump", MAX_SYMBOL_NAME_SIZE); break;
	case NT_J_prime:
	  strncpy(buffer, "J_prime", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Assignment:
	  strncpy(buffer, "Assignment", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Print:
	  strncpy(buffer, "Print", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Expression:
	  strncpy(buffer, "Expression", MAX_SYMBOL_NAME_SIZE); break;
	case NT_E_prime:
	  strncpy(buffer, "E_prime", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Term:
	  strncpy(buffer, "Term", MAX_SYMBOL_NAME_SIZE); break;
	case NT_T_prime:
	  strncpy(buffer, "T_prime", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Factor:
	  strncpy(buffer, "Factor", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Exponent:
	  strncpy(buffer, "Exponent", MAX_SYMBOL_NAME_SIZE); break;
	case NT_Base:
	  strncpy(buffer, "Base", MAX_SYMBOL_NAME_SIZE); break;
	default: 
	  strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
	}
	return buffer;
}




/*** Scanner Class ***********************************************/
//================================================================
//================================================================
//================================================================

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
  //error for numbers that are too large
  void range_error();
  
  //WRITEME: this is a bogus member for implementing a useful stub, it should
  //be cut out once you get the scanner up and going.
  
  token_type next_token_value;
  bool next_token_valid; //whether next token has been set
  int currentLine;
  int linesToNextToken;
  
};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 

  //what we will do: if not already done, eat from the stream until 
  //we can identify the next token (do not eat it all). Then return this token, but keep 
  //track of what it was. If this is called again return it, if we eat it
  //then return it and unset
  //If we scanned over newlines when eating the stream, store the number
  //and then augment a global counter when we eat the tokens
  //If errors are encountered we use this global counter to identify the line

  token_type toReturn;
  int nextChar;
  if(!next_token_valid){
    linesToNextToken=0;
  }
  else{
    return next_token_value;
  }
  while(!next_token_valid){
    nextChar=cin.get();
    if(nextChar==-1){
      toReturn=T_eof;
      next_token_valid=true;
    }
    else if(nextChar==' ' || nextChar=='\t'){
      //gobble neglegible whitespace
      continue;
    }
    else if(nextChar=='\n'){
      linesToNextToken++;
      continue;
    }
    else{
      //expecting a token next
      //should be able to use one char and put back
      next_token_valid=true;
      if(nextChar >='0' && nextChar <= '9'){
	toReturn = T_num;
      }
      else if(nextChar == '+'){
	toReturn = T_plus;
      }
      else if(nextChar == '-'){
	toReturn = T_minus;
      }
      else if(nextChar == '*'){
	//need to peek another character
	if(cin.peek()=='*'){
	  toReturn = T_power;
	}
	else{
	  toReturn = T_times;
	}
      }
      else if(nextChar=='/'){
	toReturn = T_divide;
      }
      else if(nextChar=='='){
	toReturn = T_equals;
      }
      else if(nextChar=='('){
	toReturn = T_openparen;
      }
      else if(nextChar==')'){
	toReturn = T_closeparen;
      }
      else if(nextChar=='['){
	toReturn = T_opensquare;
      }
      else if(nextChar==']'){
	toReturn = T_closesquare;
      }
      else if(nextChar=='m'){
	toReturn = T_m;
      }
      else if(nextChar=='g'){
	toReturn = T_goto;
      }
      else if(nextChar=='i'){
	toReturn = T_if;
      }
      else if(nextChar=='p'){
	toReturn = T_print;
      }
      else if(nextChar=='L'){
	toReturn = T_label;
      }
      else if(nextChar==':'){
	toReturn = T_colon;
      }
      else{
	next_token_valid=false;
	//unexpected character, lex error
	scan_error(nextChar);
      }
    }
  }
  cin.putback(nextChar);
  next_token_value = toReturn;
  return toReturn;
}

void scanner_t::eat_token(token_type c)
{
  //if we are supposed to eat token c, and it does not match
  //what we are supposed to be reading from file, then it is a 
  //mismatch error ( call - mismatch_error(c) )
  
  if(! next_token_valid){
    next_token();
  }

  currentLine+=linesToNextToken;
  
  if( next_token_value != c){
    mismatch_error(c);
  }
  else{
    //success....so far
    //eat the rest of the token here
    char goto_expected[] = {'g','o','t','o'};
    char print_expected[] = {'p','r','i','n','t'};

    switch(c){
    case T_eof:
      cin.get();
      break;
    case T_num: {
      //omnomnom until we either find something not a digit
      //or get more than 10 digits (automatically too big)
      //if 9 digits, need to check against max value
      //we simply check for overflow in our int
      int value=0;
      int expo=0;
      char c=cin.get();
      while(c>='0' && c<='9'){
	int cValue = c-'0';
	value+=(cValue*pow(10,expo));
	expo++;
	c=cin.get();
      }
      cin.unget();
      if(value<0){
	//overflow, thus value too large
	range_error();
      }
      else{
	//success
      }
    }
      break;
    case T_plus:
    case T_minus:
    case T_times:
    case T_divide:
    case T_power:
    case T_equals:
    case T_openparen:
    case T_closeparen:
    case T_opensquare:
    case T_closesquare:
    case T_m:
    case T_colon:
    case T_label:
      //one-character tokens, checks already been
      //performed in next_token, simply eat and return
      cin.get();
      break;
    case T_goto:
      for(int i=0; i<4; ++i){
	char c = cin.get();
	if(c!=goto_expected[i]){
	  //error!
	  scan_error(c);
	  break;
	}
      }
      //success!
      break;
    case T_if:
      char c;
      if((c=cin.get())!='i'){
	//error
	scan_error(c);
      } 
      else if((c=cin.get())!='f'){
	//error
	scan_error(c);
      }
      //success
      break;
    case T_print:
      for(int i=0; i<5; ++i){
	char c = cin.get();
	if(c!=print_expected[i]){
	  //error!
	  scan_error(c);
	  break;
	}
      }
      //success!
      break;
    default:
      //wtf error
      cerr<<"eat_token() passed unexpected token type"<<endl;
      exit(1);
      break;
    }
    next_token_valid=false;
  }
  cerr << token_to_string(c)<<" ";
}

scanner_t::scanner_t()
{
  next_token_valid=false;
  currentLine=1;
  linesToNextToken=0;
}

int scanner_t::get_line()
{
  return currentLine;
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
  printf("range error: found integer with value exceeding 2147483646 - line %d\n",
	 get_line());
  exit(1);
}


/*** ParseTree Class **********************************************/
//================================================================
//================================================================
//================================================================


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
//================================================================
//================================================================
//================================================================
//================================================================

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
  void S_prime();
  void Label();
  void Jump();
  void J_prime();
  void Assignment();
  void Print();
  void Expression();
  void E_prime();
  void Term();
  void T_prime();
  void Factor();
  void Exponent();
  void Base();
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

void parser_t::Start()
{
  //start here
  parsetree.push(NT_Start);
  Statements();
  parsetree.pop();
}

void parser_t::Statements()
{
  parsetree.push(NT_Statements);
  Statement();
  S_prime();
  parsetree.pop();
}

void parser_t::S_prime()
{
  parsetree.push(NT_S_prime);
  //stuff
  switch(scanner.next_token()){
  case T_eof:
    eat_token(T_eof);
    parsetree.drawepsilon();
    break;
  case T_label:
  case T_goto:
  case T_m:
  case T_print:
    Statements();
    break;
  default:
    syntax_error(NT_S_prime);
    break;
  }
  parsetree.pop();
}

void parser_t::Statement()
{
  parsetree.push(NT_Statement);
  switch(scanner.next_token()){
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
  }
  parsetree.pop();
}

void parser_t::Label()
{
  parsetree.push(NT_Label);
  eat_token(T_label);
  eat_token(T_num);
  eat_token(T_colon);
  parsetree.pop();
}

void parser_t::Jump()
{
  parsetree.push(NT_Jump);
  eat_token(T_goto);
  eat_token(T_num);
  J_prime();
  parsetree.pop();
}

void parser_t::J_prime()
{
  parsetree.push(NT_J_prime);
  switch(scanner.next_token()){
  case T_if:
    eat_token(T_if);
    Expression();
    break;
  case T_label:
  case T_goto:
  case T_print:
  case T_m:
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_J_prime);
    break;
  }
  parsetree.pop();
}

void parser_t::Assignment()
{
  parsetree.push(NT_Assignment);
  eat_token(T_m);
  eat_token(T_opensquare);
  Expression();
  eat_token(T_closesquare);
  eat_token(T_equals);
  Expression();
  parsetree.pop();
}

void parser_t::Print()
{
  parsetree.push(NT_Print);
  eat_token(T_print);
  Expression();
  parsetree.pop();
}

void parser_t::Expression(){
  parsetree.push(NT_Expression);
  if(scanner.next_token()==T_openparen && false){
    eat_token(T_openparen);
    Expression();
    eat_token(T_closeparen);
  }
  else{
    Term();
    E_prime();
  }
  parsetree.pop();
}

void parser_t::E_prime()
{
  parsetree.push(NT_E_prime);
  switch(scanner.next_token()){
  case T_plus:
    eat_token(T_plus);
    Term();
    E_prime();
    break;
  case T_minus:
    eat_token(T_minus);
    Term();
    E_prime();
    break;
  case T_label:
  case T_goto:
  case T_print:
  case T_m:
  case T_eof:
  case T_closeparen:
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_E_prime);
    break;
  }
  parsetree.pop();
}

void parser_t::Term()
{
  parsetree.push(NT_Term);
  Factor();
  T_prime();
  parsetree.pop();
}

void parser_t::T_prime()
{
  parsetree.push(NT_T_prime);
  switch(scanner.next_token()){
  case T_times:
    eat_token(T_times);
    Factor();
    T_prime();
    break;
  case T_divide:
    eat_token(T_divide);
    Factor();
    T_prime();
    break;
  case T_num:
  case T_m:
  case T_openparen:
  case T_plus:
  case T_minus:
  case T_label:
  case T_goto:
  case T_print:
  case T_eof:
  case T_closeparen:
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_T_prime);
    break;
  }
  parsetree.pop();
}

void parser_t::Factor(){
  parsetree.push(NT_Factor);
  if(scanner.next_token() == T_openparen && false){
    eat_token(T_openparen);
    Expression();
    eat_token(T_closeparen);
  }
  else{
    Base();
    Exponent();
  }
  parsetree.pop();
}

void parser_t::Exponent()
{
  parsetree.push(NT_Exponent);
  switch(scanner.next_token()){
  case T_power:
    eat_token(T_power);
    Base();
    Exponent();
    break;
  case T_num:
  case T_m:
  case T_openparen:
  case T_plus:
  case T_minus:
  case T_label:
  case T_goto:
  case T_print:
  case T_eof:
  case T_divide:
  case T_times:
  case T_closeparen:
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Exponent);
    break;
  }
  parsetree.pop();
}

void parser_t::Base()
{
  parsetree.push(NT_Base);
  switch(scanner.next_token()){
  case T_num:
    eat_token(T_num);
    break;
  case T_m:
    eat_token(T_m);
    eat_token(T_opensquare);
    Expression();
    eat_token(T_closesquare);
    break;
  case T_openparen:
    eat_token(T_openparen);
    Expression();
    eat_token(T_closeparen);
  case T_plus:
  case T_minus:
  case T_label:
  case T_goto:
  case T_print:
  case T_eof:
  case T_divide:
  case T_times: 
  case T_power:
  case T_closeparen:
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Base);
    break;
  }
  parsetree.pop();
}

/*
//Here is an example
void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);

	switch( scanner.next_token() ) 
	{
		case T_plus:
			eat_token(T_plus);
			Start();
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
}
*/



//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

void testScanner(bool print){
  scanner_t scanner;
  token_type t;
  while((t=scanner.next_token())!=T_eof){
    scanner.eat_token(t);
    if (print) cout<<token_to_string(t)<<" ";
  }
  if (print) cout<<token_to_string(t)<<endl;
}

int main()
{
  //testScanner(false);
  
  parser_t parser;
  parser.parse();
  return 0;
}
