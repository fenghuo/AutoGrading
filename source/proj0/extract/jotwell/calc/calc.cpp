#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <string>
#include <math.h>

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
	NT_Statements_Prime,
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Jump_Prime,
	NT_Assignment,
	NT_Print,
	NT_Expression_Prime,
	NT_Mult,
	NT_Mult_Prime,
	NT_Exp,
	NT_Exp_Prime,
	NT_Paren,
	NT_Id
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
		  case NT_Expression_Prime: strncpy(buffer,"Expression Prime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statements_Prime: strncpy(buffer,"Statements Prime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump_Prime: strncpy(buffer,"Jump Prime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Mult: strncpy(buffer,"Multiplication",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Mult_Prime: strncpy(buffer,"Multiplication Prime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Exp: strncpy(buffer,"Exponent",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Exp_Prime: strncpy(buffer,"Exponent Prime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Paren: strncpy(buffer,"Parentheses",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Id: strncpy(buffer,"Identification",MAX_SYMBOL_NAME_SIZE); break;
		  default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
		}
	return buffer;
}

/*** Scanner Class ***********************************************/
class token
{
	public:
		token_type type;
		int num;
		int line;
		token* next;
		token(token_type t, int l)
		{
			type = t;
			num = 0;
			line = l;
			next = NULL;
		}
		token(token_type t, int n, int l)
		{
			type = t;
			num = n;
			line = l;
			next = NULL;
		}
};

class scanner_t {
  public:

	//eats the next token and prints an error if it is not of type c
	void eat_token(token_type c);

	//peeks at the lookahead token
	token_type next_token();

	//return line number for errors
	int get_line();

	//print the scanner for testing purposes
	void print();
	
	int get_number();

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
	//error message for int too large
	void int_error();

	token* front;
	int line;
};

token_type scanner_t::next_token()
{
	//will take a peek at the next token and return it to the parser.
	return front->type;
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
	if (c != front->type)
		mismatch_error(c);
	else
	{
		token* temp;
		temp = front;
		front = temp->next;
		token_type token;
		token = temp->type;
		delete temp;
	}
}

int scanner_t::get_number()
{
	return front->num;
}

void scanner_t::print()
{
	token* token;
	token = front;
	token_type t;
	while (token->next != NULL)
	{
		t = token->type;
		printf("%s (%d, %d) ",token_to_string(t), token->num, token->line);
		token = token->next;
	}
	t = token->type;
	printf("%s (%d, %d) ",token_to_string(t), token->num, token->line);
	printf("\n");
}

scanner_t::scanner_t()
{
	char c;
	line = 1;
	token* currentToken = NULL;
	while(c = getchar())
	{
		start:
		if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' 
			|| c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
		{
			string number = "";
			number += c;
			while(c = getchar())
			{	
				if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' 
					|| c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
				{
					number += c;
				}
				else
					break;
			}
			long num = strtol(number.c_str(), NULL, 10);
			if (num > (long)pow(2, 31) - 1)
			{
				int_error();
			}
			if (currentToken == NULL)
			{
				currentToken = new token(T_num, (int)num, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_num, (int)num, line);
				currentToken = currentToken->next;
			}
			goto start;
		}
		
		else if (c == 'L')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_label, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_label, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == ':')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_colon, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_colon, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == 'g')
		{
			c = getchar();
			if (c == 'o')
			{
				c = getchar();
				if (c == 't')
				{
					c = getchar();
					if (c == 'o')
					{
						if (currentToken == NULL)
						{
							currentToken = new token(T_goto, line);
							front = currentToken;
						}
						else
						{
							currentToken->next = new token(T_goto, line);
							currentToken = currentToken->next;
						}
						c = getchar();
						goto start;
					}
				}
			}
			scan_error(c);
		}
		
		else if (c == 'i')
		{
			c = getchar();
			if (c == 'f')
			{
				if (currentToken == NULL)
				{
					currentToken = new token(T_if, line);
					front = currentToken;
				}
				else
				{
					currentToken->next = new token(T_if, line);
					currentToken = currentToken->next;
				}
			}
			else
			{
				scan_error(c);
			}
		}
		
		else if (c == 'm')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_m, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_m, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == '[')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_opensquare, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_opensquare, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == ']')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_closesquare, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_closesquare, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == '=')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_equals, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_equals, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == 'p')
		{
			c = getchar();
			if (c == 'r')
			{
				c = getchar();
				if (c == 'i')
				{
					c = getchar();
					if (c == 'n')
					{
						c = getchar();
						if (c == 't')
						{
							if (currentToken == NULL)
							{
								currentToken = new token(T_print, line);
								front = currentToken;
							}
							else
							{
								currentToken->next = new token(T_print, line);
								currentToken = currentToken->next;
							}
							c = getchar();
							goto start;
						}
					}
				}
			}
			scan_error(c);
		}
		
		else if (c == '+')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_plus, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_plus, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == '-')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_minus, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_minus, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == '/')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_divide, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_divide, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == '(')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_openparen, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_openparen, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == ')')
		{
			if (currentToken == NULL)
			{
				currentToken = new token(T_closeparen, line);
				front = currentToken;
			}
			else
			{
				currentToken->next = new token(T_closeparen, line);
				currentToken = currentToken->next;
			}
		}
		
		else if (c == '*')
		{
			c = getchar();
			if (c == '*')
			{
				if (currentToken == NULL)
				{
					currentToken = new token(T_power, line);
					front = currentToken;
				}
				else
				{
					currentToken->next = new token(T_power, line);
					currentToken = currentToken->next;
				}
			}
			else
			{
				if (currentToken == NULL)
				{
					currentToken = new token(T_times, line);
					front = currentToken;
				}
				else
				{
					currentToken->next = new token(T_times, line);
					currentToken = currentToken->next;
				}
				goto start;
			}
		}
		
		else if (c == ' ')
		{
			continue;
		}
		
		else if (c == '\n')
		{
			line++;
			continue;
		}
		
		else if (c == '\t')
		{
			continue;
		}
		
		else if (c == EOF)
		{
			break;
		}
		
		else
		{
			scan_error(c);
		}
	}
	
	if (currentToken == NULL)
	{
		currentToken = new token(T_eof, line);
		front = currentToken;
	}
	else
	{
		currentToken->next = new token(T_eof, line);
		currentToken = currentToken->next;
	}
}

