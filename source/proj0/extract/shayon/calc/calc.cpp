#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <vector>
#include <sstream>
#include <cmath>
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
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_JumpPrime,
	NT_Assignment,
	NT_Print,
	NT_ExpressionPrime,
	NT_Term,
	NT_TermPrime,
	NT_Power,
	NT_PowerPrime,
	NT_Factor
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
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_JumpPrime: strncpy(buffer,"JumpPrime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_ExpressionPrime: strncpy(buffer,"ExpressionPrime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_TermPrime: strncpy(buffer,"TermPrime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Power: strncpy(buffer,"Power",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_PowerPrime: strncpy(buffer,"PowerPrime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
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
	//list of numbers scanned
	std::vector<int> numberList;
	
  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  
  	int lineNumber;
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
	bool isNotDone = true;
	token_type token;
	char c,o,t,oPrime,f,r,i,n,lt,w;
	std::vector<char> word;
	int lineAdd=0;
	int place=0;
	do
	{	
		c=getc(stdin);
		switch(c)
		{
			case '+':	token=T_plus;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case '-':	token=T_minus;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case '*':	w=getc(stdin);
						if(w=='*')
							token=T_power;
						else
							token=T_times;
						isNotDone=false;
						ungetc(w,stdin);
						ungetc(c,stdin);
						break;
			case '/':	token=T_divide;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case '=':	token=T_equals;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case '(':	token=T_openparen;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case ')':	token=T_closeparen;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case '[':	token=T_opensquare;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case ']':	token=T_closesquare;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case 'm':	token=T_m;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case ':':	token=T_colon;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case 'L':	token=T_label;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case ' ':	isNotDone=true;
						word.push_back(c);
						place++;
						break;
			case '\n':	isNotDone=true;
						word.push_back(c);
						place++;
						lineAdd++;
						lineNumber++;
						break;
			case EOF:	token=T_eof;
						isNotDone=false;
						ungetc(c,stdin);
						break;
			case 'g':	o=getc(stdin);
						if(o=='o')
						{
							t=getc(stdin);
							if(t=='t')
							{
								oPrime=getc(stdin);
								if(oPrime=='o')
								{
									token=T_goto;
								}
								else
									scan_error(oPrime);
								ungetc(oPrime,stdin);
							}
							else
								scan_error(t);
							ungetc(t,stdin);
						}
						else
							scan_error(o);
						ungetc(o,stdin);
						ungetc(c,stdin);
						isNotDone=false;
						break;
			case 'i':	f=getc(stdin);
						if(f=='f')
						{
							token=T_if;
							ungetc(f,stdin);
							ungetc(c,stdin);
						}
						else
							scan_error(f);
						isNotDone=false;
						break;
			case 'p':	r = getc(stdin);
						if(r=='r')
						{
							i = getc(stdin);
							if(i=='i')
							{
								n = getc(stdin);
								if(n=='n')
								{
									lt = getc(stdin);
									if(lt=='t')
									{
										token = T_print;	
									}
									else
										scan_error(lt);
									ungetc(lt,stdin);
								}
								else
									scan_error(n);
								ungetc(n,stdin);
							}
							else
								scan_error(i);
							ungetc(i,stdin);
						}
						else
							scan_error(r);
						ungetc(r,stdin);
						ungetc(c,stdin);
						isNotDone=false;
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
			case '9':	token=T_num;
						ungetc(c,stdin);
						isNotDone=false;
						break;
			default:	scan_error(c);
						ungetc(c,stdin);
						isNotDone=false;
		}
	}while(isNotDone);
	lineNumber=lineNumber-lineAdd;
	for(int y=place-1;y >= 0;y--)
	{
		ungetc(word[y],stdin);
	}
	return token;
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
	//if ( bogo_token!=T_plus && bogo_token!=T_eof ) return T_plus;
	//else return bogo_token;
}

void scanner_t::eat_token(token_type expected)
{

	bool isNotDone = true;
	token_type token;
	char c,o,t,oPrime,f,r,i,n,lt,w;
	int number;
	stringstream converter;
	do
	{	
		c=getc(stdin);
		switch(c)
		{
			case '+':	token=T_plus;
						isNotDone=false;
						break;
			case '-':	token=T_minus;
						isNotDone=false;
						break;
			case '*':	w=getc(stdin);
						if(w=='*')
							token=T_power;
						else
						{
							token=T_times;
							ungetc(w, stdin);
						}
						isNotDone=false;
						break;
			case '/':	token=T_divide;
						isNotDone=false;
						break;
			case '=':	token=T_equals;
						isNotDone=false;
						break;
			case '(':	token=T_openparen;
						isNotDone=false;
						break;
			case ')':	token=T_closeparen;
						isNotDone=false;
						break;
			case '[':	token=T_opensquare;
						isNotDone=false;
						break;
			case ']':	token=T_closesquare;
						isNotDone=false;
						break;
			case 'm':	token=T_m;
						isNotDone=false;
						break;
			case ':':	token=T_colon;
						isNotDone=false;
						break;
			case 'L':	token=T_label;
						isNotDone=false;
						break;
			case ' ':	isNotDone=true;
						break;
			case '\n':	isNotDone=true;
						lineNumber++;
						break;
			case EOF:	token=T_eof;
						isNotDone=false;
						break;
			case 'g':	o=getc(stdin);
						if(o=='o')
						{
							t=getc(stdin);
							if(t=='t')
							{
								oPrime=getc(stdin);
								if(oPrime=='o')
								{
									token=T_goto;
								}
								else
									mismatch_error(T_goto);
							}
							else
								mismatch_error(T_goto);
						}
						else
							mismatch_error(T_goto);
						isNotDone=false;
						break;
			case 'i':	f=getc(stdin);
						if(f=='f')
						{
							token=T_if;
						}
						else
							mismatch_error(T_if);
						isNotDone=false;
						break;
			case 'p':	r = getc(stdin);
						if(r=='r')
						{
							i = getc(stdin);
							if(i=='i')
							{
								n = getc(stdin);
								if(n=='n')
								{
									lt = getc(stdin);
									if(lt=='t')
									{
										token = T_print;
									}
									else
										mismatch_error(T_print);
								}
								else
										mismatch_error(T_print);
							}
							else
										mismatch_error(T_print);
						}
						else
										mismatch_error(T_print);
						isNotDone=false;
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
			case '9':	token=T_num;
						isNotDone=false;
						while(isdigit(c))
						{
							converter << c; 
							c=getc(stdin);
						}
						ungetc(c,stdin);
						converter >> number;
						numberList.push_back(number);
						//printf("%d ", number);
						/*if(number < 0 || number > pow(2,31)-1)
						{
							printf("scan error: Range Error, Number not within range -line %d\n",x, get_line());
							exit(1);
						}*/
						break;
			default:	scan_error(c);
						isNotDone=false;
		}
	}while(isNotDone);
	if(token!=expected)
		mismatch_error(expected);

	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eat_token
	//if ( rand()%10 < 8 ) bogo_token = T_plus;
	//else bogo_token = T_eof;

}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	lineNumber=1;
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
	int powBuffer;
	void Start();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)
	void Statements();
	void Statement();
	void Label();
	void Jump();
	void JumpP(int);
	void Assign();
	void Print();
	void Expression();
	void ExpressionP();
	void Term();
	void TermP();
	void Power();
	void PowerP();
	void Factor();
	//place you are in numberList
	int numberListSpot;

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
	numberListSpot=0;
	powBuffer=-1;
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
		case T_print:
			Statements();
			break;
		case T_m:
			Statements();
			break;
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
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Statements);

	switch( scanner.next_token() ) 
	{
		case T_label:
			Statement();
			Statements();
			break;
		case T_print:
			Statement();
			Statements();
			break;
		case T_m:
			Statement();
			Statements();
			break;
		case T_goto:
			Statement();
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

void parser_t::Statement()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Statement);

	switch( scanner.next_token() ) 
	{
		case T_label:
			Label();
			break;
		case T_goto:
			Jump();
			break;
		case T_m:
			Assign();
			break;
		case T_print:
			Print();
			break;
		default:
			syntax_error(NT_Start);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Label()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Label);

	if(scanner.next_token() == T_label)
	{
		eat_token(T_label);
		cerr << "label";
		if(scanner.next_token() == T_num)
		{
			eat_token(T_num);
			cerr << scanner.numberList[numberListSpot];
			numberListSpot++;
			if(scanner.next_token()== T_colon)
			{
				eat_token(T_colon);
				cerr << ": ";
			}
			else
				syntax_error(NT_Label);
		}
		else
			syntax_error(NT_Label);

	}
	else
		syntax_error(NT_Label);

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	cerr << "\n    ";
	parsetree.pop();
}

