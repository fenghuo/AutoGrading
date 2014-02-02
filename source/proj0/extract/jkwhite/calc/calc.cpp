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
	NT_Statements2,
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Assignment,
	NT_Print,
	NT_Expression2,
	NT_Mem,
	NT_Paren,
	NT_Factor,
	NT_Factor2,
	NT_Term,
	NT_Term2,
	NT_Number

	
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
	case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Statements2: strncpy(buffer,"Statements2",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Mem: strncpy(buffer,"Mem",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Expression2: strncpy(buffer,"Expression2",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Paren: strncpy(buffer,"Paren",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Factor2: strncpy(buffer,"Factor2",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Term2: strncpy(buffer,"Term2",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Number: strncpy(buffer,"Number",MAX_SYMBOL_NAME_SIZE); break;
	
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
  
  void range_error();    

        int line_number;

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	token_type bogo_token;

};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
 bool done  = false;
 char token, temp, temp2, temp3, temp4, temp5;
  token_type type;

  while (done == false ) {
    token = cin.peek();
   
    switch (token ) {

    case '0': 
    case '1':
    case '2':
    case '3': 
    case '4':
    case '5':
    case '6':
    case '7':
    case '8': 
    case '9': type = T_num;
      done = true;
      break;

    case 'm': type = T_m;
      done = true;
      break;
    case 'L': type = T_label;
      done = true;
      break;

    case '+': type = T_plus;
      done = true;
      break;
    case '-': type =T_minus;
      done = true;
      break;
    case '*':{
      cin.get(temp);
      if(cin.peek() == '*'){
	cin.putback(temp);
	type = T_power;
      }
      else{
	cin.putback(temp);
	type = T_times;
      }
      done = true;
      break;
    }
    case '/': type = T_divide;
      done = true;
      break;
    case '=': type = T_equals;
      done = true;
      break;

    case 'i':{ 
      cin.get(temp);
      if(cin.peek() == 'f'){
	cin.putback(temp);
	type = T_if;
	done = true;
      }
      else{
	cin.putback(temp);
	scan_error(token);
      }
      break;
    }

    case '(': type = T_openparen;
      done = true;
      break;
    case ')': type = T_closeparen;
      done = true;
      break;
    case '[': type = T_opensquare;
      done = true;
      break;
    case ']': type = T_closesquare;
      done = true;
      break;
    case ':': type = T_colon;
      done = true;
      break;

    case ' ': token = getchar();
      done = false;
      break;
    case '\n': token = getchar();  
      line_number++;
      done = false;
      break;

    case 'g': { 
      cin.get(temp);
      if(cin.peek() == 'o'){
	cin.get(temp2);
	if(cin.peek() == 't'){
	  cin.get(temp3);
	  if(cin.peek() == 'o'){
	    cin.putback(temp3);
	    cin.putback(temp2);
	    cin.putback(temp);
	    type = T_goto;
	  }
	  else{
	    cin.putback(temp3);
	    cin.putback(temp2);
	    cin.putback(temp);
	    scan_error(token);
	  }
	}
	else {
	  cin.putback(temp2);
	  cin.putback(temp);
	  scan_error(token);
	}
      }
      else{
	cin.putback(temp);
	scan_error(token);
      }
      done = true;
      break;
    }

    case 'p':{
      // cout << "reading print\n";
      cin.get(temp);
      if(cin.peek() == 'r'){
	cin.get(temp2);
	if(cin.peek() == 'i'){
	  cin.get(temp3);
	  if(cin.peek() == 'n'){
	    cin.get(temp4);
	    if(cin.peek() == 't'){
	      cin.putback(temp4);
	      cin.putback(temp3);
	      cin.putback(temp2);
	      cin.putback(temp);
	      type = T_print;
	    }
	    else 
	      scan_error(token);
	    
	  }
	  else
	    scan_error(token);
	  
	}
	else 
	  scan_error(token);
	
      }
      else
	scan_error(token);
    
      done = true; 
      break;  
    }
    case EOF:
      return T_eof;
      break;

    default: scan_error(token);
      break;
    }
  }

  return type;
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

  bool done = false;
  int periods = 0;
  string number = "";
  char next_char;
  int temp = 0;

  if(c == T_print){
    while(temp < 5){
      // cout << "eating print\n";
      getchar();
      temp++;
    }
  }

  else if(c == T_goto){
    while(temp < 4){
      getchar();
      temp++;
    }
  }

  else if(c == T_if){
    getchar();
    getchar();
  }

  else if(c == T_power){
    getchar();
    getchar();
  }
  
  else if (c == T_num) {
    while (done == false) {
      next_char = cin.peek();
      switch (next_char) {

      case '0':
      case '1':
      case '2':
      case '3': 
      case '4':
      case '5':
      case '6':
      case '7':
      case '8': 
      case '9': 
	number += getchar();
	while(isdigit(cin.peek()))
	  number += getchar();
	
	if(atoi(number.c_str()) >= 2147483647 || atoi(number.c_str()) < 0){
	  range_error();
	} 
	break;


      default: done = true;
	break;
      }
    } 

  } else if (token_to_string(c)[0] == getchar()) {
    return;
  } else { 
    mismatch_error(c);
  }

}

scanner_t::scanner_t()
{
  line_number = 1;
}

int scanner_t::get_line()
{
  return line_number;
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

void scanner_t::range_error()
{
  printf("range error: number was out of range 0 - (2^31 - 1)\n");
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
  void Statements();
  void Statements2();
  void Statement();
  void Label();
  void Jump();
  void Assignment();
  void Print();
  void Expression();
  void Expression2();
  void Term();
  void Term2();
  void Factor();
  void Factor2();
  void Mem();
  void Number();
  void Paren();

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
	case T_m:
	case T_label:
	case T_print:
	case T_goto:
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
}

//WRITEME: you will need to put the rest of the procedures here

void parser_t::Statements()
{
  parsetree.push(NT_Statements);

  Statement();
  
	switch( scanner.next_token() ) 
	{
	case T_m:
	case T_label:
	case T_print:
	case T_goto:
	  Statements2();
	  break;

	default:
	  syntax_error(NT_Statements);
	  break;
	}

	parsetree.pop();
}
  
void parser_t::Statements2()
{
  parsetree.push(NT_Statements2);

	switch( scanner.next_token() ) 
	{
	case T_m:
	case T_label:
	case T_print:
	case T_goto:
	  Statement();
	  Statements2();
	  break;
	  
	case T_eof:
	  parsetree.drawepsilon();
	  break;

	default:
	   syntax_error(NT_Statements2);
	  break;
	}
	parsetree.pop();

}
 
void parser_t::Statement()
{
  parsetree.push(NT_Statement);

  switch(scanner.next_token())
    {
    case T_m:
      Assignment();
      break;
      
    case T_label:
      Label();
      break;

    case T_print:
      Print();
      break;

    case T_goto:
      Jump();
      break;

    default:
      syntax_error(NT_Statement);
      break;
    }
	parsetree.pop();
}

void parser_t::Label()
{
  parsetree.push(NT_Label);
  
  if(scanner.next_token() == T_label)
    eat_token(T_label);
  else
    syntax_error(NT_Label);
  if(scanner.next_token() == T_num)
    eat_token(T_num);
  else
    syntax_error(NT_Label);
  if(scanner.next_token() == T_colon)
    eat_token(T_colon);
  else
    syntax_error(NT_Label);
  
  parsetree.pop();
  
}
 
void parser_t::Jump()
{
  parsetree.push(NT_Jump);
  
  if(scanner.next_token() == T_goto)
    eat_token(T_goto);
  else 
    syntax_error(NT_Jump);
  if(scanner.next_token() == T_num)
    eat_token(T_num);
  else
    syntax_error(NT_Jump);
 
  if(scanner.next_token() == T_if){
    eat_token(T_if);
    switch(scanner.next_token())
      {
      case T_m:	
      case T_num:
      case T_openparen:
	Expression();
	break;

      default:
	syntax_error(NT_Jump);
	break;
      } 
  }

  parsetree.pop();
}

void parser_t::Assignment()
{
  parsetree.push(NT_Assignment);

  if(scanner.next_token() == T_m)
    eat_token(T_m);
  else 
    syntax_error(NT_Assignment);
  if(scanner.next_token() == T_opensquare)
    eat_token(T_opensquare);
  else 
    syntax_error(NT_Assignment);

  Expression();

  if(scanner.next_token() == T_closesquare)
    eat_token(T_closesquare);
  else
    syntax_error(NT_Assignment);
  if(scanner.next_token() == T_equals)
    eat_token(T_equals);
  else 
    syntax_error(NT_Assignment);

  Expression();

  parsetree.pop();

}
 
void parser_t::Print()
{

  if(scanner.next_token() == T_print)
    eat_token(T_print);
  else
    syntax_error(NT_Print);

  switch(scanner.next_token())
    {
    case T_m:
    case T_num:
    case T_openparen:
      Expression();
      break;

    default:
      syntax_error(NT_Print);
      break;
    }
  parsetree.pop();
}
 
void parser_t::Expression()
{
  parsetree.push(NT_Expression);
  
  switch(scanner.next_token())
    {
    case T_m:
    case T_num:
    case T_openparen:
      Term();
      Expression2();
      break;
      
    default:
      syntax_error(NT_Print);
      break;
    }
  parsetree.pop();

}
 
void parser_t::Expression2()
{
  parsetree.push(NT_Expression2);

  switch(scanner.next_token())
    {
    case T_plus:
      eat_token(T_plus);
      Term();
      Expression2();
      break;

    case T_minus:
      eat_token(T_minus);
      Term();
      Expression2();
      break;

    case T_eof:
      parsetree.drawepsilon();
      break;

    default:
      parsetree.drawepsilon();
      break;
      } 
  parsetree.pop();
}
 
 void parser_t::Mem()
 {
   parsetree.push(NT_Mem);

   switch(scanner.next_token())
     {
     case T_m:
       eat_token(T_m);
       eat_token(T_opensquare);
       Expression();
       eat_token(T_closesquare);
       break;

     case T_num:
       Number();
       break;
     
     default:
       syntax_error(NT_Mem);
       break;
     }
   parsetree.pop();
 }
  
 void parser_t::Number()
 {
   parsetree.push(NT_Number);

    eat_token(T_num);

   parsetree.pop();

 }
  
 void parser_t::Paren()
 {
   parsetree.push(NT_Paren);
   
   switch(scanner.next_token())
     {
     case T_openparen:
       eat_token(T_openparen);
       Expression();
       if(scanner.next_token() == T_closeparen)
	 eat_token(T_closeparen);
       else
	 syntax_error(NT_Paren);
       break;

     case T_m:
     case T_num:
       Mem();
       break;

     default:
       syntax_error(NT_Paren);
       break;
     } 
   parsetree.pop(); 
}

void parser_t::Term()
{
  parsetree.push(NT_Term);
  
  switch(scanner.next_token())
    {
    case T_m:
    case T_num:
    case T_openparen:
      Factor();
      Term2();
      break;

    default:
      syntax_error(NT_Factor);
      break;
    }
  parsetree.pop();
}
 
void parser_t::Term2()
{
  parsetree.push(NT_Term2);

  switch(scanner.next_token())
    {
    case T_times:
      eat_token(T_times);
      Factor();
      Term2();
      break;
    case T_divide:
      eat_token(T_divide);
      Factor();
      Term2();
      break;

    default:
      parsetree.drawepsilon();
      break;
    }
  parsetree.pop();
}
 
void parser_t::Factor()
{
  parsetree.push(NT_Factor);
  
  switch(scanner.next_token())
    {
    case T_m:
    case T_num:
    case T_openparen:
      Paren();
      Factor2();
      break;

    default:
      syntax_error(NT_Factor);
      break;
    }
  parsetree.pop();
}
 
void parser_t::Factor2()
{
  parsetree.push(NT_Factor2);

  switch(scanner.next_token())
    {
    case T_power:
      eat_token(T_power);
      Paren();
      Factor2();
      break;

    default:
      parsetree.drawepsilon();
      break;
    }
  parsetree.pop();

}
 

/*** Main ***********************************************/

int main()
{
  
  /*
  scanner_t scanner;
  token_type tok = scanner.next_token();
  while(tok != T_eof){
    scanner.eat_token(tok);
    printf("%s ", token_to_string(tok));
    tok = scanner.next_token();
  }

    printf("%s\n", token_to_string(tok));
  */
  	parser_t parser;
  	parser.parse();
	return 0;
}