int scanner_t::get_line()
{
	return front->line;
}

void scanner_t::scan_error (char x)
{
	printf("scan error: unrecognized character '%c' -line %d\n",x, line);
	exit(1);

}

void scanner_t::int_error ()
{
	printf("scan error: integer too large -line %d\n", line);
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
	void StatementsP();
	void Statement();
	void Label();
	void Jump();
	void JumpP(int n);
	void Assignment();
	void Print();
	void Expression();
	void ExpressionP();
	void Mult();
	void MultP();
	void Exp();
	void ExpP();
	void Paren();
	void Id();

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



void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);

	fprintf(stderr, "#include <stdio.h>\n#include <math.h>\nint main()\n{\nint m[101];\n");
	Statements();
	fprintf(stderr, "return 0;\n}\n");
	
	switch( scanner.next_token() ) 
	{
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

void parser_t::Statements()
{
	parsetree.push(NT_Statements);

	Statement();
	StatementsP();
	

	parsetree.pop();
}

void parser_t::StatementsP()
{
	parsetree.push(NT_Statements_Prime);

	switch( scanner.next_token() ) 
	{
		case T_eof:
			parsetree.drawepsilon();
			break;
		case T_label:
			Statement();
			StatementsP();
			break;
		case T_goto:
			Statement();
			StatementsP();
			break;
		case T_m:
			Statement();
			StatementsP();
			break;
		case T_print:
			Statement();
			StatementsP();
			break;
		default:
			syntax_error(NT_Statements_Prime);
			break;
	}


	parsetree.pop();
}

void parser_t::Statement()
{
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

void parser_t::Label()
{
	parsetree.push(NT_Label);

	eat_token(T_label);
	int n = scanner.get_number();
	eat_token(T_num);
	eat_token(T_colon);
	
	fprintf(stderr, "L%d:\n", n);

	parsetree.pop();
}

void parser_t::Jump()
{
	parsetree.push(NT_Jump);

	eat_token(T_goto);
	int n = scanner.get_number();
	eat_token(T_num);
	
	
	
	JumpP(n);

	parsetree.pop();
}

void parser_t::JumpP(int n)
{
	parsetree.push(NT_Jump_Prime);

	switch( scanner.next_token() ) 
	{
		case T_if:
			eat_token(T_if);
			fprintf(stderr, "if (");
			Expression();
			fprintf(stderr, ")\n	goto L%d;\n", n);
			break;
		case T_label:
			fprintf(stderr, "goto L%d;\n", n);
			parsetree.drawepsilon();
			break;
		case T_goto:
			fprintf(stderr, "goto L%d;\n", n);
			parsetree.drawepsilon();
			break;
		case T_m:
			fprintf(stderr, "goto L%d;\n", n);
			parsetree.drawepsilon();
			break;
		case T_print:
			fprintf(stderr, "goto L%d;\n", n);
			parsetree.drawepsilon();
			break;
		case T_eof:
			fprintf(stderr, "goto L%d;\n", n);
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Statement);
			break;
	}


	parsetree.pop();
}

void parser_t::Assignment()
{
	parsetree.push(NT_Assignment);

	eat_token(T_m);
	eat_token(T_opensquare);
	
	fprintf(stderr, "m[");
	Expression();
	fprintf(stderr, "] = ");
	
	eat_token(T_closesquare);
	eat_token(T_equals);
	Expression();
	fprintf(stderr, ";\n");

	parsetree.pop();
}

void parser_t::Print()
{
	parsetree.push(NT_Print);

	fprintf(stderr, "printf(\"%%d\\n\", ");
	eat_token(T_print);
	Expression();
	fprintf(stderr, ");\n");

	parsetree.pop();
}

void parser_t::Expression()
{
	parsetree.push(NT_Expression);

	Mult();
	ExpressionP();

	parsetree.pop();
}

void parser_t::ExpressionP()
{
	parsetree.push(NT_Expression_Prime);

	switch( scanner.next_token() ) 
	{
		case T_plus:
			eat_token(T_plus);
			fprintf(stderr, " + ");
			Mult();
			ExpressionP();
			break;
		case T_minus:
			eat_token(T_minus);
			fprintf(stderr, " - ");
			Mult();
			ExpressionP();
			break;
		case T_goto:
			parsetree.drawepsilon();
			break;
		case T_label:
			parsetree.drawepsilon();
			break;
		case T_m:
			parsetree.drawepsilon();
			break;
		case T_print:
			parsetree.drawepsilon();
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
			syntax_error(NT_Expression_Prime);
			break;
	}


	parsetree.pop();
}

void parser_t::Mult()
{
	parsetree.push(NT_Mult);

	Exp();
	MultP();

	parsetree.pop();
}

void parser_t::MultP()
{
	parsetree.push(NT_Mult_Prime);

	switch( scanner.next_token() ) 
	{
		case T_times:
			eat_token(T_times);
			fprintf(stderr, " * ");
			Exp();
			MultP();
			break;
		case T_divide:
			eat_token(T_divide);
			fprintf(stderr, " / ");
			Exp();
			MultP();
			break;
		case T_plus:
			parsetree.drawepsilon();
			break;
		case T_minus:
			parsetree.drawepsilon();
			break;
		case T_goto:
			parsetree.drawepsilon();
			break;
		case T_label:
			parsetree.drawepsilon();
			break;
		case T_m:
			parsetree.drawepsilon();
			break;
		case T_print:
			parsetree.drawepsilon();
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
			syntax_error(NT_Mult_Prime);
			break;
	}


	parsetree.pop();
}

void parser_t::Exp()
{
	parsetree.push(NT_Exp);

	fprintf(stderr, "(int)pow(");
	Paren();
	fprintf(stderr, ", ");
	ExpP();

	parsetree.pop();
}

void parser_t::ExpP()
{
	parsetree.push(NT_Exp_Prime);

	switch( scanner.next_token() ) 
	{
		case T_power:
			eat_token(T_power);
			Paren();
			fprintf(stderr, ")");
			ExpP();
			break;
		case T_times:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_divide:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_plus:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_minus:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_goto:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_label:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_m:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_print:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_closeparen:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_closesquare:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		case T_eof:
			parsetree.drawepsilon();
			fprintf(stderr, "1)");
			break;
		default:
			syntax_error(NT_Exp_Prime);
			break;
	}


	parsetree.pop();
}

void parser_t::Paren()
{
	parsetree.push(NT_Paren);

	switch( scanner.next_token() ) 
	{
		case T_openparen:
			eat_token(T_openparen);
			fprintf(stderr, "(");
			Expression();
			eat_token(T_closeparen);
			fprintf(stderr, ")");
			break;
		case T_num:
			Id();
			break;
		case T_m:
			Id();
			break;
		default:
			syntax_error(NT_Paren);
			break;
	}

	parsetree.pop();
}

void parser_t::Id()
{
	parsetree.push(NT_Id);

	switch( scanner.next_token() ) 
	{
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			fprintf(stderr, "m[");
			Expression();
			fprintf(stderr, "]");
			eat_token(T_closesquare);
			break;
		case T_num:
			int n;
			n = scanner.get_number();
			fprintf(stderr, "%d", n);
			eat_token(T_num);
			break;
		default:
			syntax_error(NT_Id);
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
