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
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Assignment,
	NT_Print,
	NT_Expression,
	NT_Factor,
	NT_Term,
	NT_Term2,
	NT_Term3,
	NT_Expression2,
	NT_Exponent

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
		case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Expression2: strncpy(buffer,"Expression2",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Term2: strncpy(buffer,"Term2",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Term3: strncpy(buffer,"Term3",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		case NT_Exponent: strncpy(buffer,"Exponent",MAX_SYMBOL_NAME_SIZE); break;


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
	char array[5]={'p','r','i','n','t'};
	char array2[4]={'g','o','t','o'};
	char array3[2]={'i','f'};
	char array4[2]={'*','*'};
	
	int l=0;
	//constructor 
	scanner_t();

  private:

	bool peekTokenExist= false;
	token_type peekToken;
	int lineNumber=1;

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);


};

token_type scanner_t::next_token()
{
	if ( peekTokenExist ) return peekToken;
	int asciiValue=cin.get();
	switch(asciiValue) 
	{
		case '\n':lineNumber++;return next_token();break;
		case ' ' : return next_token(); break;
    	case '\t': return next_token(); break;
		case EOF: peekToken=T_eof;break;
		case '+': peekToken=T_plus;break;
		case '-': peekToken=T_minus;break;
		case '*': 
			if(cin.get()=='*')//checking for exponentiation
			{
				peekToken=T_power;
				break;			
			}
			else
			{
				cin.unget();//put token back 
				peekToken=T_times;
							break;

			}

		case '/': peekToken=T_divide;break;
		case '=': peekToken=T_equals;break;
		case '[': peekToken=T_opensquare;break;
		case ']': peekToken=T_closesquare;break;
		case '(': peekToken=T_openparen;break;
		case ')': peekToken=T_closeparen;break;
		case 'm': peekToken=T_m;break;
		case 'if': peekToken=T_if;break;
		case 'print': peekToken=T_print;break;
		case 'goto': peekToken=T_goto;break;
		case 'L': peekToken=T_label;break;
		case ':': peekToken=T_colon;break;

		default:
			long number = asciiValue - '0';//get rid of ascii value
      		long expand = 10;
       		if(number >= 0 && number <= 9) //if is a number
       		{
         		peekToken = T_num;//make peekToken num
         		while(true)
         		{
          			int asciiValue = cin.get();//get next digit
           			if(!(asciiValue >= '0' && asciiValue <= '9')) //if is not a number( bracket or paren colon)
           			{
           			break;
           			}
           			number = number +expand * (asciiValue - '0');//
           			expand =expand * 10;
           			if(number > 2147483647)//number too big
           			{
           				scan_error(asciiValue);
           			} 
         		}
        		cin.unget(); // put back unused character
       		} 	
       					
       		else if(asciiValue >= 'a' && asciiValue <= 'z')//checking for commands (print if goto)
			{
				int i=0;
				int j=0;
				int k=0;
				if(asciiValue!='p'&&asciiValue!='r'&&asciiValue!='i'&&asciiValue!='n'&&asciiValue!='t'&&asciiValue!='g'&&asciiValue!='o'&&asciiValue!='f')
					{
						scan_error(asciiValue);

					}
							
					while(asciiValue==array[i]&&i<=4)//print command
					{
						asciiValue=cin.get();
						i++;
						if(i==4)
						{
						peekToken=T_print;
						}
					}
					while(asciiValue==array2[j]&&j<=3)//goto command
					{
						asciiValue=cin.get();
						j++;
						if(j==3)
						{
						peekToken=T_goto;
						}
					}
					while(asciiValue==array3[k]&&k<=1)//if command
					{
						asciiValue=cin.get();
						k++;
						if(k==1)
						{
						peekToken=T_if;
						}		
					}
					

				}
			else
			{
         		scan_error(asciiValue);
         	}	
			  break;
	}//end of switch block
	peekTokenExist=true;
	return peekToken;
	}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	if (c!=next_token())
	{
		mismatch_error(c);
	}
	peekTokenExist = false;
	
}

scanner_t::scanner_t()
{
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
    void Statement();

    void Expression();
    void Expression2();

    void Label();
    void Jump();
    void Print();
    void Assignment();
    void Exponent();

    void Term();    
    void Term2();
    void Term3();

    void Factor();
  public:	
	void parse();
	void StartCodeGen();
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
	StartCodeGen();
}


//WRITEME: the Start() function is not quite right.  Right now
//it is made to parse the grammar:
//       Start -> '+' Start | EOF
//which is not a very interesting language.  It has been included
//so you can see the basics of how to structure your recursive 
//decent code.

