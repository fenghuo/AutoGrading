#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <stack>

using namespace std;

/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25

//all of the terminals in the language
typedef enum {
  T_eof = 0,	// 0: end of file
  T_num,	// 1: numbers
  T_plus,	// 2: +
  T_minus,	// 3: -
  T_times,	// 4: *
  T_divide,	// 5: /
  T_power, 	// 6: **
  T_equals,	// 7: =
  T_openparen,	// 8: (
  T_closeparen, // 9: )
  T_opensquare,	// 10: [
  T_closesquare,// 11: ]
  T_m,		// 12: m
  T_goto,	// 13: goto
  T_if,		// 14: if
  T_print,	// 15: print
  T_label,	// 16: L
  T_colon	// 17: :
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
    case T_plus:	strncpy(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
    case T_minus:	strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
    case T_times:	strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
    case T_divide:	strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
    case T_power:	strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
    case T_equals:	strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
    case T_openparen:	strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
    case T_closeparen:	strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
    case T_opensquare:	strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
    case T_closesquare:	strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
    case T_m:		strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
    case T_goto:	strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
    case T_if:		strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
    case T_print:	strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
    case T_label:	strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
    case T_colon:	strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
    default:   		strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
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
  NT_GotoZ,
  NT_NumZ,
  NT_Assignment,
  NT_Print,
  NT_Expression,
  NT_ExpressionP,
  NT_Term,
  NT_TermP,
  NT_Power,
  NT_PowerP,
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
    case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
    case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
    case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
    case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
    case NT_GotoZ: strncpy(buffer,"GotoZ",MAX_SYMBOL_NAME_SIZE); break;
    case NT_NumZ: strncpy(buffer,"NumZ",MAX_SYMBOL_NAME_SIZE); break;
    case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
    case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
    case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
    case NT_ExpressionP: strncpy(buffer,"ExpressionP",MAX_SYMBOL_NAME_SIZE); break;
    case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
    case NT_TermP: strncpy(buffer,"TermP",MAX_SYMBOL_NAME_SIZE); break;
    case NT_Power: strncpy(buffer,"Power",MAX_SYMBOL_NAME_SIZE); break;
    case NT_PowerP: strncpy(buffer,"PowerP",MAX_SYMBOL_NAME_SIZE); break;
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

  //return current number
  int get_number();

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

  //peeks at the next number
  int next_number();

  //peeks at input and tests against reference char array
  void next_char(const char* ref);

  //error message and exit for unrecognized character
  void scan_error(char x);

  //error message and exit for token mismatch
  void mismatch_error(token_type c);

  //WRITEME: this is a bogus member for implementing a useful stub, it should
  //be cut out once you get the scanner up and going.
  int cur_token; // default is -1 so we know we have to eat up more tokens to load cur_token
  int cur_number; // the token's associated number is stored here, if it has any
  int cur_line;
};

token_type scanner_t::next_token() {
  /*
    T_eof = 0,      // 0: end of file
    T_num,          // 1: numbers
    T_plus,         // 2: +
    T_minus,        // 3: -
    T_times,        // 4: *
    T_divide,       // 5: /
    T_power,        // 6: **
    T_equals,       // 7: =
    T_openparen,    // 8: (
    T_closeparen,   // 9: )
    T_opensquare,   // 10: [
    T_closesquare,  // 11: ]
    T_m,            // 12: m
    T_goto,         // 13: goto
    T_if,           // 14: if
    T_print,        // 15: print
    T_label,        // 16: L
    T_colon         // 17: :
  */

  if (cur_token != -1)
    return static_cast<token_type>(cur_token);

  int c;

  while (cur_token == -1) {
    // first we'll peek, see if it's a multi-char token
    // if so, let next_char()/next_number() handle it
    // otherwise, we'll read in the char and handle the other cases
    c = cin.peek();

    switch (c) {
      case '0' ... '9':
	cur_number = next_number();
	cur_token = T_num;
	continue;

      case 'g':
	next_char("goto");
	cur_token = T_goto;
	continue;

      case 'i':
        next_char("if");
        cur_token = T_if;
        continue;

      case 'p':
        next_char("print");
        cur_token = T_print;
        continue;

      default:
	// we'll let the next switch handle it
	break;
    }

    c = cin.get();

    switch (c) {
      case EOF:
	cur_token = T_eof; continue;

      case '\n':
	printf("\n");
	cur_line++; // hey, it's a newline!
      case ' ': case '\t': case '\v': case '\f': case '\r':
	// ew, whitespace
	continue;

      case '+':
	cur_token = T_plus; continue;

      case '-':
	cur_token = T_minus; continue;

      case '*':
	if (cin.peek() == '*') {
	  // so this is really an exponent
	  c = cin.get();
	  cur_token = T_power;
	} else {
	  cur_token = T_times;
	}

	continue;

      case '/':
	cur_token = T_divide; continue;

      case '=':
	cur_token = T_equals; continue;

      case '(':
	cur_token = T_openparen; continue;

      case ')':
	cur_token = T_closeparen; continue;

      case '[':
	cur_token = T_opensquare; continue;

      case ']':
	cur_token = T_closesquare; continue;

      case 'm':
	cur_token = T_m; continue;

      case 'L':
	cur_token = T_label; continue;

      case ':':
	cur_token = T_colon; continue;

      default:
	// well that's not nice
	scan_error(c);
	break;
    }
  }

  return static_cast<token_type>(cur_token);
}

void scanner_t::eat_token(token_type c)
{
  if (cur_token == -1) {
    next_token();
  }

  if (c != static_cast<token_type>(cur_token)) {
    mismatch_error(c);
  }

  // otherwise, we'll reset cur_token and cur_number
  cur_token = -1;
  cur_number = -1;
}

int scanner_t::next_number() {
  // called multiple times, don't eat input
  if (cur_number != -1) {
    return cur_number;
  }

  // max number is an undefined number of chars long (it could be "0000000004" for instance)
  // so we should reallocate as we go...
  char *buf = NULL;

  int c = cin.peek();
  size_t n = 0;

  while (true) {
    if (c < '0' || c > '9') {
      // we got a character other than 0-9
      // the number could be over, or maybe there was no number... which is bad!
      break;
    } else {
      // we now know it's safe to eat up this character
      c = cin.get();
    }

    // realloc and store
    buf = (char *)realloc(buf, sizeof(char) * ++n);
    buf[n-1] = c;

    // NULL character
    buf = (char *)realloc(buf, sizeof(char) * (n + 1));
    buf[n] = '\0';

    // we should check every iteration if we're trying to store too large of a number
    // too large in this case is >2^31 - 1 or 2147483647
    // too small in this case is <0 (though this if check should never return true)
    // we do not need to worry about negative numbers or other weird input
    // since anything other than 0-9 is ignored and terminates bringing in more chars
    long int num = atol(buf);

    if (num < 0 || num > 2147483647) {
      scan_error(c);
    } else {
      cur_number = static_cast<int>(num);
    }

    // let's peek again!
    c = cin.peek();
  }

  return cur_number;
}

void scanner_t::next_char(const char* ref) {
  int c = cin.get();
  size_t n = 0;
  while (ref[n]) {
    if (c != ref[n]) {
      scan_error(c);
    }

    c = cin.get();
    n++;
  }
}

scanner_t::scanner_t()
{
  cur_token = -1;
  cur_line = 1;
}

int scanner_t::get_number() {
  return cur_number;
}

int scanner_t::get_line()
{
  //WRITEME: this function should return the current line number (used
  //for error reporting
  return cur_line;
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
  string Label(string line);
  string Jump(string line);
  string GotoZ(string line);
  string NumZ(string line);
  string Assignment(string line);
  string Print(string line);
  string Expression(string line);
  string ExpressionP(string line);
  string Term(string line);
  string TermP(string line);
  string Power(string line);
  string PowerP(string line);
  string Factor(string line);

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
  cerr << "#include <stdio.h>" << endl
       << "#include <math.h>" << endl
       << "int main() {" << endl
       << "int m[101];" << endl;
  Start();
  cerr << "}" << endl;
}

/*
 * Start         : Statement Statements
 *               ;
*/
void parser_t::Start()
{
  //push this non-terminal onto the parse tree.
  //the parsetree class is just for drawing the finished
  //parse tree, and should in should have no effect the actual
  //parsing of the data
  parsetree.push(NT_Start);

  switch (scanner.next_token()) {
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      Statement();
      Statements();
      break;
  }

  switch (scanner.next_token()) {
    case T_eof:
      break;
    default:
      syntax_error(NT_Start);
  }

  parsetree.pop();
}

/*
 * Statements    : Statement Statements
 *               |
 *               ;
 */
void parser_t::Statements()
{
  parsetree.push(NT_Statements);

  switch (scanner.next_token()) {
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      Statement();
      Statements();
      break;
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_Statements);
  }

  parsetree.pop();
}