void parser_t::Jump()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Jump);

	if(scanner.next_token() == T_goto)
	{
		eat_token(T_goto);
		if(scanner.next_token() == T_num)
		{
			eat_token(T_num);
			int labelNum = scanner.numberList[numberListSpot];
			numberListSpot++;
			JumpP(labelNum);
		}
		else
			syntax_error(NT_Jump);

	}
	else
		syntax_error(NT_Jump);

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::JumpP(int labelNum)
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_JumpPrime);

	switch( scanner.next_token() ) 
	{
		case T_label:
			parsetree.drawepsilon();
			cerr << "goto label";
			cerr << labelNum;
			cerr << "; ";
			cerr << "\n    ";
			break;
		case T_goto:
			parsetree.drawepsilon();
			cerr << "goto label";
			cerr << labelNum;
			cerr << "; ";
			cerr << "\n    ";
			break;
		case T_print:
			parsetree.drawepsilon();
			cerr << "goto label";
			cerr << labelNum;
			cerr << "; ";
			cerr << "\n    ";
			break;
		case T_if:
			eat_token(T_if);
			cerr << "if(";
			Expression();
			cerr << ")\n        ";
			cerr << "goto label";
			cerr << labelNum;
			cerr << "; ";
			break;
		case T_m:
			parsetree.drawepsilon();
			cerr << "goto label";
			cerr << labelNum;
			cerr << "; ";
			cerr << "\n    ";
			break;
		case T_eof:
			parsetree.drawepsilon();
			cerr << "goto label";
			cerr << labelNum;
			cerr << "; ";
			cerr << "\n    ";
			break;
		default:
			syntax_error(NT_JumpPrime);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree

	parsetree.pop();
}

