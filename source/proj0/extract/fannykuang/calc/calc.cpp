#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <string>
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
  NT_Expression,
  //WRITEME: add symbolic names for you non-terminals here
  NT_Label,
  NT_Goto,
  NT_Print,
  NT_Assign,
  NT_Paren,
  NT_States,
  NT_State,
  NT_B,
  NT_C,
  NT_D,

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
    //WRITEME: add the other nonterminals you need here		
  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Goto: strncpy(buffer,"Goto",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Assign: strncpy(buffer,"Assign",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Paren: strncpy(buffer,"Paren",MAX_SYMBOL_NAME_SIZE); break;
  case NT_States: strncpy(buffer,"States",MAX_SYMBOL_NAME_SIZE); break;
  case NT_State: strncpy(buffer,"State",MAX_SYMBOL_NAME_SIZE); break;
  case NT_B: strncpy(buffer,"B",MAX_SYMBOL_NAME_SIZE); break;
  case NT_C: strncpy(buffer,"C",MAX_SYMBOL_NAME_SIZE); break;
  case NT_D: strncpy(buffer,"D",MAX_SYMBOL_NAME_SIZE); break;

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

  // get current cin character
  char getCurrChar();

private:

  //WRITEME: Figure out what you will need to write the scanner
  //and to implement the above interface.  It does not have to
  //be a state machine or anything fancy.  Just read in the
  //characters one at a time (using getchar would be a good way)
  //and group them into tokens.  
  int currentLine;
  char currChar;

  //error message and exit for unrecognized character
  void scan_error(char x);
  //error message and exit for token mismatch
  void mismatch_error(token_type c);

  //WRITEME: this is a bogus member for implementing a useful stub, it should
  //be cut out once you get the scanner up and going.
  token_type bogo_token;

};

char scanner_t::getCurrChar() {
	return currChar;
}

token_type scanner_t::next_token()
{
  //WRITEME: replace this bogus junk with code that will take a peek
  //at the next token and return it to the parser.  It should _not_
  //actually consume a token - you should be able to call next_token()
  //multiple times without actually reading any more tokens in 
  
    switch( cin.peek() ) {
    case EOF:  return T_eof; break;
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
      return T_num; break;
    case '+': return T_plus; break;
    case '-': return T_minus; break;
    case '/': return T_divide; break;
    case '=': return T_equals; break;
    case '(': return T_openparen; break;
    case ')': return T_closeparen; break;
    case '[': return T_opensquare; break;
    case ']': return T_closesquare; break;
    case 'm': return T_m; break;
    case 'L': return T_label; break;
    case ':': return T_colon; break;
      // multi-char tokens 
    case '*': 
      cin.get();
      if (cin.peek() == '*') {
         cin.putback('*');
	     return T_power;
         }
      else {
           cin.putback('*');
           return T_times;
           }
      break;
    case 'g':
      cin.get();
      if (cin.peek()=='o') {
         cin.get();
         if (cin.peek()=='t') {
            cin.get();
            if (cin.peek()=='o') {
               cin.putback('t');
               cin.putback('o');
               cin.putback('g');
               return T_goto;
            }
            else {
                 cin.putback('t');
                 }
         }
         else {
              cin.putback('o');
              }
      }
      else {
           cin.putback('g');
           }
      break;
    case 'i':
      cin.get();
      if (cin.peek() == 'f') {
         cin.putback('i');
         return T_if;
         }
      else 
           cin.putback('i');
      break;
    case 'p':
      cin.get();
      if (cin.peek() == 'r'){
	     cin.get();
         if (cin.peek() == 'i'){
	        cin.get();
	        if (cin.peek() == 'n'){
    	        cin.get();
                if (cin.peek() == 't'){
                   cin.putback('n');
                   cin.putback('i');
                   cin.putback('r');
                   cin.putback('p');
    	           return T_print;
                }
                else {
    	        cin.putback('n');
                }
	        }
	        else {
	             cin.putback('i');
              }
         }
         else {
         	  cin.putback('r');
            }
         }
         else {
         cin.putback('p');
         }
      break;     
      // spaces and newlines and extras

    case '\n':
      cin.get();
      currentLine++;
      next_token();
      break;
    case ' ':
      cin.get();
      next_token();
      break;
    }
}

void scanner_t::eat_token(token_type c)
{
  //if we are supposed to eat token c, and it does not match
  //what we are supposed to be reading from file, then it is a 
  //mismatch error ( call - mismatch_error(c) )

  //WRITEME: cut this bogus stuff out and implement eat_token
	switch (c) {
	case T_eof: cin.get(); break;
	case T_num: currChar=(char)cin.get(); break;
	case T_plus: cerr << (char)cin.get(); break;
	case T_minus: cerr << (char)cin.get(); break;
	case T_times: cerr << (char)cin.get(); break;
	case T_divide: cerr << (char)cin.get(); break;
	case T_equals: cerr << (char)cin.get(); break;
	case T_openparen: cerr << (char)cin.get(); break;
	case T_closeparen: cerr << (char)cin.get(); break;
	case T_opensquare: cerr << (char)cin.get(); break;
	case T_closesquare: cerr << (char)cin.get(); break;
	case T_m: cerr << (char)cin.get(); break;
	case T_colon: cerr << (char)cin.get(); break;
	case T_label: cerr << (char)cin.get(); break;
		// multi-char tokens
	case T_power:
		for (int i = 0; i < 2; i++)
			cerr << (char)cin.get();
		break;
	case T_goto:
		for (int i = 0; i < 4; i++)
			cin.get();
    break;
  case T_if:
    for (int i = 0; i<2; i++)
		cerr << (char)cin.get();
    break;
  case T_print: 
    for (int i = 0; i<5; i++)
      cin.get();
	cerr << "cout << ";
    break;

  }

}

scanner_t::scanner_t()
{
  //WRITEME: this is the scanner constructor
  currentLine = 1;
}

int scanner_t::get_line()
{
  //WRITEME: this function should return the current line number (used
  //for error reporting
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
  void Label();
  void Goto();
  void Print();
  void Assign();
  void Expression();
  void Paren();
  void States();
  void State();
  void B();
  void C();
  void D();

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
	cerr << "#include<iostream>" << endl;
	cerr << "#include<cmath>" << endl;
	cerr << "using namespace std;" << endl;
	cerr << "int main() {" << endl;
	cerr << "int m[101];" << endl;
	States();

	cerr << "return 0;" << endl;
	cerr << "}";


	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here


void parser_t::States() {
  parsetree.push(NT_States);
  while (scanner.next_token() != T_eof) {
	  switch (scanner.next_token()) {
	  case T_label: case T_goto: case T_print: case T_m:
		  State();
		  break;
	  case T_eof:
		  parsetree.drawepsilon();
		  break;
	  default:
		  syntax_error(NT_States);
		  break;
	  }
  }
  eat_token(T_eof);
  parsetree.drawepsilon();
  parsetree.pop();
}

void parser_t::State() {
  parsetree.push(NT_State);
  switch (scanner.next_token()) {
  case T_label:
    Label();
	cerr << endl;
    break;
  case T_goto:
    Goto();
    break;
  case T_print:
    Print();
	cerr << ";" << endl;
    break;
  case T_m:
    Assign();
	cerr << ";" << endl;
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_State);
    break;
  }
  parsetree.pop();
}

void parser_t::Label() {
  parsetree.push(NT_Label);
  switch(scanner.next_token()) {
  case T_label:
    eat_token(T_label);
    Label();
    break;
  case T_num:
    eat_token(T_num);
	cerr << scanner.getCurrChar();
    Label();
    break;
  case T_colon:
    eat_token(T_colon);
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Label);
    break;
  }
  parsetree.pop();
}