/*
 * Statement     : Label
 *               | Jump
 *               | Assignment
 *               | Print
 *               ;
 */
void parser_t::Statement()
{
  parsetree.push(NT_Statement);

  string line;

  switch (scanner.next_token()) {
    case T_label:
      line = Label(line);
      break;
    case T_goto:
      line = Jump(line);
      break;
    case T_m:
      line = Assignment(line);
      break;
    case T_print:
      line = Print(line);
      break;
    default:
      syntax_error(NT_Statement);
  }

  switch (scanner.next_token()) {
    case T_eof:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      break;
    default:
      syntax_error(NT_Statement);
  }

  // statements end with semi-colon and newline
  cerr << line << ";" << endl;

  parsetree.pop();
}

/*
 * Label         : 'l' 'n' ':'
 *               ;
 */
string parser_t::Label(string line)
{
  parsetree.push(NT_Label);

  if (scanner.next_token() != T_label)
    syntax_error(NT_Label);

  eat_token(T_label);

  if (scanner.next_token() != T_num) syntax_error(NT_Label);
  line.append("L" + to_string(scanner.get_number()));
  eat_token(T_num);

  if (scanner.next_token() != T_colon) syntax_error(NT_Label);
  line.append(":");
  eat_token(T_colon);

  switch (scanner.next_token()) {
    case T_eof:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      break;
    default:
      syntax_error(NT_Label);
  }

  parsetree.pop();

  return line;
}