void parser_t::Assign()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Assignment);

	if(scanner.next_token() == T_m)
	{
		eat_token(T_m);
		cerr << "m";
		if(scanner.next_token() == T_opensquare)
		{
			eat_token(T_opensquare);
			cerr << "[";
			Expression();
			if(scanner.next_token()== T_closesquare)
			{
				eat_token(T_closesquare);
				cerr << "]";
				if(scanner.next_token()== T_equals)
				{
					eat_token(T_equals);
					cerr << "=";
					Expression();
				}
				else
					syntax_error(NT_Assignment);
			}
			else
				syntax_error(NT_Assignment);
		}
		else
			syntax_error(NT_Assignment);

	}
	else
		syntax_error(NT_Assignment);

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	cerr << ";\n    ";
	parsetree.pop();
}


void parser_t::Print()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Print);

	if(scanner.next_token() == T_print)
	{
		eat_token(T_print);
		cerr << "printf(\"%d\",";
		Expression();
	}
	else
		syntax_error(NT_Print);

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	cerr << ");\n    ";
	parsetree.pop();
}

void parser_t::Expression()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Expression);

	switch( scanner.next_token() ) 
	{
		case T_num:
			Term();
			ExpressionP();
			break;
		case T_m:
			Term();
			ExpressionP();
			break;
		case T_openparen:
			Term();
			ExpressionP();
			break;
		default:
			syntax_error(NT_Expression);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::ExpressionP()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_ExpressionPrime);

	switch( scanner.next_token() ) 
	{
		case T_label:
			parsetree.drawepsilon();
			break;
		case T_goto:
			parsetree.drawepsilon();
			break;
		case T_print:
			parsetree.drawepsilon();
			break;
		case T_m:
			parsetree.drawepsilon();
			break;
		case T_closeparen:
			parsetree.drawepsilon();
			break;
		case T_closesquare:
			parsetree.drawepsilon();
			break;
		case T_plus:
			eat_token(T_plus);
			cerr << "+";
			Term();
			ExpressionP();
			break;
		case T_minus:
			eat_token(T_minus);
			cerr << "-";
			Term();
			ExpressionP();
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_ExpressionPrime);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}


