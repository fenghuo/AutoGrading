/*
  Sam Dacanay (4893582) for CS160
  Coding Buddy: Nedda Amini
  1/22/14
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <string>
#include <sstream>
#include <math.h>
#include <iostream>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25

//all of the terminals in the language
typedef enum {
	T_eof = 0,			// 0: end of file
	T_num,				// 1: numbers
	T_plus,				// 2: +
	T_minus,				// 3: -
	T_times,				// 4: *
	T_divide,				// 5: /
	T_power, 				// 6: **
	T_equals,				// 7: =
	T_openparen,			// 8: (
	T_closeparen, 			// 9: )
	T_opensquare,			// 10: [
	T_closesquare,			// 11: ]
	T_m,					// 12: m
	T_goto,				// 13: goto
	T_if,				// 14: if
	T_print,				// 15: print
	T_label,				// 16: L
	T_colon,				// 17: :
	T_space,
	T_newline

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
		case T_minus:		strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
		case T_times:		strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
		case T_divide:		strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
		case T_power:		strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
		case T_equals:		strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
		case T_openparen:	strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
    		case T_closeparen:	strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
   	  	case T_opensquare:	strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
  	   	case T_closesquare:	strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
 	    	case T_m:	       	strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
 	    	case T_goto:		strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
 	    	case T_if:		strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
  	   	case T_print:		strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
  	   	case T_label:		strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
  	   	case T_colon:		strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
	        case T_space:           strncpy(buffer," ",MAX_SYMBOL_NAME_SIZE); break;
	        case T_newline:         strncpy(buffer,"\n",MAX_SYMBOL_NAME_SIZE); break;
		default: 	       	strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
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
	//WRITEME: add symbolic names for you non-terminals here
	NT_PARENTHESES,
	NT_EXPONENT,
	NT_EXPRESSION,
	NT_TERM,
	NT_TERMINAL,
	NT_JUMP
		
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
		  case NT_EXPRESSION: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  //WRITEME: add the other nonterminals you need here		
		  case NT_PARENTHESES: strncpy(buffer,"Parentheses",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_EXPONENT: strncpy(buffer,"Exponent",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_TERM: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_TERMINAL: strncpy(buffer,"Terminal",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_JUMP: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
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
	
  int line_num;

private:

  //WRITEME: Figure out what you will need to write the scanner
  //and to implement the above interface.  It does not have to
  //be a state machine or anything fancy.  Just read in the
  //characters one at a time (using getchar would be a good way)
  //and group them into tokens.
  token_type get_tokens(char input);  

  //error message and exit for unrecognized character
  void scan_error(char x);
  //error message and exit for token mismatch
  void mismatch_error(token_type c);
	
};

token_type scanner_t::get_tokens(char token_char_current) {
	
  char powerchar;

  char printchar1;
  char printchar2;
  char printchar3;
  char printchar4;
  char printchar5;
	
  char gotochar1;
  char gotochar2;
  char gotochar3;
	
  char ifchar;
 	
  if(token_char_current == EOF) {
    //printf(" eoftoken ");
    return T_eof;
  }
  else if (token_char_current == '0' || token_char_current == '1' ||
	   token_char_current == '2' || token_char_current == '3' ||
	   token_char_current == '4' || token_char_current == '5' ||
	   token_char_current == '6' || token_char_current == '7' ||
	   token_char_current == '8' || token_char_current == '9') {
    //printf(" numtoken ");
    return T_num;
  }
  else if(token_char_current == '+') {
    //printf(" plustoken ");
    return T_plus;
  }
  else if(token_char_current == '-') {
    //printf(" minustoken ");
    return T_minus;
  }
  else if(token_char_current == '/') {
    //printf(" dividetoken ");
    return T_divide;
  }
  else if(token_char_current == '*') {
    powerchar = getchar();
    if(powerchar == '*') {
      ungetc(powerchar, stdin);
      //printf(" powertoken ");
      return T_power;
    }
    else {
      ungetc(powerchar, stdin);
      //printf(" timestoken ");
      return T_times;
    }
  }
  else if(token_char_current == '=') {
    //printf(" equalstoken ");
    return T_equals;
  }
  else if(token_char_current == '(') {
    //printf(" openparen ");
    return T_openparen;
  }
  else if(token_char_current == ')') {
    //printf(" closeparen ");
    return T_closeparen;
  }
  else if(token_char_current == '[') {
    //printf(" opensquare ");
    return T_opensquare;
  }
  else if(token_char_current == ']') {
    //printf(" closesquare ");
    return T_closesquare;
  }
  else if(token_char_current == 'm') {
    //printf(" mtoken ");
    return T_m;
  }
  else if(token_char_current == 'g'){
    gotochar1 = getchar();
    if(gotochar1 == 'o') {
      gotochar2 = getchar();
      if(gotochar2 == 't') {
	gotochar3 = getchar();
	if(gotochar3 == 'o') {
	  ;
	}
	ungetc(gotochar3, stdin);
      }
      ungetc(gotochar2, stdin);
    }
    ungetc(gotochar1, stdin);
    // printf(" gototoken ");
    return T_goto;
  }
  else if(token_char_current == 'i') {
    ifchar = getchar();
    if(ifchar == 'f') {
      ;
    }
    ungetc(ifchar, stdin);
    //printf(" iftoken ");
    return T_if;
  }
  else if(token_char_current == 'p') {
    printchar1 = getchar();
    if(printchar1 = 'r') {
      printchar2 = getchar();
      if(printchar2 = 'i') {
	printchar3 = getchar();
	if(printchar3 = 'n') {
	  printchar4 = getchar();
	  if(printchar4 = 't') {
	    ;
	  }
	  ungetc(printchar4, stdin);
	}
	ungetc(printchar3, stdin);
      }
      ungetc(printchar2, stdin);
    }
    ungetc(printchar1, stdin);
    //printf(" printtoken ");
    return T_print;
  }	
  else if(token_char_current == 'L') {
    //printf(" labeltoken ");
    return T_label;
  }
  else if(token_char_current == ':') {
    // printf(" colontoken ");
    return T_colon;
  }
  else if(token_char_current == ' ') {
    //printf(" spacetoken ");
    return T_space;
  }
  else if(token_char_current == '\n') {
    //printf(" newlinetoken ");
    return T_newline;
  }
	
}

token_type scanner_t::next_token() {

  char next;
  token_type check_next;
	
  next = getchar();
  check_next = get_tokens(next);
	
  ungetc(next, stdin);
  return check_next;
	
}
	


void scanner_t::eat_token(token_type c)
{
  //if we are supposed to eat token c, and it does not match
  //what we are supposed to be reading from file, then it is a 
  //mismatch error ( call - mismatch_error(c) )
  char charToEat;
  token_type tokenToEat;
	
  charToEat = getchar();
  tokenToEat = get_tokens(charToEat);

  //i'm pretty sure we just need to eat tokens that are longer than 
  //one character. and we can just 'forget' to ungetc the stream. yeah.
  if(tokenToEat != c)
    mismatch_error(c);
	
  switch(tokenToEat) {
  case T_goto:
    for (int i = 0; i < 3; i++) {
      charToEat = getchar();
    }
    break;
  case T_if: 
    getchar(); 
    break;
  case T_print: 
    for (int i = 0; i < 4; i++) {
      getchar();
    }
    break;
  }
	
  

}

scanner_t::scanner_t()
{
  line_num = 0;
  while(next_token() != T_eof) {
    printf(token_to_string(next_token()));
    eat_token(next_token());
  }
}

int scanner_t::get_line()
{
  //WRITEME: this function should return the current line number (used
  //for error reporting
  /* char line_num_input;
  token_type line_num_input_token;
	
  line_num_input = getchar();
  line_num_input_token = get_tokens(line_num_input);
	
  ungetc(line_num_input, stdin);
	
  return line_num; */
  int i;
  if(next_token() == T_newline)
    i++;

  return i;

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

//-----------------------------------------------------------------/
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

//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

int main()
{
  //parser_t parser;
  //parser.parse();
  scanner_t scanner;
  return 0;
}