/*
 * Jump          : 'g' GotoZ
 *               ;
 */
string parser_t::Jump(string line) {
  parsetree.push(NT_Jump);

  if (scanner.next_token() != T_goto)
    syntax_error(NT_Jump);

  eat_token(T_goto);
  line = GotoZ(line);

  switch (scanner.next_token()) {
    case T_eof:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      break;
    default:
      syntax_error(NT_Jump);
  }

  parsetree.pop();

  return line;
}

/*
 * GotoZ         : 'n' NumZ
 *               ;
 */
string parser_t::GotoZ(string line) {
  parsetree.push(NT_GotoZ);

  if (scanner.next_token() != T_num)
    syntax_error(NT_GotoZ);

  string left, right;

  right.append("goto L" + to_string(scanner.get_number()));
  eat_token(T_num);

  if (scanner.next_token() == T_if) {
    left.append("if(");
    left = NumZ(left);
    left.append("){");
    right.append(";");
    left.append(right);
    left.append("}");
  } else {
    left = NumZ(left);
    left.append(right);
  }

  line.append(left);

  switch (scanner.next_token()) {
    case T_eof:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      break;
    default:
      syntax_error(NT_GotoZ);
  }

  parsetree.pop();

  return line;
}

/*
 * NumZ          : 'i' Expression
 *               |
 *               ;
*/
string parser_t::NumZ(string line) {
  parsetree.push(NT_NumZ);

  switch (scanner.next_token()) {
    case T_if:
      eat_token(T_if);
      line = Expression(line);
      break;
    case T_eof:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_NumZ);
  }

  parsetree.pop();

  return line;
}

/*
 * Assignment    : 'm' '[' Expression ']' '=' Expression
 *               ;
 */
string parser_t::Assignment(string line) {
  parsetree.push(NT_Assignment);

  if (scanner.next_token() != T_m)
    syntax_error(NT_Assignment);

  eat_token(T_m);

  line.append("m");

  if (scanner.next_token() == T_opensquare) {
    eat_token(T_opensquare);
    line.append("[");
  } else {
    syntax_error(NT_Assignment);
  }

  line = Expression(line);

  eat_token(T_closesquare);
  line.append("]");

  if (scanner.next_token() == T_equals) {
    eat_token(T_equals);
    line.append("=");
  } else {
    syntax_error(NT_Assignment);
  }

  line = Expression(line);

  switch (scanner.next_token()) {
    case T_eof:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      break;
    default:
      syntax_error(NT_Assignment);
  }

  parsetree.pop();

  return line;
}

/*
 * Print         : 'p' Expression
 *               ;
 */
string parser_t::Print(string line)
{
  parsetree.push(NT_Print);

  switch (scanner.next_token()) {
    case T_print:
      eat_token(T_print);
      line.append("printf(\"%d\\n\", ");
      line = Expression(line);
      line.append(")");
      break;
    default:
      syntax_error(NT_Print);
      break;
  }

  switch (scanner.next_token()) {
    case T_eof:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      break;
    default:
      syntax_error(NT_Print);
  }

  parsetree.pop();

  return line;
}

/*
 * Expression    : Term ExpressionP
 *               ;
 */
string parser_t::Expression(string line)
{
  parsetree.push(NT_Expression);

  switch (scanner.next_token()) {
    case T_openparen:
    case T_m:
    case T_num:
      line = Term(line);
      line = ExpressionP(line);
      break;
    default:
      syntax_error(NT_Expression);
  }

  switch (scanner.next_token()) {
    case T_eof:
    case T_closeparen:
    case T_closesquare:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      break;
    default:
      syntax_error(NT_Expression);
  }

  parsetree.pop();

  return line;
}