void parser_t::Goto() {
	string n = "";
  parsetree.push(NT_Goto);
  switch (scanner.next_token()) {
  case T_goto:
	
    eat_token(T_goto);
    while (scanner.next_token()==T_num) {
          eat_token(T_num);
		  n += scanner.getCurrChar();
    }
    if (scanner.next_token() == T_if) {
      eat_token(T_if);
	  cerr << "(";
      Expression();
	  cerr << ")" << endl;
	  cerr << "goto L" << n << ";" << endl;
    }
	else 
		cerr << "goto L" << n << ";" << endl;
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Goto);
    break;
  }
  parsetree.pop();
}

void parser_t::Print() {
  parsetree.push(NT_Print);
  switch (scanner.next_token()) {
  case T_print:
    eat_token(T_print);
    Expression();
    break;
  default:
    syntax_error(NT_Print);
    break;
  }
  cerr << " << endl";
  parsetree.pop();
}

void parser_t::Expression() {
  parsetree.push(NT_Expression);
  switch (scanner.next_token()){
  case T_num:  case T_openparen: case T_m: 
    B();     
    //Expmini();
    break;
  case T_closesquare: 
    parsetree.drawepsilon();
    break;
  case T_closeparen:
    parsetree.drawepsilon();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Expression);
    break;
  }
  parsetree.pop();
}

/*
void parser_t::Expmini() {
  parsetree.push(NT_Expmini);
  switch (scanner.next_token()) {
  case T_plus:
  }
  parsetree.pop();
}*/


