//Gordon Cheung
//5849039
//cs160

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <cstdio>
#include <iostream>
#include <istream>

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
	NT_T,
	NT_En,
	NT_Tn,
	NT_M,
	NT_Mn,
	NT_P
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
		    case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
	case NT_T: strncpy(buffer,"T",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Tn: strncpy(buffer,"Tn",MAX_SYMBOL_NAME_SIZE); break;
	case NT_En: strncpy(buffer,"En",MAX_SYMBOL_NAME_SIZE); break;
	case NT_M: strncpy(buffer,"M",MAX_SYMBOL_NAME_SIZE); break;
	case NT_Mn: strncpy(buffer,"Mn",MAX_SYMBOL_NAME_SIZE); break;
	case NT_P: strncpy(buffer,"P",MAX_SYMBOL_NAME_SIZE); break;

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
  int getcounter();
	//return line number for errors
	int get_line();
  // int getcurrent();
	//constructor 
	scanner_t();

  private:
  int counter;
  int linenumber;
  //int current;
	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  
  void range_error();
	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

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
  char temp = cin.peek();
  //cout<<"anything"<<endl;
  if(temp == '+')return T_plus;
  else if(temp == '-')return T_minus;
  else if(temp == '*'){char x=cin.get();if(cin.peek()=='*'){cin.unget();return T_power;} else {cin.unget();return T_times;}}

  //else if(temp == '*'){return T_times;}
  else if(temp == '/')return T_divide;
  //else if(temp == '**')return T_power;
  else if(temp == '=')return T_equals;
  else if(temp == '(')return T_openparen;
  else if(temp == ')')return T_closeparen;
  else if(temp == '[')return T_opensquare;
  else if(temp == ']')return T_closesquare;
  else if(temp == 'm'){return T_m;}
  else if(isdigit(temp)){
    return T_num;}
  else if(temp,cin.eof())return T_eof;
  else if(temp == ' '){getchar(); next_token();}
  else if(temp == '\n'){linenumber++;cin.get();cout<<endl; next_token();}
  else if(temp == ':'){return T_colon;}
  else if(temp == 'L'){return T_label;}
  else if(temp == 'p'){
    return T_print;
  
    
  }
  else if(temp == 'g'){
    return T_goto;
   
  }
  else if(temp == 'i'){
    return T_if;
   
  }
  else scan_error(temp);

 
}

void scanner_t::eat_token(token_type c)
{
	
  //char temp = getchar();
   char temp = cin.peek();
   //if(next_token() != c)mismatch_error(c); 
  //cout<<"anything"<<endl;
   if(c == T_plus)getchar();
   else if(c== T_minus)getchar();
   else if(c== T_times){getchar();}
   else if(c== T_divide)getchar();
   else if(c== T_power){getchar();getchar();}
   else if(c== T_equals)getchar();
   else if(c== T_openparen)getchar();
   else if(c== T_closeparen)getchar();
   else if(c== T_opensquare)getchar();
   else if(c== T_closesquare)getchar();
   else if(c== T_m){getchar();}
   else if(c== T_num){
     int x;
     cin>>x;
     if(x < 0 || x > 2147483647)range_error();
     else counter=x;
   
   }
   else if(c== T_eof)getchar();
  else if(temp == ' '){getchar(); next_token();}  else if(temp == '\n'){linenumber++;cin.get();cout<<endl; next_token();}
  else if(c== T_colon){getchar();}
  else if(c== T_label){getchar();}
  else if(temp == 'p'){
    char use = getchar();
    use=getchar();
    if(use != 'r')scan_error('r');
    use=getchar();
    if(use != 'i')scan_error('i');
    use=getchar();
    if(use != 'n')scan_error('n');
    use=getchar();
    if(use != 't')scan_error('t');
  }
  else if(temp == 'g'){
    char use = getchar();
    use=getchar();
    if(use != 'o')scan_error('o');
    use=getchar();
    if(use != 't')scan_error('t');
    use=getchar();
    if(use != 'o')scan_error('o');
  }
  else if(temp == 'i'){
    char use = getchar();
    use=getchar();
    if(use != 'f')scan_error('f');
  }
  else if(next_token()!=c)mismatch_error(c);
  else 
    getchar();
  
	//WRITEME: cut this bogus stuff out and implement eat_token
	//if ( rand()%10 < 8 ) bogo_token = T_plus;
	//else bogo_token = T_eof;

}

scanner_t::scanner_t()
{
  linenumber = 1;
	//WRITEME: this is the scanner constructor
}

int scanner_t::getcounter()
{
  return counter;
}

