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
	NT_Statements,
	NT_Statementsprime,
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Assignment,
	NT_Print,
	NT_Eprime,
	NT_D,
	NT_Dprime,
	NT_C,
	NT_Cprime,
	NT_Cprime2,
	NT_B,
	NT_A,
	NT_F,
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
	case epsilon: strncpy(buffer,"e",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Start: strncpy(buffer,"Start",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
	  //WRITEME: add the other nonterminals you need here	
	case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Statementsprime: strncpy(buffer,"Statementsprime",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Eprime: strncpy(buffer,"Eprime",MAX_SYMBOL_NAME_SIZE); break;
	case NT_D: strncpy(buffer,"D",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Dprime: strncpy(buffer,"Dprime",MAX_SYMBOL_NAME_SIZE); break;
	case NT_C: strncpy(buffer,"C",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Cprime: strncpy(buffer,"Cprime",MAX_SYMBOL_NAME_SIZE); break;
	case NT_B: strncpy(buffer,"B",MAX_SYMBOL_NAME_SIZE); break;
	case NT_A: strncpy(buffer,"A",MAX_SYMBOL_NAME_SIZE); break;
	case NT_F: strncpy(buffer,"F",MAX_SYMBOL_NAME_SIZE); break;
	  
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
  void range_error(token_type x);
  //WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
  token_type bogo_token;
  int linenumber;

};

token_type scanner_t::next_token()
{
  //WRITEME: replace this bogus junk with code that will take a peek
  //at the next token and return it to the parser.  It should _not_
  //actually consume a token - you should be able to call next_token()
  //multiple times without actually reading any more tokens in 
  //  if ( bogo_token!=T_plus && bogo_token!=T_eof ) return T_plus;
  //  else return bogo_token;
  bool finished = false;
  while(finished == false){
    token_type holder_token;
    char c = cin.peek();
   
    switch (c)
      {
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
	{
	  return T_num;
	  /*	char tempt;
	tempt = getchar();
	char a[9];
	int i;
	string Text;
	int y;
	int b = 0;
	i = 0;
	while (isdigit(cin.peek()) && i <= 9){
	  cout << "I = %s" << i << endl;
	  tempt = getchar();
	  Text += tempt;
	  a[i] = tempt;
	  if (i == 9 && isdigit(cin.peek()))
	    scan_error(a[i]);
	  i++;
	}
	while (i >= 0){
	  cin.putback(a[i]);
	    i--;
	  }

	int max1; 
	max1= 2147483647;
	y = atoi(Text.c_str());
	//stringstream convert(Text);
	//if (!(convert >> y)){
	//  y = 0;
	//}
	if (y <= max1 && y >= 0){
	  cout << "RETURNING NUM" << endl;
	  return T_num;
	}
	else{
	  cout << y << endl;
	  cout << "HELP" << endl;
	  char c;
	  scan_error(c);
	}
	//} */
	break;}

      case '+': {
	return T_plus;    
	break;	}
      case '-': {
	return T_minus;   
	break;}
      case '*': {
	char temp = getchar(); 
	if (cin.peek() == '*'){
	  cin.putback(temp); 
	  return T_power;
	} else{
	  cin.putback(temp);
	  //holder_token = T_times;
	  return T_times;
	}
	break;}
	//     case '**': holder_token = T_power;  	break;	
      case '/': { 
	return T_divide; //holder_token = T_divide;  	
	break;}
      case '(':{ 
	//holder_token = T_openparen;  	
	return T_openparen;
	break;}
      case ')':{ 
	// holder_token = T_closeparen; 
	return T_closeparen;
	break;}
      case '[':{ 
	//    holder_token = T_opensquare;  
	return T_opensquare;
	break;}
      case ']':{ 
	//    holder_token = T_closesquare;  	
	return T_closesquare;
	break;}
      case '=': {
	// holder_token = T_equals;  	
	return T_equals;
	break;	}
      case 'm':{
	return T_m;
	//    holder_token = T_m;	
	break;}
      case 'g': //case goto
	{char temp, temp2, temp3;
	temp = getchar();
	if (cin.peek() == 'o'){
	  temp2 = getchar();
	  if (cin.peek() == 't'){
	    temp3 = getchar();
	    if (cin.peek() == 'o'){
	      cin.putback(temp3);
	      cin.putback(temp2);
	      cin.putback(temp);
	      return T_goto;
	      //holder_token = T_goto;
	      //putback
	    }else{
	      scan_error(temp3);
	      //error
	    }
	  } else{
	    scan_error(temp2);
	  }
	}else{
	  //cin.putback(temp);
	  scan_error(temp);
	}
	break;}	
      case 'i': // case if	        
	{char temp = getchar(); 
	if (cin.peek() == 'f'){
	  cin.putback(temp); 
	  return T_if;
	} else{
	  //	cin.putback(temp);
	  scan_error(temp);
	}
	break;}	
      case 'p':	//case print
	{char temp, temp2, temp3, temp4;
	temp = getchar();
	if (cin.peek() == 'r'){
	  temp2 = getchar();
	  if (cin.peek() == 'i'){
	    temp3 = getchar();
	    if (cin.peek() == 'n'){
	      temp4 = getchar();
	      if (cin.peek() == 't'){
		cin.putback(temp4);
		cin.putback(temp3);
		cin.putback(temp2);
		cin.putback(temp);
		return T_print;
		//holder_token = T_print; 
	      }else{
		scan_error(temp4);
	      }
	    }else{
	      scan_error(temp3);
	    }	  
	  } else{
	    scan_error(temp2);
	  }
	}else{
	  scan_error(temp);
	}      
	break;}	
      case 'L':	//case label
	{return T_label;
	  /*
char temp, temp2, temp3, temp4;
	temp = getchar();
	if (cin.peek() == 'a'){
	  temp2 = getchar();
	  if (cin.peek() == 'b'){
	    temp3 = getchar();
	    if (cin.peek() == 'e'){
	      temp4 = getchar();
	      if (cin.peek() == 'l'){
		cin.putback(temp4);
		cin.putback(temp3);
		cin.putback(temp2);
		cin.putback(temp);
		return T_label; 
	      }else{
		scan_error(temp4);
	      }
	    }else{
	      scan_error(temp3);
	    }	  
	  } else{
	    scan_error(temp2);
	  }
	}else{
	  scan_error(temp);
	  } */
	break;}	
      case ':':{	
	return T_colon; 
	break;	}
      case '\n' :{
	char eat = getchar();
	linenumber++;
	break;}

      case ' ': {
	getchar();
	break;
      }
      case EOF:{
	return T_eof;
	break;
      }
      default: scan_error(c);
	break;
      }
  }
}

void scanner_t::eat_token(token_type c)
{
  //if we are supposed to eat token c, and it does not match
  //what we are supposed to be reading from file, then it is a 
  //mismatch error ( call - mismatch_error(c) )
  
  //WRITEME: cut this bogus stuff out and implement eat_token
  //if ( rand()%10 < 8 ) bogo_token = T_plus;
  //else bogo_token = T_eof;
  //cout << "ABOUT TO EAT" << endl;
  //cout << token_to_string(c) << endl;

  int x;
  token_type holder = next_token();
  //cout << token_to_string(holder) << endl;
    if (holder == c){
      if (holder == T_print){
	x = 0;
	while (x < 5){
	  getchar();
	  x++;
	}
      }
      else if (holder == T_goto){
        x = 0;
        while (x < 4){
          getchar();
          x++;
        }
      } 
      else if (holder == T_if){
          getchar();
          getchar();
      } 
      else if (holder == T_num){ // could cause error?
	char a[9];
	int i;
	string Text;
	int y;
	int b = 0;
	i = 0;
	while (isdigit(cin.peek())){
	  char tempt = getchar();
	  Text += tempt;
	}
	int max1; 
	max1= 2147483647;
	y = atoi(Text.c_str());
	if (y <= max1 && y < 0){
	  range_error(c);
	}
      }
      else if (holder == T_power){
	getchar();
	getchar();
      }
      else {
	getchar();
      }
      
    }else {
      mismatch_error(c);
    }
    //number, power
    //number have to combine then check
}

scanner_t::scanner_t()
{
  //WRITEME: this is the scanner constructor
  linenumber = 1;
  
}

int scanner_t::get_line()
{
  //WRITEME: this function should return the current line number (used
  //for error reporting
  return linenumber;
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
 void scanner_t::range_error(token_type x)
{
	printf("range error %s - line %d\n", token_to_string(x), get_line());
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
  void Statementsprime();
  void Statement();
  void Label();
  void Jump();
  void Assignment();
  void Print();
  void Expression();
  void Eprime();
  void D();
  void Dprime();
  void C();
  void Cprime();
  void Cprime2();
  void B();
  void A();
  void F();
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
    case T_label:
      Statements();
      break;
    case T_goto:
      Statements();
      break;
    case T_m:
      Statements();
      break;
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
  
}
//WRITEME: you will need to put the rest of the procedures here
void parser_t::Statements(){
  parsetree.push(NT_Statements);
  Statement();

switch( scanner.next_token() ) 
  {    
  case T_label:
  case T_goto:
  case T_m: 
  case T_print:
    Statementsprime();
    break;
    
    // case EOF:
    // parsetree.drawepsilon();
    // break;

 default:
   syntax_error(NT_Statements);
      break; 
  }
 parsetree.pop();
      
  }
void parser_t::Statementsprime(){
  parsetree.push(NT_Statementsprime);
 switch( scanner.next_token() ) 
    {
    case T_label:
      // Label();
      // break;
    case T_goto:
      // Jump();
      // break;
      //ISSUES?
    case T_m:
      //ASSIGNMENT OR A?
      //Assignment();
      //break;
    case T_print:
      // Print();
      //break;
      Statement();
      Statementsprime();
      break;

    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_Statementsprime);
      break;
    } 
  parsetree.pop();    
}
//first sets if no ep, first then follow if ep
void parser_t::Statement(){
  parsetree.push(NT_Statement);
  switch( scanner.next_token() ) 
    {
    case T_label:
      Label();
      break;
    case T_goto:
      Jump();
      break;
      //ISSUES?
    case T_m:
      //ASSIGNMENT OR A?
      Assignment();
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

void parser_t::Label(){
  parsetree.push(NT_Label);
    eat_token(T_label);
  
    eat_token(T_num);
   
    eat_token(T_colon);
   
  parsetree.pop();
}

void parser_t::Jump(){	
  parsetree.push(NT_Jump);
    eat_token(T_goto);
   
    eat_token(T_num);
    
  if (scanner.next_token() == T_if){
    eat_token(T_if);
    Expression();
  }
  
  parsetree.pop();  
}

void parser_t::Assignment(){
  parsetree.push(NT_Assignment);
  eat_token(T_m);
  eat_token(T_opensquare);
  Expression();
  eat_token(T_closesquare);
  eat_token(T_equals);
  Expression();
  parsetree.pop();  
}

void parser_t::Print(){	
  parsetree.push(NT_Print);
    eat_token(T_print);
    Expression();  
   
  
  parsetree.pop();
  
}
void parser_t::Expression(){
  parsetree.push(NT_Expression);
  switch( scanner.next_token() ) 
    {
    case T_m:
    case T_num:
    case T_openparen:
      D();
      Eprime();
      break;
    default:
      syntax_error(NT_Expression);
      break;
    }
  parsetree.pop();
}
void parser_t::Eprime(){
  parsetree.push(NT_Eprime);
 switch( scanner.next_token() ) 
    {
    case T_plus:
      eat_token(T_plus);
      D();
      Eprime();
      break;
    case T_minus:
      eat_token(T_minus);
      D();
      Eprime();
      break;
    case T_eof:
     parsetree.drawepsilon();
      break;
      //Part 3 
    default:
      //syntax_error(NT_Eprime);
      parsetree.drawepsilon();
      break;
    }
  parsetree.pop();  
}

void parser_t::D(){
  parsetree.push(NT_D);
  switch( scanner.next_token() ) 
    {
    case T_m:
    case T_num:
    case T_openparen:
      C();
      Dprime();
      break;
    case T_plus:
    case T_minus:
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_D);
      break;
    }

  parsetree.pop();  
}

void parser_t::Dprime(){
  parsetree.push(NT_Dprime);
  switch( scanner.next_token() ) 
    {
    case T_times:
      eat_token(T_times);
      C();
      Dprime();
      break;
    case T_divide:
      eat_token(T_divide);
      C();
      Dprime();
      break;
      /* case T_eof:
     parsetree.drawepsilon();
     break;
      */
    case T_plus:
    case T_minus:
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      //syntax_error(NT_Dprime);
      parsetree.drawepsilon();
      break;
    }

  parsetree.pop();  
}

void parser_t::C(){
  parsetree.push(NT_C);
  switch (scanner.next_token()){
  case (T_openparen):  
  case (T_num):
  case (T_m):
    B();
  Cprime();
  break;
  case T_plus:
  case T_minus:
  case T_eof:
  case T_times:
  case T_divide:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_C);
    break;
  }
  parsetree.pop();  
}

void parser_t::Cprime(){
  parsetree.push(NT_Cprime);
  switch (scanner.next_token())
    {
    case T_power:
      eat_token(T_power);
      B();
      Cprime();
      break;
    
      /* case T_eof:
      parsetree.drawepsilon();
      break;*/
    case T_plus:
    case T_minus:
    case T_eof:
    case T_times:
    case T_divide:
      parsetree.drawepsilon();
      break;
    default:
      parsetree.drawepsilon();
      break;
    }
  parsetree.pop();  
}


void parser_t::B(){
  parsetree.push(NT_B);
  switch (scanner.next_token()){
  case T_openparen:  
    eat_token(T_openparen);
    Expression();
    eat_token(T_closeparen);
    break;
  case T_num:
  case T_m:
    A();
  break;  
  case T_plus:
  case T_minus:
  case T_eof:
  case T_times:
  case T_divide:
    parsetree.drawepsilon();
    break;
  }  
  parsetree.pop();  
}

void parser_t::A(){
  parsetree.push(NT_A);
  switch (scanner.next_token()){
  case T_m:
    eat_token(T_m);
    eat_token(T_opensquare);
    Expression();
    eat_token(T_closesquare);
    break;
  case T_num:
    F();
    break;
  }
  parsetree.pop();  
}

void parser_t::F(){
  parsetree.push(NT_F);
  eat_token(T_num);
  parsetree.pop();  
}
/*** Main ***********************************************/

int main()
{
  /* scanner_t scanner;
     token_type tok = scanner.next_token();
     while (tok != T_eof){
     scanner.eat_token(tok);
     printf("%s ", token_to_string(tok));
     tok = scanner.next_token();
     }printf("%s\n", token_to_string(tok));*/
  parser_t parser;
  parser.parse();
  return 0;
}
/*
	while(1){
	char temp,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,temp0;
	temp = getchar();
	if (isdigit(cin.peek())){
	  temp2 = getchar();
	    if (isdigit(cin.peek())){
	    temp3 = getchar();
	    if (isdigit(cin.peek())){
	      temp4 = getchar();
	      if (isdigit(cin.peek())){
		temp5 = getchar();
		if (isdigit(cin.peek())){
		  temp6 = getchar();
		  if (isdigit(cin.peek())){
		    temp7 = getchar();
		    if (isdigit(cin.peek())){
		      temp8 = getchar();
		      if (isdigit(cin.peek())){
			temp9 = getchar();
			if (isdigit(cin.peek())){
			  temp0 = getchar();
			}else{
			  string result = temp0 + temp9 + temp8 + temp7 +temp6 + temp5 + temp4 + temp3 +temp2 + temp;
			  cin.putback(temp0);
			  cin.putback(temp9);
			  cin.putback(temp8);
			  cin.putback(temp7);
			  cin.putback(temp6);
			  cin.putback(temp5);
			  cin.putback(temp4);
			  cin.putback(temp3);
			  cin.putback(temp2);
			  cin.putback(temp);
			}
		      }
		      else{
			string result = temp9 + temp8 + temp7 +temp6 + temp5 + temp4 + temp3 +temp2 + temp;
			cin.putback(temp9);
			cin.putback(temp8);
			cin.putback(temp7);
			cin.putback(temp6);
			cin.putback(temp5);
			cin.putback(temp4);
			cin.putback(temp3);
			cin.putback(temp2);
			cin.putback(temp);}
		    }else{string result = temp8 + temp7 +temp6 + temp5 + temp4 + temp3 +temp2 + temp;
		      cin.putback(temp8);
		      cin.putback(temp7);
		      cin.putback(temp6);
		      cin.putback(temp5);
		      cin.putback(temp4);
		      cin.putback(temp3);
		      cin.putback(temp2);
		      cin.putback(temp);}
		  }else{string result = temp7 + temp6 + temp5 + temp4 + temp3 +temp2 + temp;
		    cin.putback(temp7);
		    cin.putback(temp6);
		    cin.putback(temp5);
		    cin.putback(temp4);
		    cin.putback(temp3);
		    cin.putback(temp2);
		    cin.putback(temp);
		  }
		}else{string result = temp6 + temp5 + temp4 + temp3 +temp2 + temp;
		  cin.putback(temp6);
		  cin.putback(temp5);
		  cin.putback(temp4);
		  cin.putback(temp3);
		  cin.putback(temp2);
		  cin.putback(temp);	}
	      }else{string result = temp5 + temp4 + temp3 + temp2 +temp;
		cin.putback(temp5);
		cin.putback(temp4);
		cin.putback(temp3);
		cin.putback(temp2);
		cin.putback(temp);     }
	    }else{string result = temp4 + temp3 + temp2 + temp;
	      cin.putback(temp4);
	      cin.putback(temp3);
	      cin.putback(temp2);
	      cin.putback(temp);}
	  }else{string result = temp3 + temp2 + temp;
	    cin.putback(temp3);
	    cin.putback(temp2);
	    cin.putback(temp);}
	}else{string result = temp2 + temp;
	  
	  cin.putback(temp2);
	  cin.putback(temp);
	}
	else{string result = temp;
	  cin.putback(temp);
	}
*/	
