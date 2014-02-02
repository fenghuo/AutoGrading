//David Truong
//5843115
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>

using namespace std;

/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/
#define MAX_SYMBOL_NAME_SIZE 25
//all of the terminals in the language
typedef enum {
  T_eof = 0,			// 0: end of file
  T_num,	      	      	// 1: numbers
  T_plus,      			// 2: +
  T_minus,	       		// 3: -
  T_times,     			// 4: *
  T_divide,			// 5: /
  T_power, 			// 6: **
  T_equals,			// 7: =
  T_openparen,		        // 8: (
  T_closeparen,                 // 9: )
  T_opensquare,	                // 10: [
  T_closesquare,                // 11: ]
  T_m,				// 12: m
  T_goto,    			// 13: goto
  T_if,			       	// 14: if
  T_print,	       		// 15: print
  T_label,   			// 16: L
  T_colon,                      // 17: :
  T_newline                     // 18: \n
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
  case T_minus:	        strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
  case T_times:   	strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
  case T_divide:	strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
  case T_power:	        strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
  case T_equals:	strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
  case T_openparen:    	strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
  case T_closeparen:   	strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
  case T_opensquare:   	strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
  case T_closesquare:	strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
  case T_m:	       	strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
  case T_goto:		strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
  case T_if:	       	strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
  case T_print:	        strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
  case T_label:	        strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
  case T_colon:	        strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
  case T_newline:	strncpy(buffer,"newline",MAX_SYMBOL_NAME_SIZE); break;
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
  NT_Expression,
  NT_Statements,
  NT_Statement,
  NT_Label,
  NT_Jump,
  NT_JumpCheck,
  NT_Assignment,
  NT_Print,
  NT_Expression2,
  NT_Term,
  NT_Term2,
  NT_Power,
  NT_Power2,
  NT_Factor
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
  case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
  case NT_JumpCheck: strncpy(buffer,"JumpCheck",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Expression2: strncpy(buffer,"Expression2",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Term2: strncpy(buffer,"Term2",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Power: strncpy(buffer,"Power",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Power2: strncpy(buffer,"Power2",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
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
  int lineNumber;
  token_type peekToken;
  bool tokenEaten;
  bool inLabel;
  bool inGoto;
  bool inPrint;
  bool inIf;
};

token_type scanner_t::next_token()
{
  //WRITEME: replace this bogus junk with code that will take a peek
  //at the next token and return it to the parser.  It should _not_
  //actually consume a token - you should be able to call next_token()
  //multiple times without actually reading any more tokens in       
  if (tokenEaten) //only get next token if you have eaten
       return peekToken;

  int t = getchar();
  switch(t)
    {     
    case EOF:
      peekToken = T_eof;
      if (inLabel)
	{
	  inLabel = false;
	  break;
	}
      else
	cerr << ";" << endl;
      break;
    case '+': cerr << "+"; peekToken = T_plus; break;
    case '-': cerr << "-"; peekToken = T_minus; break;
    case '*':
      {
	peekToken = T_times; //default to times
	int y = getchar();   //get next char after asterisk
	if (y == '*')
	  {
	    cerr << "**";
	    peekToken = T_power;
	    break;
	  }
	else
	  {
	    cerr << "*";
	    ungetc(y,stdin); //if next symbol is not * put back into stream
	    break;
	  }
      }
    case '/': cerr << "/"; peekToken = T_divide; break;
    case '=': cerr << "="; peekToken = T_equals; break;
    case '(':cerr << "("; peekToken = T_openparen; break;
    case ')': cerr << ")"; peekToken = T_closeparen;break;
    case '[': cerr << "["; peekToken = T_opensquare; break;
    case ']':
      cerr << "]";
      if (inPrint)
	{
	  inPrint = false;
	  cerr << ")";
	}
      if (inIf)
	{
	  inIf = false;
	  cerr << ")";
	}
      peekToken = T_closesquare; 
      break;
    case 'm': cerr << "m"; peekToken = T_m; break;
    case 'g':
      {
	cerr << "g";
	int a = getchar();
	if (a == 'o')
	  {
	    cerr << "o";
	    int b = getchar();
	    if (b == 't')
	      {
		cerr << "t";
		int c = getchar();
		if(c == 'o')
		  {
		    cerr << "o L";
		    peekToken = T_goto;
		    inGoto = true;
		    break;
		  }
		else
		  scan_error(c);
	      }
	    else
	      scan_error(b);
	  }
	else
	  scan_error(a);
      }
    case 'i':
      {
	cerr << "i";
	int d = getchar();
	if (d == 'f')
	  {
	    cerr << "f (";
	    peekToken = T_if;
	    inIf = true;
	    break;
	  }
	else
	  scan_error(d);
      }
    case 'p':
      {
	cerr << "p";
	int e = getchar();
	if (e == 'r')
	  {
	    cerr << "r";
	    int f = getchar();
	    if (f == 'i')
	      {
		cerr << "i";
		int g = getchar();
		if (g == 'n')
		  {
		    cerr << "n";
		    int h = getchar();
		    if (h == 't')
		      {
			cerr << "tf(\"%d\", ";
			peekToken = T_print;
			inPrint = true;
			break;
		      }
		    else
		      scan_error(h);
		  }
		else
		  scan_error(g);
	      }
	    else
	      scan_error(f);
	  }
	else
	  scan_error(e);
      }
    case ':': cerr << ":" << endl; peekToken = T_colon; break;
    case 'L': cerr << "L"; inLabel = true; peekToken = T_label; break;
    case '\n':
      lineNumber ++; 
      peekToken = T_newline;
      if (inLabel == true)
	{
	  inLabel = false;
	  break;
	}
      else
	cerr << ";" << endl;
      if (inGoto)
	{
	  inGoto = false;
	  break;
	}
      if(inIf)
	{
	inIf = false;
	break;
	}
      break;
    case ' ': return next_token(); break;
    default:
      if (t >= '0' && t <= '9')
	{
	  string a;
	  a = (char)t;
	  cerr << a;
	  peekToken = T_num;
	  int i = getchar();
	  while(i >= '0' && i <= '9')
	    {
	      string b;
	      b = (char)i;
	      cerr << b;
	      i = getchar();
	    }
	  if (inGoto)
	    {
	      cerr << " " ;
	      inGoto = false;
	    }
	  if (inIf)
	    {
	      cerr << ")";
	    }
	  ungetc(i,stdin);
	  break;
	}
      else
	scan_error(t);
    }
  tokenEaten = true;
  return peekToken;
}

void scanner_t::eat_token(token_type c)
{
  //if we are supposed to eat token c, and it does not match
  //what we are supposed to be reading from file, then it is a 
  //mismatch error ( call - mismatch_error(c) )
  if (c != next_token())
    mismatch_error(c);
  tokenEaten = false; //if no eat, doesnt peek next token
}
scanner_t::scanner_t()
{
   lineNumber = 1;
   tokenEaten = false;
   inLabel = false;
   inGoto = false;
   inPrint = false;
   inIf = false;
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
  void Start();
  void Expression();
  void Statements();
  void Statement();
  void Label();
  void Jump();
  void JumpCheck();
  void Assignment();
  void Print();
  void Expression2();
  void Term();
  void Term2();
  void Power();
  void Power2();
  void Factor();
public:	
  void parse();
};
//this function not only eats the token (moving the scanner forward one
//token), it also makes sure that token is drawn in the parse tree 
//properly by calling push and pop.
void parser_t::eat_token(token_type t)
{
  if (t != T_newline)
    {
      parsetree.push(t);
      scanner.eat_token(t);
      parsetree.pop();
    }
  else
    scanner.eat_token(t);
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
  parsetree.push(NT_Start);
  token_type parseToken = scanner.next_token();
  token_type parseToken2;
  switch(parseToken)
    {
    case T_m:
    case T_label:
    case T_goto:
    case T_print:
      Statement();
      parseToken2 = scanner.next_token();
      switch(parseToken2)
	{
	case T_newline:
	  eat_token(T_newline);
	  Start();
	  break;
	case T_eof:
	  break;
	default:
	  syntax_error(NT_Start);
	  break;
	}
    case T_eof:
      break;
    default:
      syntax_error(NT_Start);
      break;
    }
  parsetree.pop();
}
void parser_t::Statement()
{
    parsetree.push(NT_Statement);
    token_type parseToken = scanner.next_token();
    switch(parseToken) 
    {
    case T_m:
      Assignment();
      break;
    case T_label: 
      Label();
      break;
    case T_goto:
      Jump();
      break;
    case T_print: 
      Print();
      break;
    default:
      syntax_error(NT_Statement);
      break;
    }
    parsetree.pop();
}

void parser_t::Jump()
{
  parsetree.push(NT_Jump);
  token_type parseToken = scanner.next_token();
  token_type parseToken2;
  switch(parseToken)
    {
    case T_goto:
      eat_token(T_goto);
      parseToken2 = scanner.next_token();
      switch(parseToken2)
	{
	case T_num:
	  eat_token(T_num);
	  JumpCheck();
	  break;
	default:
	  syntax_error(NT_Jump);
	  break;
	}
      break;
    default:
      syntax_error(NT_Jump);
      break;
    }
  parsetree.pop();
}

void parser_t::JumpCheck()
{
  parsetree.push(NT_JumpCheck);
  token_type parseToken = scanner.next_token();
  switch(parseToken)
    {
    case T_if:
      eat_token(T_if);
      Expression();
      break;
    case T_eof:
    case T_newline:
      break;
    default:
      syntax_error(NT_JumpCheck);
      break;
    }
  parsetree.pop();
}

void parser_t::Print()
{
  parsetree.push(NT_Print);
  token_type parseToken = scanner.next_token();
  switch(parseToken)
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

void parser_t::Label()
{
  parsetree.push(NT_Label);
  token_type parseToken = scanner.next_token();
  token_type parseToken2;
  token_type parseToken3;
  switch(parseToken)
    {
    case T_label:
      eat_token(T_label);
      parseToken2 = scanner.next_token();
      switch(parseToken2)
	{
	case T_num:
	  eat_token(T_num);
	  parseToken3 = scanner.next_token();
	  switch(parseToken3)
	    {
	    case T_colon:
	      eat_token(T_colon);
	      break;
	    default:
	      syntax_error(NT_Label);
	      break;
	    }
	  break;
	default:
	  syntax_error(NT_Label);
	  break;
	}
      break;
    default:
      syntax_error(NT_Label);
      break;
    }
  parsetree.pop();
}

void parser_t::Assignment()
{
    parsetree.push(NT_Assignment);
    token_type parseToken = scanner.next_token();
    token_type parseToken2;
    switch(parseToken)
    {
    case T_m:
      eat_token(T_m);
      parseToken2 = scanner.next_token();
      switch(parseToken2)
	{
	case T_opensquare:
	  {
	    eat_token(T_opensquare);
	    Expression();
	    eat_token(T_closesquare);
	    token_type parseToken3 = scanner.next_token();
	    switch(parseToken3)
	      {
	      case T_equals:
		eat_token(T_equals);
		Expression();
		break;
	      default:
		syntax_error(NT_Assignment);
		break;
	      }
	    break;
	  }
	  break;
	default:
	  syntax_error(NT_Assignment);
	  break;
	}
      break;
    default:
      syntax_error(NT_Assignment);
      break;
    }
    parsetree.pop();
}

void parser_t::Expression()
{
    parsetree.push(NT_Expression);
    token_type parseToken = scanner.next_token();
    switch(parseToken)
      {
      case T_num:
      case T_openparen:
      case T_m:
	Term();
	Expression2();
	break;
      default:
	syntax_error(NT_Expression);
	break;
      }
    parsetree.pop();
}

void parser_t::Expression2()
{
  parsetree.push(NT_Expression2);
  token_type parseToken = scanner.next_token();
  switch(parseToken)
    {
    case T_plus:
      eat_token(parseToken);
      Expression();
      break;
    case T_minus:
      eat_token(parseToken);
      Expression();
      break;
    case T_closeparen:
      break;
    case T_closesquare:
      break;
    case T_eof:
    case T_newline:
      break;
    default:
      syntax_error(NT_Expression2);
      break;
    }
  parsetree.pop();
}

void parser_t::Term()
{
  parsetree.push(NT_Term);
  token_type parseToken = scanner.next_token();
  switch(parseToken)
    {
    case T_num:
    case T_openparen:
    case T_m:
      Power();
      Term2();
      break;
    default:
      syntax_error(NT_Term);
      break;
    }
  parsetree.pop();
}

void parser_t::Term2()
{
  parsetree.push(NT_Term2);
  token_type parseToken = scanner.next_token();
  switch(parseToken)
    {
    case T_times:
      eat_token(T_times);
      Term();
      break;
    case T_divide:
      eat_token(T_divide);
      Term();
      break;
    case T_plus:
      break;
    case T_minus:
      break;
    case T_closeparen:
      break;
    case T_closesquare:
      break;
    case T_eof:
    case T_newline:
      break;
    default:
      syntax_error(NT_Term2);
      break;
    }
  parsetree.pop();
}
void parser_t::Power()
{
  parsetree.push(NT_Power);
  token_type parseToken = scanner.next_token();
  switch(parseToken)
    {
    case T_num:
    case T_openparen:
    case T_m:
      Factor();
      Power2();
      break;
    default:
      syntax_error(NT_Power);
      break;
    }
  parsetree.pop();
}

void parser_t::Power2()
{
  parsetree.push(NT_Power2);
  token_type parseToken = scanner.next_token();
  switch(parseToken)
    {
    case T_power:
      eat_token(parseToken);
      Power();
      break;
    case T_times:
      break;
    case T_divide:
      break;
    case T_plus:
      break;
    case T_minus:
      break;
    case T_closeparen:
      break;
    case T_closesquare:
      break;
    case T_eof:
    case T_newline:
      break;
    default:
      syntax_error(NT_Power2);
      break;
    }
  parsetree.pop();
}

void parser_t::Factor()
{
  parsetree.push(NT_Factor);
  token_type parseToken = scanner.next_token();
  token_type parseToken5;
  switch(parseToken)
    {
    case T_num:
      eat_token(T_num);
      break;
    case T_openparen:
      eat_token(T_openparen);
      Expression();
      eat_token(T_closeparen);
      break;
    case T_m:
      eat_token(T_m);
      parseToken5 = scanner.next_token();
      switch(parseToken5)
	{
	case T_opensquare:
	  eat_token(T_opensquare);
	  Expression();
	  eat_token(T_closesquare);
	  break;
	default:
	  syntax_error(NT_Factor);
	  break;
	}
      break;
    default:
      syntax_error(NT_Factor);
      break;
    }
  parsetree.pop();
}  


/*** Main ***********************************************/
int main()
{
  cerr << "#include <stdlib.h>" << endl;
  cerr << "#include <stdio.h>" << endl;
  cerr << "#include <string.h>" << endl;
  cerr << "#include <iostream>" << endl;
  cerr << "#include <math.h>" << endl;
  cerr << "using namespace std;"<< endl;
  cerr << "int main()" << endl;
  cerr << "{" << endl;
  cerr << "int m[100];" << endl;
  

  parser_t parser;
  parser.parse();
 

  cerr << "return 0;" << endl;
  cerr << "}" << endl;
/*   //test scanner
   scanner_t scanner;
   token_type token = scanner.next_token();
   while (token != T_eof)
   {
   printf("%s\n", token_to_string(token));
   scanner.eat_token(token);
   token = scanner.next_token();
   }
  */
return 0;
}

