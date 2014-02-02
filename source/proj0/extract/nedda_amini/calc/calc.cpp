#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>

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
	T_colon,
	T_space,				// 17: :
	T_line
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
		case T_space:   strncpy(buffer," ",MAX_SYMBOL_NAME_SIZE); break;
		case T_line:   strncpy(buffer,"\n",MAX_SYMBOL_NAME_SIZE); break;
		default: 
			strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
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
	NT_Label,
	NT_Jump,
	NT_Assignment,
	NT_Print,
	NT_Statement,
	NT_Stmts,
	NT_Factor,
	NT_Args,
	NT_Term,
	NT_X
	//WRITEME: add symbolic names for you non-terminals here
	//DONE
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
		  case NT_Label: strncpy(buffer, "Label", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer, "Jump", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer, "Assignment", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer, "Print", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statement: strncpy(buffer, "Statement", MAX_SYMBOL_NAME_SIZE); break;

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

	token_type char_to_token(char x);	

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
	
	char peak;
	peak = getchar();
//	printf("first = %c",i);
	token_type peak_token;
	peak_token = char_to_token(peak);
	ungetc(peak, stdin);
	return peak_token;	
//printf("second = %c",j);


//	if ( bogo_token!=T_plus && bogo_token!=T_eof ) return T_plus;
//	else return bogo_token;
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
	int i = 0;
	char getC;
	getC = getchar();
	token_type token_new;
	token_new = char_to_token(getC);
	if (token_new != c)
		mismatch_error(c);
	if(token_new == T_print)
	{
		
		while(i<4)
		{
			getC = getchar();

			i++;
		}
	}
	else if(token_new == T_goto)
	{
		while(i<3)
		{
			getchar();
			i++;
		}
	}
	else if(token_new == T_if)
		getchar();

	
		
	//WRITEME: cut this bogus stuff out and implement eat_token

}

scanner_t::scanner_t()
{
/*        while (next_token() != T_eof){
		printf(token_to_string(next_token()));
	//getchar();
		eat_token(next_token());
	}
*/	//WRITiiEME: this is the scanner constructor
}

int scanner_t::get_line()
{
	int count;
	if(next_token() == T_line)
		 count++;

	return count;
	//WRITEME: this function should return the current line number (used
	//for error reporting
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


token_type scanner_t::char_to_token(char x)
{
	char check_1;
        char check_2;
        char check_3;
	char check_4;

	if (x == ' ')
		return T_space;
	else if(x == '=')
		return T_equals;
	else if (x == '+')
		return T_plus;
	else if (x == '-')
		return T_minus;
	else if (x == '*'){
		check_1 = getchar();
		if(check_1 == '*')
			{
				ungetc(check_1, stdin);
				return T_power;
			}
		else
			return T_times;
	}
	else if (x == '/')
		return T_divide;
	else if (x == '(')
		return T_openparen;
	else if (x == ')')
		return T_closeparen;
	else if (x == '[')
		return T_opensquare;
	else if (x == ']')
		return T_closesquare;
	else if (x == ':')		
		return T_colon;
	else if (x == '\n')
		return T_line;		
	else if (x == EOF)
		return T_eof;
	else if (x == 'm')
		return T_m;
	else if (x == 'L')
		return T_label;
	else if (x == 'i'){
			check_1 = getchar();
			if (check_1 == 'f')
			ungetc(check_1, stdin);
		
		return T_if;
		}
	
	else if (x == 'g')
		{
			check_1 = getchar();
			if (check_1 == 'o')
			{
				check_2 = getchar();
				if (check_2 == 't')
				{
					check_3 = getchar();
					if (check_3 == 'o')
					{
					}
					ungetc(check_3, stdin);
				}
				ungetc(check_2, stdin);
			}
			ungetc(check_1, stdin);        
			return T_goto;
	}
	else if (x == 'p')
                {
                        check_1 = getchar();
                        if (check_1 == 'r')
                        {
	                        check_2 = getchar();
                                if (check_2 == 'i')
                                {
		                        check_3 = getchar();
                                        if (check_3 == 'n')
                                        {
						check_4 = getchar();
			
						if(check_4 == 't')
							{	
					

							ungetc(check_4, stdin);
							}
							
						
                                        }
				        
                                        ungetc(check_3, stdin);
				}
                                ungetc(check_2, stdin);
                                
                        }
                        ungetc(check_1, stdin);
			
			return T_print;
               	}

	else if (x=='1'|x=='2'|x=='3'|x=='4'|x=='5'|x=='6'|x=='7'|x=='8'|x=='9'|x=='0')
		return T_num; 


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
	void Stmts();
	void Stmt();
	void X();
	void Label();
	void Jump();	
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

/*
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

*/

	Stmts();
	if(scanner.next_token()==T_eof)
	{
		parsetree.drawepsilon();
	
	}	

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}


void parser_t::Stmts()
{
	parsetree.push(NT_Stmts);
	Stmt();
	X();
	parsetree.pop();
}


void parser_t::X()
{
	parsetree.push(NT_X);
	switch(scanner.next_token())
	{
		case T_label:
			Stmt();
			X();
			break;
		case T_goto:
			Stmt();
			X();
			break;
		case T_m:
			Stmt();
			X();
			break;
		case T_print:
			Stmt();
			X();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}


void parser_t::Stmt()
{
	parsetree.push(NT_Statement);
	switch(scanner.next_token())
        {
                case T_label:
                        Label();
			break;
                case T_goto:
                    	Jump();
                        break;
                case T_m:
                        //Assign();
                        break;
                case T_print:
                        //Print();
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
	switch(scanner.next_token())
	{
		case T_label:
			eat_token(T_label);
			eat_token(T_num);
			eat_token(T_colon);
			break;
		default:
			syntax_error(NT_Label);
			break;
	}
	parsetree.pop();
}

void parser_t::Jump()
{
	parsetree.push(NT_Jump);
	eat_token(T_goto);
	eat_token(T_num);
	switch(scanner.next_token())
	{
	
	}	
	parsetree.pop();
}


//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

int main()
{
	//parser_t parser;
	//parser.parse();
	scanner_t scanner;
	//printf(token_to_string(scanner.next_token()));
	//scanner.scanner_t();
	return 0;
}