void parser_t::Assign() {
  parsetree.push(NT_Assign);
  switch (scanner.next_token()) {
  case T_m:
    eat_token(T_m);
    eat_token(T_opensquare);
    Expression();
    eat_token(T_closesquare);
    if(scanner.next_token()==T_equals) {
      eat_token(T_equals);
      Expression();
    }
    break;
  case T_eof:
    parsetree.drawepsilon();
   default:
    syntax_error(NT_Assign);
    break; 
  }
  parsetree.pop();
}
/*
void parser_t::A() {
  parsetree.push(NT_A);
  switch (scanner.next_token()) {
  case T_openparen: case T_m: case T_num:
    B();
    A();
    break;
  }
  parsetree.pop();
  }*/


void parser_t::B() {
  parsetree.push(NT_B);
  switch (scanner.next_token()) {
  case T_openparen: case T_m: case T_num:
    C();
    if (scanner.next_token()==T_plus || scanner.next_token()==T_minus || scanner.next_token()==T_times || scanner.next_token()==T_divide || scanner.next_token()==T_power)
       B();
    break;
  case T_plus:
    eat_token(T_plus);
    B();
    break;
  case T_minus:
    eat_token(T_minus);
    B();
    break;
  case T_closeparen:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_B);
    break;
  }
  parsetree.pop();
  }

void parser_t::C() {
  parsetree.push(NT_C);
  switch (scanner.next_token()) {
  case T_openparen: case T_m: case T_num:
    D();
    if (scanner.next_token()==T_plus || scanner.next_token()==T_minus || scanner.next_token()==T_times || scanner.next_token()==T_divide || scanner.next_token()==T_power)
       C();
    break;
  case T_times:
    eat_token(T_times);
    C();
    break;
  case T_divide:
    eat_token(T_divide);
    C();
    break;
  case T_plus:
    parsetree.drawepsilon();
    break;
  case T_minus:
    parsetree.drawepsilon();
    break;
  case T_closeparen:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_C);
    break;
  }
  parsetree.pop();
}

void parser_t::D() {
  parsetree.push(NT_C);
  switch (scanner.next_token()) {
  case T_openparen: case T_num: case T_m:
		  Paren();
    if (scanner.next_token()==T_plus || scanner.next_token()==T_minus || scanner.next_token()==T_times || scanner.next_token()==T_divide || scanner.next_token()==T_power)
       D();
    break;
  case T_power:
    eat_token(T_power);
    D();
    break;
  case T_times:
    parsetree.drawepsilon();
    break;
  case T_divide:
    parsetree.drawepsilon();
    break;
  case T_plus:
    parsetree.drawepsilon();
    break;
  case T_minus:
    parsetree.drawepsilon();
    break;
  case T_closeparen:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_D);
    break;
  }
  parsetree.pop();
}

void parser_t::Paren() {
  parsetree.push(NT_Paren);
  switch (scanner.next_token()) {
  case T_openparen: 
    eat_token(T_openparen);
    Expression();
    eat_token(T_closeparen);
    if (scanner.next_token()==T_plus || scanner.next_token()==T_minus || scanner.next_token()==T_times || scanner.next_token()==T_divide || scanner.next_token()==T_power)
        Paren();
    break;
  case T_m:
    eat_token(T_m);
    eat_token(T_opensquare);
    Expression();
    eat_token(T_closesquare);
    if (scanner.next_token()==T_plus || scanner.next_token()==T_minus || scanner.next_token()==T_times || scanner.next_token()==T_divide || scanner.next_token()==T_power)
       Paren();
    break;
  case T_num:
    while(scanner.next_token()==T_num)
      {
		eat_token(T_num);
		cerr << scanner.getCurrChar();
		}
    if (scanner.next_token()==T_plus || scanner.next_token()==T_minus || scanner.next_token()==T_times || scanner.next_token()==T_divide || scanner.next_token()==T_power)
       Paren();
    break;
  case T_power:
    parsetree.drawepsilon();
    break;
  case T_times:
    parsetree.drawepsilon();
    break;
  case T_divide:
    parsetree.drawepsilon();
    break;
  case T_plus:
    parsetree.drawepsilon();
    break;
  case T_minus:
    parsetree.drawepsilon();
    break;
  case T_closeparen:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Paren);
    break;
  }
  parsetree.pop();
}





/*** Main ***********************************************/

int main()
{

  /*scanner_t scanner;
  token_type c;
  c = scanner.next_token();
  while (c!=T_eof) {
    cout << token_to_string(c);
    scanner.eat_token(c);
    c = scanner.next_token();
  }
  cout << token_to_string(c);*/
  

  
  parser_t parser;
  parser.parse();
  return 0;
}