int scanner_t::get_line()
{
  return linenumber;
	//WRITEME: this function should return the current line number (used
	//for error reporting
}
void scanner_t::range_error(){
  printf("range error, number is not between 0 and 2^31 -1");
  exit(1);
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
	void Statement();
	void Label();
	void Jump();
	void Assignment();
	void Print();
	void Expression();
	void En();
	void T();
	void Tn();
	void M();
	void Mn();
	void P();
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
	cerr<< "#include <stdio.h>"<<endl;
	cerr<< "#include <math.h>"<<endl;
	cerr<< "int main(){" << endl;
	cerr<< "int m[101];"<<endl;
	Statements();
	/*
	switch( scanner.next_token() ) 
	{
		case T_openparen:
			eat_token(T_openparen);
			Start();
			break;
		case T_m:
			eat_token(T_m);
			
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Start);
			break;
	}
	*/
	cerr<< "}"<<endl;

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Statements(){
  parsetree.push(NT_Statements);
  switch(scanner.next_token()){
  case T_label:
    Statement();
    cerr<<endl;
    Statements();
    break;
  case T_goto:
    Statement();
    cerr<<";"<<endl;
    Statements();
    break;
  case T_m:
    Statement();
    cerr<<";"<<endl;
    Statements();
    break;
  case T_print:
    Statement();
    cerr<<";"<<endl;
    Statements();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Statements);
    break;
  }
  parsetree.pop();
}

void parser_t::Statement(){
  parsetree.push(NT_Statement);
  // cout<<"This is the token: "<< token_to_string(scanner.next_token())<<endl;
  switch(scanner.next_token()){
  case T_label:
    //eat_token(T_label);
    Label();
    break;
  case T_goto:
    //eat_token(T_goto);
    Jump();
    break;
  case T_m:
    //eat_token(T_m);
    Assignment();
    break;
  case T_print:
    //eat_token(T_print);
    Print();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Statement);
    break;
  }
  parsetree.pop();
}
void parser_t::Label(){
  parsetree.push(NT_Label);
  switch(scanner.next_token()){
  case T_label:
    eat_token(T_label);
    cerr<<"L";
    eat_token(T_num);
    cerr<<scanner.getcounter();
    eat_token(T_colon);
    cerr<<":";
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

void parser_t::Jump(){
  parsetree.push(NT_Jump);
  switch(scanner.next_token()){
  case T_goto:
    eat_token(T_goto);
    scanner.next_token();
    eat_token(T_num); 
    int z;
    if(scanner.getcounter()!=0)z=scanner.getcounter();
    scanner.next_token();
    if(scanner.next_token()==T_if){eat_token(T_if);cerr<<"if(";Expression();cerr<<")";}
    cerr<<"goto ";//<<scanner.getcounter();
    if(z!=0)cerr<<"L"<<z;
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Jump);
    break;
  }
  parsetree.pop();
}

void parser_t::Assignment(){
  parsetree.push(NT_Assignment);
  //cout<<"hello"<<endl;
  switch(scanner.next_token()){
  case T_m:
    eat_token(T_m);
    eat_token(T_opensquare);
    cerr<< "m[";
    Expression();
    eat_token(T_closesquare);
    cerr<<"]";
    scanner.next_token();
    //cout<<"asdasda::: "<<token_to_string(scanner.next_token())<<endl;
    //cout<<"asdasda::: "<<token_to_string(scanner.next_token())<<endl;

    eat_token(T_equals);
    cerr<<"=";
    //cout<<"asdkjhads"<<endl;
    Expression();
    break;
    /*
  case T_closesquare:
    parsetree.drawepsilon();
    break;
 */
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Assignment);
    break;
  }
  parsetree.pop();
}

void parser_t::Print(){
  parsetree.push(NT_Print);
  switch(scanner.next_token()){
  case T_print:
    eat_token(T_print);
    cerr<<"printf(\"%i\",";
    Expression();
    cerr<<")";
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Print);
    break;
  }
  parsetree.pop();
}	

void parser_t::Expression(){
  parsetree.push(NT_Expression);
  switch(scanner.next_token()){
  case T_openparen:
    T();
    En();
    break;
  case T_m:
    T();
    En();
    break;
  case T_num:
    T();
    En();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    //eat_token(T_closesquare);
    break;
  case T_closeparen:
    parsetree.drawepsilon();
    break;
  default:
    syntax_error(NT_Expression);
    break;	
  }
  parsetree.pop();	
}