/*
 * ExpressionP   : '+' Term ExpressionP
 *               | '-' Term ExpressionP
 *               |
 *               ;
 */
string parser_t::ExpressionP(string line)
{
  parsetree.push(NT_ExpressionP);

  switch (scanner.next_token()) {
    case T_plus:
      eat_token(T_plus);
      line.append("+");
      line = Term(line);
      line = ExpressionP(line);
      break;
    case T_minus:
      eat_token(T_minus);
      line.append("-");
      line = Term(line);
      line = ExpressionP(line);
      break;
    case T_eof:
    case T_closeparen:
    case T_closesquare:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_ExpressionP);
  }

  parsetree.pop();

  return line;
}

/*
 * Term          : Factor TermP
 *               ;
*/
string parser_t::Term(string line)
{
  parsetree.push(NT_Term);

  switch (scanner.next_token()) {
    case T_openparen:
    case T_m:
    case T_num:
      line = Factor(line);
      line = TermP(line);
      break;
    default:
      syntax_error(NT_Term);
  }

  switch (scanner.next_token()) {
    case T_eof:
    case T_plus:
    case T_minus:
    case T_closeparen:
    case T_closesquare:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      break;
    default:
      syntax_error(NT_Term);
  }

  parsetree.pop();

  return line;
}

/*
 * TermP         : '*' Power TermP
 *               | '/' Power TermP
 *               |
 *               ;
 */
string parser_t::TermP(string line)
{
  parsetree.push(NT_TermP);

  switch (scanner.next_token()) {
    case T_times:
      eat_token(T_times);
      line.append("*");
      line = Power(line);
      line = TermP(line);
      break;
    case T_divide:
      eat_token(T_divide);
      line.append("/");
      line = Power(line);
      line = TermP(line);
      break;
    case T_eof:
    case T_plus:
    case T_minus:
    case T_closeparen:
    case T_closesquare:
    case T_label:
    case T_goto:
    case T_m:
    case T_print:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_TermP);
  }

  parsetree.pop();

  return line;
}

/*
 * Factor        : Power PowerP
 *               ;
 */
string parser_t::Factor(string line)
{
  parsetree.push(NT_Factor);

  string left, right;

  switch (scanner.next_token()) {
    case T_openparen:
    case T_m:
    case T_num:
      left = Power(left);

      if (scanner.next_token() == T_power) {
	left.insert(0, "(int)powf((int)");
	right.append(", (int)");
	right = PowerP(right);
	right.append(")");
      } else {
	right = PowerP(right);
      }

      // put right at the end of left
      left.append(right);

      // recurse our line back up
      line.append(left);

      break;
    default:
      syntax_error(NT_Factor);
      break;
  }

  switch (scanner.next_token()) {
    case T_eof:
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
    default:
      syntax_error(NT_Factor);
  }

  parsetree.pop();

  return line;
}

/*
 * PowerP        : 'e' Factor
 *               |
 *               ;
 */
string parser_t::PowerP(string line)
{
  parsetree.push(NT_PowerP);

  switch (scanner.next_token()) {
    case T_power:
      eat_token(T_power);
      line = Factor(line);
      break;
    case T_eof:
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
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_PowerP);
  }

  parsetree.pop();

  return line;
}

/*
 * Power         : '(' Expression ')'
 *               | 'm' '[' Expression ']'
 *               | 'n'
 *               ;
 */
string parser_t::Power(string line)
{
  parsetree.push(NT_Power);

  switch (scanner.next_token()) {
    case T_openparen:
      eat_token(T_openparen);
      line.append("(");
      line = Expression(line);
      eat_token(T_closeparen);
      line.append(")");
      break;
    case T_m:
      eat_token(T_m);
      line.append("m");
      eat_token(T_opensquare);
      line.append("[");
      line = Expression(line);
      eat_token(T_closesquare);
      line.append("]");
      break;
    case T_num:
      line.append(to_string(scanner.get_number()));
      eat_token(T_num);
      break;
    default:
      syntax_error(NT_Power);
  }

  switch (scanner.next_token()) {
    case T_eof:
    case T_power:
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
    default:
      syntax_error(NT_Power);
  }

  parsetree.pop();

  return line;
}

/*** Main ***********************************************/

int main()
{
  parser_t parser;
  parser.parse();

  scanner_t scanner;

  /*
  while (true) {
    token_type token = scanner.next_token();

    if (token == T_num) {
      printf("n%d ", scanner.get_number());
    } else {
      printf("%s ", token_to_string(token));
    }

    scanner.eat_token(token);
    if (token == T_eof) break;
  }
  */

  return 0;
}