//Here is an example
void parser_t::StartCodeGen()
{
	string CodeGenHeader;
	string CodeGenRest;//attempt at printing out C code

	CodeGenHeader = "#include <stdio.h> #include <stdlib.h> #include <assert.h> include <string.h> #include <ctype.h> #include <stack> #include <iostream> using namespace std;" ;
	CodeGenRest= "int main{}";
		Start();

	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	CodeGenRest=CodeGenRest+"return 0;}";
	fprintf(stderr,"%s\n%s",CodeGenHeader.c_str(),CodeGenRest.c_str());
}
void parser_t::Start()
{
	parsetree.push(NT_Start);

	switch( scanner.next_token() ) 
	{
		case T_m:
		case T_goto:
		case T_print:
		case T_label:
			Statement();
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

void parser_t::Statement()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Statement);

	switch( scanner.next_token() ) 
	{
		case T_m:
			Assignment();
			break;
		case T_print:
			Print();
			break;
		case T_label:
			Label();
			break;
		case T_goto:
			Jump();
			break;
		default:
			syntax_error(NT_Statement);
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

	switch( scanner.next_token() ) 
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

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

	void parser_t::Print()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
			parsetree.push(NT_Print);
			eat_token(T_print);
			Expression();

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

	void parser_t::Jump()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Jump);

	switch( scanner.next_token() ) 
	{
		case T_goto:
			eat_token(T_goto);
			eat_token(T_num);
			if (scanner.next_token()==T_if)
			{
			eat_token(T_if);
			Expression();
			}
			break;
		default:
			syntax_error(NT_Jump);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}
void parser_t::Assignment()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Assignment)
	;

	switch( scanner.next_token() ) 
	{
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			Expression();
			eat_token(T_closesquare);
			eat_token(T_equals);
			Expression();
			break;
		default:
			syntax_error(NT_Assignment);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}
void parser_t::Expression()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Expression)
	;

	switch( scanner.next_token() ) 
	{
		case T_m:
		case T_goto:
		case T_print:
		case T_label:
		case T_num:
		case T_openparen:
		case T_opensquare:
			Term();
			Expression2();
			break;
		default:
			syntax_error(NT_Expression);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}
void parser_t::Expression2()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Expression2)
	;

	switch( scanner.next_token() ) 
	{
		case T_plus:
			eat_token(T_plus);
			Expression();
			break;
		case T_minus:
			eat_token(T_minus);
			Expression();
			break;
		case T_closeparen:
		case T_equals:
		case T_closesquare:
		case T_goto:
		case T_label:
		case T_print:
		case T_m:
		case T_num:
		case T_eof:
			break;
		default:
			syntax_error(NT_Expression2);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

//WRITEME: you will need to put the rest of the procedures here
void parser_t::Term()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Term)
	;

	switch( scanner.next_token() ) 
	{
		case T_m:
		case T_goto:
		case T_print:
		case T_label:
		case T_num:
		case T_openparen:
		case T_opensquare:
		Exponent();			
		Term2();
			break;
		default:
			syntax_error(NT_Term);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

void parser_t::Term2()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Term2)
	;

	switch( scanner.next_token() ) 
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
		case T_minus:
		case T_closeparen:
		case T_equals:
		case T_closesquare:
		case T_goto:
		case T_num:
		case T_label:
		case T_print:
		case T_m:
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Term2);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

void parser_t::Term3()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Term3)
	;

	switch( scanner.next_token() ) 
	{
		case T_power:
			eat_token(T_power);
			Term();
			break;
		case T_divide:
		case T_equals:
		case T_plus:
		case T_minus:
		case T_times:
		case T_closeparen:
		case T_closesquare:
		case T_goto:
		case T_label:
		case T_print:
		case T_m:
		case T_eof:
		parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Term3);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}


void parser_t::Exponent()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Exponent);
	Factor();
	Term3();
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
	parsetree.push(NT_Factor)
	;

	switch( scanner.next_token() ) 
	{
		case T_openparen:
			eat_token(T_openparen);
			Expression();
			eat_token(T_closeparen);
			break;
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			Expression();
			eat_token(T_closesquare);
			break;
		case T_goto:
			eat_token(T_goto);
			break;
		case T_label:
			eat_token(T_label);
			break;
		case T_num:
			eat_token(T_num);
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
{
 //  scanner_t scanner;
 //  token_type tok = scanner.next_token();
	// while(tok != T_eof){
	// 	scanner.eat_token(tok);
 //        printf("%s", token_to_string(tok));
 //        tok = scanner.next_token();
 //      }
 //      printf("%s\n", token_to_string(tok));

	parser_t parser;
	parser.parse();
	return 0;
}