void parser_t::Term()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Term);

	switch( scanner.next_token() ) 
	{
		case T_num:
			Power();
			TermP();
			break;
		case T_m:
			Power();
			TermP();
			break;
		case T_openparen:
			Power();
			TermP();
			break;
		default:
			syntax_error(NT_Term);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}


void parser_t::TermP()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_TermPrime);

	switch( scanner.next_token() ) 
	{
		case T_label:
			parsetree.drawepsilon();
			break;
		case T_m:
			parsetree.drawepsilon();
			break;
		case T_closeparen:
			parsetree.drawepsilon();
			break;
		case T_goto:
			parsetree.drawepsilon();
			break;
		case T_print:
			parsetree.drawepsilon();
			break;
		case T_closesquare:
			parsetree.drawepsilon();
			break;
		case T_plus:
			parsetree.drawepsilon();
			break;
		case T_minus:
			parsetree.drawepsilon();
			break;
		case T_times:
			eat_token(T_times);
			cerr << "*";
			Power();
			TermP();
			break;
		case T_divide:
			eat_token(T_divide);
			cerr << "/";
			Power();
			TermP();
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_TermPrime);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Power()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Power);

	switch( scanner.next_token() ) 
	{
		case T_num:
			Factor();
			PowerP();
			break;
		case T_m:
			Factor();
			PowerP();
			break;
		case T_openparen:
			Factor();
			PowerP();
			break;
		default:
			syntax_error(NT_Power);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::PowerP()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_PowerPrime);

	switch( scanner.next_token() ) 
	{
		case T_label:
			parsetree.drawepsilon();
			break;
		case T_m:
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
		case T_goto:
			parsetree.drawepsilon();
			break;
		case T_print:
			parsetree.drawepsilon();
			break;
		case T_closesquare:
			parsetree.drawepsilon();
			break;
		case T_plus:
			parsetree.drawepsilon();
			break;
		case T_minus:
			parsetree.drawepsilon();
			break;
		case T_power:
			eat_token(T_power);
			cerr << "pow(";
			if(powBuffer!=-1)
			{
				cerr << powBuffer;
				powBuffer=-1;
				cerr << " , ";
			}
			Factor();
			PowerP();
			cerr << " ) ";
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_PowerPrime);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Factor()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Factor);

	switch( scanner.next_token() ) 
	{
		case T_num:
			eat_token(T_num);
			if(scanner.next_token()==T_power)
			{
				powBuffer=scanner.numberList[numberListSpot];
			}
			else
				cerr << scanner.numberList[numberListSpot];
			numberListSpot++;
			break;
		case T_m:
			eat_token(T_m);
			cerr << "m";
			if (scanner.next_token() == T_opensquare)
			{
				eat_token(T_opensquare);
				cerr << "[";
				Expression();
				if(scanner.next_token() == T_closesquare)
				{
					eat_token(T_closesquare);
					cerr << "]";
				}
				else
					syntax_error(NT_Factor);
			}
			else
				syntax_error(NT_Factor);
			break;
		case T_openparen:
			eat_token(T_openparen);
			cerr << "(";
			Expression();
			if (scanner.next_token() == T_closeparen)
			{
				eat_token(T_closeparen);
				cerr << ")";
			}
			else
				syntax_error(NT_Factor);
			break;
		default:
			syntax_error(NT_Factor);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}


/*** Main ***********************************************/

int main()
{	/* //SCANNER TESTS
	scanner_t scanner;
	while(scanner.next_token()!=T_eof)
	{
		token_type t=scanner.next_token();
		//printf("%s . ", token_to_string(t));
		scanner.eat_token(t);
	}
	printf("\n");
	for (int i = scanner.numberListSpot-1; i >= 0; i--)
	{
		printf("%d ", scanner.numberList[i]);
	}
	*/
	///*
	parser_t parser;
	cerr << "#include <stdio.h> \n#include <math.h> \nint main() \n{\n    int m[101];\n    ";

	parser.parse();

	cerr << "return 0;\n}";
	//*/
	return 0;
}
