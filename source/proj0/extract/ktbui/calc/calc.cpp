#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <vector>
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
       
	//WRITEME: add symbolic names for you non-terminals here
	NT_Statements,
	NT_Statement1,
        NT_Statement2,
        NT_Label,
        NT_Jump,
        NT_JumpEmpty,
	NT_Assignment,
        NT_Print,
        NT_Expression,
        NT_addSubtract,
        NT_multiplyDivide,
	NT_power,
        NT_paren,
        NT_lastTwo,

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

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
        int lineNumber;

        int stringIndex;
  
        int tokenIndex;   

        vector<string> stringToken ;
  
        vector<token_type> tokens ;

  

         

};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in
  return tokens.at(tokenIndex);
 
}  

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eat_token
  if (tokens.at(tokenIndex) != c )
    {
         mismatch_error(c);
    }   

  else { 
         tokenIndex++;
         stringIndex++;
       }


}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
  
  lineNumber = 1;
  stringIndex = 0;
  tokenIndex = 0;
 
  int end;
  int value;
  string lineLength;
  string linePosition;
  string num; 
  string temp;

  while(getline(cin,lineLength)){
    for(int i = 0; i<lineLength.length(); i++){
      switch(lineLength[i]){
      case '+':
	tokens.push_back(T_plus);
	stringToken.push_back(temp += lineLength[i]);
	temp.clear();
	break;
      case '-':
	tokens.push_back(T_minus);
	stringToken.push_back(temp += lineLength[i]);
        temp.clear();
	break;
      case '/':
	tokens.push_back(T_divide);
	stringToken.push_back(temp += lineLength[i]);
     	temp.clear();
	break;
      case '(':
	tokens.push_back(T_openparen);
	stringToken.push_back(temp += lineLength[i]);
     	temp.clear();
	break;
      case ')':
	tokens.push_back(T_closeparen);
	stringToken.push_back(temp += lineLength[i]);
	temp.clear();
	break;
      case 'm':
	tokens.push_back(T_m);
	stringToken.push_back(temp += lineLength[i]);
	temp.clear();
	break;
      case '[':
	tokens.push_back(T_opensquare);
	stringToken.push_back(temp += lineLength[i]);
	temp.clear();
	break;
      case ']':
	tokens.push_back(T_closesquare);
	stringToken.push_back(temp += lineLength[i]);
	temp.clear();
	break;
      case 'L':
	tokens.push_back(T_label);
	stringToken.push_back(temp += lineLength[i]);
	temp.clear();
	break;
      case ':':
	tokens.push_back(T_colon);
	stringToken.push_back(temp += lineLength[i]);
	temp.clear();
	break;
      case '=':
	tokens.push_back(T_equals);
      	stringToken.push_back(temp += lineLength[i]);
        temp.clear();
	break;
      case '*':
	if(lineLength[i+1] == '*'){
	  tokens.push_back(T_power);
	  i++;
	  stringToken.push_back("**");
	} else {
	  tokens.push_back(T_times);
	  stringToken.push_back("*");
	}
	break;
      case 'g':
	i++;
	if(lineLength[i] == 'o'){
	  i++;
	  if(lineLength[i] == 't'){
	    i++;
	    if(lineLength[i] == 'o'){
	      tokens.push_back(T_goto);
	      stringToken.push_back("goto");
	    } else {
	      scan_error(lineLength[i]);
	    }
	  } else {
	    scan_error(lineLength[i]);
	  }
	} else {
	  scan_error(lineLength[i]);
	}
	break;
      case 'p':
	i++;
	if(lineLength[i] == 'r'){
	  i++;
	  if(lineLength[i] == 'i'){
	    i++;
	    if(lineLength[i] == 'n'){
	      i++;
	      if(lineLength[i] == 't'){
		tokens.push_back(T_print);
		stringToken.push_back("print");
	      } else {
		scan_error(lineLength[i]);
	      }
	    } else {
	      scan_error(lineLength[i]);
	    }
	  } else {
	    scan_error(lineLength[i]);
	  }
	} else {
	  scan_error(lineLength[i]);
	}
	break;
      case 'i':
	i++;
	if(lineLength[i] == 'f'){
	  tokens.push_back(T_if);
	  stringToken.push_back("if");
	} else {
	  scan_error(lineLength[i]);
	}
	break;
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
	end = i+1;
	while                                     (lineLength[end] == '0' ||
                                                  lineLength[end] == '1' ||
                                                  lineLength[end] == '2' ||
                                                  lineLength[end] == '3' ||
                                                  lineLength[end] == '4' ||
                                                  lineLength[end] == '5' ||
                                                  lineLength[end] == '6' ||
                                                  lineLength[end] == '7' ||
                                                  lineLength[end] == '8' ||
	                                          lineLength[end] == '9' ){
	  end++;
	}
	num = lineLength.substr(i,end-i);
	value = atof(num.c_str());
	if(value > 2147483647 && value < 0){
	  scan_error(lineLength[i]);
	} else {
	  tokens.push_back(T_num);
	  stringToken.push_back(num);
	  i = end-1;
	}
	break;
      case ' ':
	break;
      default:
	scan_error(lineLength[i]);
	break;
      }
    }
    lineNumber++;
  }

  tokens.push_back(T_eof);
  stringToken.push_back("EOF");



}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
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
	//WRITEME: fill this out with the rest of the 
  	//recursive decent stuff (more methods)
        void Statements();
        void Statement1();
        void Statement2();
        void Label();
        void Jump();
        void JumpEmpty();
        void Assignment();
        void Print();
        void Expression();
        void addSubtract();
        void multiplyDivide();
        void power();
        void paren();
        void lastTwo();

  


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
        Statements();
     

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here
void parser_t::Statements()
{
  parsetree.push(NT_Statements);
  Statement1();
  Statement2();
  parsetree.pop();
}