void parser_t::T(){
  parsetree.push(NT_T);
  switch(scanner.next_token()){
  case T_plus:
    parsetree.drawepsilon();
    break;
  case T_minus:
    parsetree.drawepsilon();
    break;
  case T_num:
    //eat_token(T_num);
    M();
    Tn();
    break;
  case T_openparen:
    //eat_token(T_openparen);
    M();
    Tn();
    //eat_token(T_closeparen);
    break;
  case T_m:
    //eat_token(T_m);
    //eat_token(T_opensquare);
    M();
    Tn();
    //eat_token(T_closesquare);
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

    syntax_error(NT_T);
    break;
  }
  parsetree.pop();
}

void parser_t::En(){
  parsetree.push(NT_En);
  switch(scanner.next_token()){
  case T_plus:
    eat_token(T_plus);
    cerr<<"+";
    T();
    En();
    break;
  case T_minus:
    eat_token(T_minus);
    cerr<<"-";
    T();
    En();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  case T_closeparen:
    parsetree.drawepsilon();
    break;
  case T_m:
    parsetree.drawepsilon();
    break;
  case T_goto:
    parsetree.drawepsilon();
    break;
  case T_label:
    parsetree.drawepsilon();
    break;
  case T_print:
    parsetree.drawepsilon();
    break;
  default:
    //parsetree.drawepsilon();
    syntax_error(NT_En);
    break;
  }
    parsetree.pop();
}

void parser_t::Tn(){
  parsetree.push(NT_Tn);
  switch(scanner.next_token()){
  case T_plus:
    parsetree.drawepsilon();
    break;
  case T_minus:
    parsetree.drawepsilon();
    break;
  case T_times:
    eat_token(T_times);
    cerr<<"*";
    M();
    Tn();
    break;
  case T_divide:
    eat_token(T_divide);
    cerr<<"/";
    M();
    Tn();
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  case T_closeparen:
    parsetree.drawepsilon();
    break;
  case T_m:
    parsetree.drawepsilon();
    break;
  case T_goto:
    parsetree.drawepsilon();
    break;
  case T_label:
    parsetree.drawepsilon();
    break;
  case T_print:
    parsetree.drawepsilon();
    break;
  default:
    //parsetree.drawepsilon();
    syntax_error(NT_Tn);
    break;
  }
  parsetree.pop();
}

void parser_t::M(){
  parsetree.push(NT_En);
  switch(scanner.next_token()){
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
  case T_openparen:
    P();
    Mn();
    break;
  case T_m:
    P();
    Mn();
    break;
  case T_num:
    P();
    Mn();
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
    syntax_error(NT_M);
    break;
  }
  parsetree.pop();
}

void parser_t::Mn(){
  parsetree.push(NT_Mn);
  switch(scanner.next_token()){
  case T_power:
    eat_token(T_power);
    cerr<<",";
    P();
    //cerr<<")";
    Mn();
    cerr<<")";
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  case T_closeparen:
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
  case T_m:
    parsetree.drawepsilon();
    break;
  case T_goto:
    parsetree.drawepsilon();
    break;
  case T_label:
    parsetree.drawepsilon();
    break;
  case T_print:
    parsetree.drawepsilon();
    break;
  default:
    //parsetree.drawepsilon();
    syntax_error(NT_Mn);
    break;
  }
  parsetree.pop();
}

void parser_t::P(){
  parsetree.push(NT_P);
  //cout<<token_to_string(scanner.next_token())<<endl;

  switch(scanner.next_token()){
  case T_openparen:
    eat_token(T_openparen);
    cerr<<"(";
    Expression();
    eat_token(T_closeparen);
    cerr<<")";
    break;
  case T_m:
    //cout<<"made it here atleast"<<endl;
    eat_token(T_m);
    eat_token(T_opensquare);
    cerr<<"m[";
    //cout<<"helloasdsad"<<endl;
    Expression();
    //cout<<"after expression"<<endl;
    eat_token(T_closesquare);
    cerr<<"]";
    break;
  case T_num:
    eat_token(T_num);
    if(scanner.next_token()!=T_power)
    cerr<<scanner.getcounter();
    else cerr<<"pow("<<scanner.getcounter();
    //cerr<<"num";
    //cout<<"askjdhasdh"<<endl;
    break;
  case T_eof:
    parsetree.drawepsilon();
    break;
  case T_closesquare:
    parsetree.drawepsilon();
    break;
  case T_closeparen:
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
  default:
    syntax_error(NT_P);
    break;
  }
  parsetree.pop();
}


/*** Main ***********************************************/

int main()
{
   parser_t parser;
  	parser.parse();

	return 0;
}
