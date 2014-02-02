#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <istream>
#include <cctype>
#include <stack>

using namespace std;

int count;
/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25

//all of the terminals in the language
typedef enum {
  T_eof = 0,// 0: end of file
  T_num,// 1: numbers
  T_plus,// 2: +
  T_minus,// 3: -
  T_times,// 4: *
  T_divide,// 5: /
  T_power, // 6: **
  T_equals,// 7: =
  T_openparen,// 8: (
  T_closeparen, // 9: )
  T_opensquare,// 10: [
  T_closesquare,// 11: ]
  T_m,// 12: m
  T_goto,// 13: goto
  T_if,// 14: if
  T_print,// 15: print
  T_label,// 16: L
  T_colon// 17: :
} token_type;

//this function returns a string for the token.  It is used in the parsetree_t
//class to actually dump the parsetree to a dot file (which can then be turned
//into a picture).  Note that the return char* is a reference to a local copy
//and it needs to be duplicated if you are a going require multiple instances
//simultaniously
char* token_to_string(token_type c) {
  static char buffer[MAX_SYMBOL_NAME_SIZE];
  switch( c ) {
  case T_eof:strncpy(buffer,"eof",MAX_SYMBOL_NAME_SIZE); break;
  case T_num:strncpy(buffer,"num",MAX_SYMBOL_NAME_SIZE); break;
  case T_plus:strncpy(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
  case T_minus:strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
  case T_times:strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
  case T_divide:strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
  case T_power:strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
  case T_equals:strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
  case T_openparen:strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
  case T_closeparen:strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
  case T_opensquare:strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
  case T_closesquare:strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
  case T_m:strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
  case T_goto:strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
  case T_if:strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
  case T_print:strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
  case T_label:strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
  case T_colon:strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
  default: strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
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
  NT_Statement,
  NT_Expression,
  NT_Label,
  NT_Jump,
  NT_Jump_prime,
  NT_Assign,
  NT_Print,
  NT_Exp,
  NT_Exp_prime,
  NT_Term,
  NT_Term_prime,
  NT_Factor,
  NT_Factor_prime,
  NT_B,
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
  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Jump_prime: strncpy(buffer,"Jump_prime",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Assign: strncpy(buffer,"Assign",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Exp: strncpy(buffer,"Exp",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Exp_prime: strncpy(buffer,"Exp_prime",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Term_prime: strncpy(buffer,"Term_prime",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Factor_prime: strncpy(buffer,"Factor_prime",MAX_SYMBOL_NAME_SIZE); break;
  case NT_B: strncpy(buffer,"B",MAX_SYMBOL_NAME_SIZE); break;
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

  
  token_type tokenizeEat(char c){

	  token_type token;

		  switch (c){
		  case EOF:
			  token=T_eof; break; 
		  case '*':
			  if (cin.peek()=='*'){
				  getchar();
				  token=T_power; break;
			  }else{
				  token=T_times;
				  break;
			  }

		  case '+':
			  token=T_plus;break;
		  case '-':
			  token=T_minus; break;
		  case '/':
			  token=T_divide; break;
		  case '=':
			  token=T_equals; break;
		  case '(':
			  token=T_openparen; break;
		  case ')':
			  token=T_closeparen; break;
		  case '[':
			  token=T_opensquare; break;
		  case ']':
			  token=T_closesquare; break;
		  case 'm':
			  token=T_m; break;
		  case 'g':
				 if (cin.peek()=='o'){
					c=getchar();
					if(cin.peek()=='t'){
						c=getchar();
						 if(cin.peek()=='o'){
							 getchar();
							  token=T_goto; break;
						}else{
							cin.putback(c);
							 mismatch_error(T_goto); break;
						  }
					 }else{
						 cin.putback(c);
						 mismatch_error(T_goto); break;  
					  }
				 }else{
				 	 mismatch_error(T_goto);
				 	 break;		  
				 }
		  case 'i':
			  if (cin.peek()=='f'){
				  getchar();
				  token=T_if; break;
			  }else
			  	  mismatch_error(T_if); break;
		  case 'p':
			  if (cin.peek()=='r'){
				  c=getchar();
				  if (cin.peek()=='i'){
					  c=getchar();
					  if(cin.peek()=='n'){
						  c=getchar();
						  if(cin.peek()=='t'){
							  getchar();
							  token=T_print; break;
						  }else{
							  cin.putback(c);
							  mismatch_error(T_print); break;
						  }
					  }else{
						  cin.putback(c);
						  mismatch_error(T_print); break;  
					  }
				  }else{
					  cin.putback(c);
					  mismatch_error(T_print); break;
				  }
			  }else{
			  		mismatch_error(T_print); break;
			  }
		  case 'L':
			  token=T_label; break;
		  case ':':
			  token=T_colon; break;
		  default:
			  if (isdigit(c)){
			  	 if (isdigit(cin.peek())){
			  		 c=getchar();
			  		 if(isdigit(cin.peek())){
			  			 c=getchar();							
			  			 token=T_num; break;
			  			 if(isdigit(cin.peek())){
			  				 scan_error(cin.peek());
			  				 break;
			  			 }
			  				 
			  		 }else{
			  			 token=T_num; break;  
			  		 }
			  	 }else{
			  		 token=T_num; break;
			  	 }
			  }else
				  scan_error(c); break;
		  }
		  return token;
	  
		  
	  
  }

  token_type tokenizePeek(char c){

	  token_type token;
	  char c1,c2,c3,c4;

		  switch (c){
		  case EOF:
			  token=T_eof; break; 
		  case '*':
			  c=getchar();
			  if (cin.peek()=='*'){
				  cin.putback(c);
				  token=T_power; break;
			  }else{
				  cin.putback(c);
				  token=T_times;
				  break;
			  }

		  case '+':
			  token=T_plus;break;
		  case '-':
			  token=T_minus; break;
		  case '/':
			  token=T_divide; break;
		  case '=':
			  token=T_equals; break;
		  case '(':
			  token=T_openparen; break;
		  case ')':
			  token=T_closeparen; break;
		  case '[':
			  token=T_opensquare; break;
		  case ']':
			  token=T_closesquare; break;
		  case 'm':
			  token=T_m; break;
		  case 'g':
			  c1=getchar();
				 if (cin.peek()=='o'){
					c2=getchar();
					if(cin.peek()=='t'){
						c3=getchar();
						 if(cin.peek()=='o'){
							  cin.putback(c3);
							  cin.putback(c2);
							  cin.putback(c1);
							  token=T_goto; break;
						}else{
							  cin.putback(c3);
							  cin.putback(c2);
							  cin.putback(c1);
							 mismatch_error(T_goto); break;
						  }
					 }else{
						  cin.putback(c2);
						  cin.putback(c1);
						 mismatch_error(T_goto); break;  
					  }
				 }else{
					cin.putback(c1);
				 	 mismatch_error(T_goto);
				 	 break;		  
				 }
		  case 'i':
			  c=getchar();
			  if (cin.peek()=='f'){
				  cin.putback(c);
				  token=T_if; break;
			  }else
				  cin.putback(c);
			  	  mismatch_error(T_if); break;
		  case 'p':
			  c1=getchar();
			  if (cin.peek()=='r'){
				  c2=getchar();
				  if (cin.peek()=='i'){
					  c3=getchar();
					  if(cin.peek()=='n'){
						  c4=getchar();
						  if(cin.peek()=='t'){
							  cin.putback(c4);
							  cin.putback(c3);
							  cin.putback(c2);
							  cin.putback(c1);
							  token=T_print; break;
						  }else{
							  cin.putback(c4);
							  cin.putback(c3);
							  cin.putback(c2);
							  cin.putback(c1);
							  mismatch_error(T_print); break;
						  }
					  }else{
						  cin.putback(c3);
						  cin.putback(c2);
						  cin.putback(c1);
						  mismatch_error(T_print); break;  
					  }
				  }else{
					  cin.putback(c2);
					  cin.putback(c1);
					  mismatch_error(T_print); break;
				  }
			  }else{
				  cin.putback(c1);
			  		mismatch_error(T_print); break;
			  }
		  case 'L':
			  token=T_label; break;
		  case ':':
			  token=T_colon; break;
		  default:
			  if (isdigit(c)){
				  c1=getchar();
				  if (isdigit(cin.peek())){
					  c2=getchar();
						if (isdigit(cin.peek())){
								c3=getchar();
					  			 if(isdigit(cin.peek())){
					  				 scan_error(cin.peek());
					  				 break;
					  			 }
					  			 cin.putback(c3);
							  cin.putback(c2);
							  cin.putback(c1);
								token=T_num; 
								break;	//3 digits
						  }else{
							  cin.putback(c2);
							  cin.putback(c1);
							  token=T_num; 
							  break; //2 digits
						  }
				 }else{ 
					 cin.putback(c1);
					 token=T_num; 
					 break; //1 digit 
				 } 
				 
			  }else
				  scan_error(c); break;
		  }
		  return token;
	  
		  
	  
  }
  //error message and exit for unrecognized character
  void scan_error(char x);
  //error message and exit for token mismatch
	  //printf("scan error: unrecognized character '%c' -line %d\n",x, get_line());

  void mismatch_error(token_type c);
  token_type bogo_token;

};

token_type scanner_t::next_token()
{
	char x;
	while (cin.peek()==' '||cin.peek()=='\n'||cin.peek()=='\t'){
		x=getchar(); //eat white space
		if (x=='\n')
			count++;
	}
	return tokenizePeek(cin.peek());
}

void scanner_t::eat_token(token_type c)
{
	char x;
	while (cin.peek()==' '||cin.peek()=='\n'||cin.peek()=='\t'){
			x=getchar(); //eat white space
			if (x=='\n')
				count++;
		}
if(tokenizeEat(getchar())!=c)
	this->mismatch_error(c);
}

scanner_t::scanner_t()
{
	count=1;
}

int scanner_t::get_line()
{
  return count;
}

void scanner_t::scan_error (char x)
{
  printf("\nscan error: unrecognized character '%c' -line %d\n",x, get_line());
  exit(1);

}

void scanner_t::mismatch_error (token_type x)
{
  printf("\nsyntax error: found %s ",token_to_string(next_token()) );
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
void Statement();
void Expression();
void Label();
void Jump();
void Jump_prime();
void Assign();
void Print();
void Exp();
void Exp_prime();
void Term();
void Term_prime();
void Factor();
void Factor_prime();
void B();
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
//to parse the entire input, all of which can be derived from the start
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

switch( scanner.next_token() ) 
{
case T_print:
	Print();
	Statement();

break;
case T_label:
	Label();
	Statement();

	break;
case T_goto:
	Jump();
	Statement();

	break;
case T_m:
	Assign();
	Statement();

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

switch( scanner.next_token() ) 
{
case T_print:
	Print();
	Statement();

break;
case T_label:
	Label();
	Statement();

	break;
case T_goto:
	Jump();
	Statement();

	break;
case T_m:
	Assign();
	Statement();
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

/*
void parser_t::Expression()
{
	parsetree.push(NT_Expression);

	switch( scanner.next_token() ) 
	{
	case T_label:
		Label();
		break;
	case T_goto:
		break;
	case T_m:
		break;
	case T_print:
		break;
	default:
		syntax_error(NT_Expression);
	break;
	}
	parsetree.pop();
}*/

void parser_t::Label()
{
	parsetree.push(NT_Label);

	switch( scanner.next_token() ) 
	{
	case T_label:
		eat_token(T_label);
		eat_token(T_num);
		eat_token(T_colon);

	break;
	default:
			syntax_error(NT_Expression);
		break;
	}
	parsetree.pop();
}

void parser_t::Jump()
{
	parsetree.push(NT_Jump);

	switch( scanner.next_token() ) 
	{
	case T_goto:
		eat_token(T_goto);
		eat_token(T_num);
		Jump_prime();

		break;
	default:
			syntax_error(NT_Jump);
		break;
	}
	parsetree.pop();
}

void parser_t::Jump_prime()
{
	parsetree.push(NT_Jump);

	switch( scanner.next_token() ) 
	{
	case T_if:
		// jump and num token previously eaten
		eat_token(T_if);
		Exp();
		break;
	case T_eof:
	case T_print:
	case T_label:
	case T_m:
	case T_goto:
		// jump and num token previously eaten
		parsetree.drawepsilon();
		break;
		
	default:
		syntax_error(NT_Jump_prime);
		break;
	}
	parsetree.pop();
}

void parser_t::Assign()
{
	parsetree.push(NT_Assign);

	switch( scanner.next_token() ) 
	{
	case T_m:
		eat_token(T_m);
		eat_token(T_opensquare);
		Exp();
		eat_token(T_closesquare);
		eat_token(T_equals);
		Exp();
		break;
	default:
			syntax_error(NT_Assign);
		break;
	}
	parsetree.pop();
}

void parser_t::Print()
{
	parsetree.push(NT_Print);
	switch( scanner.next_token() ) 
	{
	case T_print:
		eat_token(T_print);
		Exp();
		break;
	default:
		syntax_error(NT_Print);
		break;
	}
	parsetree.pop();
}

void parser_t::Exp()
{
	parsetree.push(NT_Exp);

	switch( scanner.next_token() ) 
	{
	case T_num:
	case T_openparen:
	case T_m:
		//don't eat, traverse
		Term();
		Exp_prime();
		break;
		
	default:
			syntax_error(NT_Exp);
		break;
	}
	parsetree.pop();
}

void parser_t::Exp_prime()
{
	parsetree.push(NT_Exp_prime);

	switch( scanner.next_token() ) 
	{
	case T_plus:
		eat_token(T_plus);
		Term();
		Exp_prime();
		break;
	case T_minus:
		eat_token(T_minus);
		Term();
		Exp_prime();
		break;
	case T_eof:
	case T_num:
	case T_openparen:
	case T_closeparen:
	case T_closesquare:
	case T_m:
	case T_goto:
	case T_print:
	case T_label:
		parsetree.drawepsilon();
		break;
	default:
		syntax_error(NT_Exp_prime);
		break;
	}
	parsetree.pop();
}

void parser_t::Term()
{
	parsetree.push(NT_Term);

	switch( scanner.next_token() ) 
	{
	case T_num:
	case T_openparen:
	case T_m:
		// don't eat num, iterate down
		Factor();
		Term_prime();
		break;
	default:
		syntax_error(NT_Term);
		break;
	}
	parsetree.pop();
}

void parser_t::Term_prime()
{
	parsetree.push(NT_Term_prime);

	switch( scanner.next_token() ) 
	{
	case T_times:
		eat_token(T_times);
		Factor();
		Term_prime();
		break;
	case T_divide:
		eat_token(T_divide);
		Factor();
		Term_prime();
		break;
	case T_num:
	case T_plus:
	case T_minus:
	case T_openparen:
	case T_closeparen:
	case T_closesquare:
	case T_m:
	case T_goto:
	case T_print:
	case T_label:
	case T_eof:
		parsetree.drawepsilon();
		break;
	default:
		syntax_error(NT_Term_prime);
		break;
	}
	parsetree.pop();
}

void parser_t::Factor()
{
	parsetree.push(NT_Factor);

	switch( scanner.next_token() ) 
	{
	case T_num:
	case T_openparen:
	case T_m:
		B();
		Factor_prime();
		break;
	default:
		syntax_error(NT_Factor);
		break;
	}
	parsetree.pop();
}

void parser_t::Factor_prime()
{
	parsetree.push(NT_Factor_prime);

	switch( scanner.next_token() ) 
	{
	case T_power:
		eat_token(T_power);
		B();
		Factor_prime();
		break;
	case T_openparen:
	case T_closeparen:
	case T_closesquare:
	case T_times:
	case T_divide:
	case T_plus:
	case T_minus:
	case T_m:
	case T_goto:
	case T_print:
	case T_label:
	case T_num:
	case T_eof:
		parsetree.drawepsilon();
		break;
	default:
		syntax_error(NT_Factor_prime);
		break;
	}
	parsetree.pop();
}

void parser_t::B()
{
	parsetree.push(NT_B);

	switch( scanner.next_token() ) 
	{
	case T_openparen:
		eat_token(T_openparen);
		Exp();
		eat_token(T_closeparen);
		break;
	case T_m:
		eat_token(T_m);
		eat_token(T_opensquare);
		Exp();
		eat_token(T_closesquare);
		break;
	case T_num:
		eat_token(T_num);
		break;
	default:
		syntax_error(NT_B);
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
	/*scanner_t scanner; 
			token_type tok = scanner.next_token(); 
			while(tok != T_eof){ 
				scanner.eat_token(tok); 
				printf("%s", token_to_string(tok)); 
				tok = scanner.next_token(); 
			} 
			printf("%s\n", token_to_string(tok));
			printf("\n");*/
		
}