void parser_t::Statement1()
{
  parsetree.push(NT_Statement2);
  switch(scanner.next_token())
    {
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
    default:
      syntax_error(NT_Statements);
      break;
    }
  parsetree.pop();
}


void parser_t::Statement2()                                                                                      \

{
  parsetree.push(NT_Statement2);
  switch(scanner.next_token())
    {
    case T_label: 
    T_goto: 
    T_m :
    T_print:
      Statement1();
      Statement2();
      break;
    case T_eof:
      parsetree.drawepsilon();
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
  JumpEmpty();
  parsetree.pop();
}

void parser_t::JumpEmpty()                                                                                                      
{
  parsetree.push(NT_JumpEmpty);
  switch(scanner.next_token())
    {
    case T_if:
      eat_token(T_if);
      Expression();
      break;
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_JumpEmpty);
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
void parser_t::Expression()
{
  parsetree.push(NT_Expression);
  
  parsetree.pop();
}


////// last 8 cases for expression

void parser_t::addSubtract()                                                                                            
{
  parsetree.push(NT_addSubtract);
  switch(scanner.next_token())
    {
    case T_plus:
      eat_token(T_plus);
      Expression();
      break;
    case T_minus:
      eat_token(T_minus);
      Expression();
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

void parser_t::multiplyDivide()                                                                                                      
{
  parsetree.push(NT_multiplyDivide);
  switch(scanner.next_token())
    {
    case T_times:
      eat_token(T_times);
      break;
    case T_divide:
      eat_token(T_divide);
      break;
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_multiplyDivide);
      break;
    }
  parsetree.pop();
}



void parser_t::power()                                                                                                     
{
  parsetree.push(NT_power);
  switch(scanner.next_token())
    {
    case T_power:
      eat_token(T_power);
     
      break;
   
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_power);
      break;
    }
  parsetree.pop();
}


void parser_t::paren()
{
  parsetree.push(NT_paren);
  switch(scanner.next_token())
    {
    case T_openparen:
      eat_token(T_openparen);
      Expression();
      eat_token(T_closeparen);
      break;
    default:
      syntax_error(NT_paren);
      break;
    }
  parsetree.pop();
}


void parser_t::lastTwo()
{
  parsetree.push(NT_lastTwo);
  switch(scanner.next_token())
    {
    case T_m:
      eat_token(T_m);
      eat_token(T_opensquare);
      Expression();
      eat_token(T_closesquare);
      break;
    case T_num:
      eat_token(T_num);
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



