#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <sstream>
#include <list>

using namespace std;
#if _MSC_VER
#define snprintf _snprintf
#endif

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
	T_colon,				// 17: :
	T_null
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
	NT_JumpPrime,
	NT_Assignment,
	NT_Print,
	NT_Expression,
	NT_ExpressionPrime,
	NT_Term,
	NT_TermPrime,
	NT_Factor,
	NT_AfterM
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
		  case NT_JumpPrime: strncpy(buffer,"JumpPrime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_ExpressionPrime: strncpy(buffer,"ExpressionPrime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_TermPrime: strncpy(buffer,"TermPrime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_AfterM: strncpy(buffer,"AfterM",MAX_SYMBOL_NAME_SIZE); break;
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

	//keeps track of numbers
	char num_count_numbers[11];

  private:

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

	token_type peeked_token;
	int current_line;
	int num_count;
	//determines if we have eaten the token and want the next token.
	bool needs_new_token;

	bool has_read_number;
};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in
	bool has_token = false;
	char character;

	if (needs_new_token == true)
	{
		//if we are looking for a new token, let the while loop know we are looking for one.
		peeked_token = T_null;

		while (peeked_token == T_null)
		{
			//We can use peek on deeper switch statements to make decisions.
			cin.get(character);
			if (character < 0 || cin.eof() || !cin.good()) {
				peeked_token = T_eof;
			}
			else if (character == '\n') {
				current_line++;
			}
			else if (character == '+') peeked_token = T_plus;
			else if (character == '-') peeked_token = T_minus;
			else if (character == '*')
			{
				if (cin.peek() == '*')
				{
					cin >> character;
					peeked_token = T_power;
				}
				else peeked_token = T_times;
			}
			else if (character == '/') peeked_token = T_divide;
			else if (character == '=') peeked_token = T_equals;
			else if (character == '(') peeked_token = T_openparen;
			else if (character == ')') peeked_token = T_closeparen;
			else if (character == '[') peeked_token = T_opensquare;
			else if (character == ']') peeked_token = T_closesquare;
			else if (character == 'm') peeked_token = T_m;
			else if (character == 'g') {
				if (cin.peek() == 'o') {
					cin.get(character);
					if (cin.peek() == 't') {
						cin.get(character);
						if (cin.peek() == 'o') {
							cin.get(character);
							peeked_token = T_goto;
						}
					}
				}
			}
			else if (character == 'i') {
				if (cin.peek() == 'f') {
					cin.get(character);
					peeked_token = T_if;
				}
			}
			else if (character == 'p') {
				if (cin.peek() == 'r') {
					cin.get(character);
					if (cin.peek() == 'i') {
						cin.get(character);
						if (cin.peek() == 'n') {
							cin.get(character);
							if (cin.peek() == 't') {
								cin.get(character);
								peeked_token = T_print;
							}
						}
					}
				}
			}
			else if (character == 'L') {
				peeked_token = T_label;
			}
			else if (character == ':') peeked_token = T_colon;
			else if (character >= 48 && character <= 57) 
			{
				if (has_read_number)
				{
					//fprintf(stderr, num_count_numbers);
					for (int i = 0; i < num_count; i++) 
					{
						num_count_numbers[i] = 0;
					}
					num_count = 0;
					has_read_number = false;
				}

				num_count_numbers[num_count] = character;
				num_count++;
				if (cin.peek() >= 48 && cin.peek() <= 57)
				{
					continue;
				}
				peeked_token = T_num;
			}
			else if (character <= 32) continue;
			else {
				scan_error(character);
			}
		}

		//Now that we have peeked the token, we dont need one right now.
		needs_new_token = false;
	
		//Make  sure our number isn't too large.
		//Whoever reads this comment, sorry about the nasty if statement.
		if (peeked_token == T_num)
		{
			if (num_count == 10)
			{
				if (num_count_numbers[0] >= '2')
				{
					scan_error(character);
					if (num_count_numbers[1] >= '1')
					{
						scan_error(character);
						if (num_count_numbers[2] >= '4')
						{
							scan_error(character);
							if (num_count_numbers[3] >= '7')
							{
								scan_error(character);
								if (num_count_numbers[4] >= '4')
								{
									scan_error(character);
									if (num_count_numbers[5] >= '8')
									{
										scan_error(character);
										if (num_count_numbers[6] >= '3')
										{
											scan_error(character);
											if (num_count_numbers[7] >= '6')
											{
												scan_error(character);
												if (num_count_numbers[8] >= '4')
												{
													if (num_count_numbers[9] >= '7')
														scan_error(character);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			else if (num_count > 10)
			{
				scan_error(character);
			}
			has_read_number = true;
		}
		return peeked_token;
	}
	else
	{
		return peeked_token;
	}
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	if (c != peeked_token)
	{
		mismatch_error(c);
	}
	else
	{
		//We have now eaten this token, and we are ready for more!
		peeked_token = T_null;
		needs_new_token = true;
	}
}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	needs_new_token = true;
	current_line = 1; //We are currently at line 1;
	num_count = 0; //By default, we have counted 0 numbers sequentially.
	has_read_number = false;

	for (int i = 0; i < 11; i++) 
		num_count_numbers[i] = 0;
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
	return current_line;
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
	//Variables
	scanner_t scanner;
	parsetree_t parsetree;

	bool shouldPrintSemicolon;
	char* expression_derivation;
	int expression_derivation_size;
	bool shouldPrintPowerSet;
	

	list<token_type> token_list;
	list<int> token_number_list;

	//Methods
	void print_token_list(bool power);
	void eat_token(token_type t);
	void syntax_error(nonterm_type);
	void Start();
	//recursive decent stuff (more methods)
	void Statements();
	void Statement();
	void Label();
	void Jump();
	void JumpPrime();
	void Assignment();
	void Print();
	void Expression();
	void ExpressionPrime();
	void Term();
	void TermPrime();
	void Factor();
	void AfterM();
	void print_semicolon();

public:
	//Methods
	void parse();
};

void parser_t::print_semicolon()
{
	fprintf(stderr, ";");
}

void parser_t::print_token_list(bool power)
{
	//Empty the list
	while(token_list.size() != 0)
	{
		if (token_list.front() == T_num && token_list.size() == 1 && power)
		{
			break;
		}

		if (token_list.front() == T_num)
		{
			fprintf(stderr,"%d",token_number_list.front());
			token_number_list.pop_front();
			token_list.pop_front();
		}
		else
		{
			fprintf(stderr, token_to_string(token_list.front()));
			token_list.pop_front();
		}
	}
}

//this function not only eats the token (moving the scanner forward one
//token), it also makes sure that token is drawn in the parse tree 
//properly by calling push and pop.
void parser_t::eat_token(token_type t)
{
	//For printing the token
	//if (t != T_num)
	//	fprintf(stderr, token_to_string(t));

	if (t != T_power && t != T_if && t != T_print)
		token_list.push_back(t);

	if (t == T_num)
	{
		token_number_list.push_back(atoi(scanner.num_count_numbers));
	}

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
	shouldPrintPowerSet = true;
	Start();
}

//The start
void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);

	fprintf(stderr, "#include <iostream>\n#include <math.h>\nusing namespace std;\n\nint main() {\n");
	fprintf(stderr, "int m[101];\n");
	//Start the statements
	Statements();

	fprintf(stderr, "return 0;\n}");

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

//The rest of the procedures for parsing.
void parser_t::Statements()
{
	parsetree.push(NT_Statements);

	while ( scanner.next_token() != T_eof )
	{
		shouldPrintSemicolon = true;
		Statement();
		//prints token list
		print_token_list(false);
		//end token list
		if (shouldPrintSemicolon)
			print_semicolon();
		fprintf(stderr, "\n");
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
		default: syntax_error(NT_Start); break;
	}

	parsetree.pop();
}

void parser_t::Label()
{
	parsetree.push(NT_Label);

	switch ( scanner.next_token() )
	{
		case T_label:
			eat_token(T_label);
			switch ( scanner.next_token() )
			{
				case T_num: 
					eat_token(T_num); 
					switch ( scanner.next_token() )
					{
						case T_colon: 
							eat_token(T_colon);
							shouldPrintSemicolon = false;
							//prints token list
							print_token_list(false);
							//end printing token list
							break;
						default:
							syntax_error(NT_Label);
							break;
					}
					break;
				default : syntax_error(NT_Label); break;
			} 
			break;
		default:
			syntax_error(NT_Label); break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Jump()
{
	parsetree.push(NT_Jump);
	switch ( scanner.next_token() )
	{
		case T_goto:
			eat_token(T_goto);
			switch ( scanner.next_token() )
			{
				case T_num:
					eat_token(T_num);
					JumpPrime();
					break;
				default:
					syntax_error(NT_Jump);
					break;
			}
			break;
		default:
			syntax_error(NT_Jump); break;
	}
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::JumpPrime()
{
	parsetree.push(NT_JumpPrime);
	int tmp_number = token_number_list.front();
	switch ( scanner.next_token() )
	{
		case T_if:
			eat_token(T_if);
			//Empty number list and goto.
			token_list.clear();
			token_number_list.clear();
			//Used to print if statement
			fprintf(stderr, "if (");
			Expression();
			//prints token list
			print_token_list(false);
			//end printing token list
			fprintf(stderr, ") {\ngoto L%d;\n}\n", tmp_number);
			shouldPrintSemicolon = false;
			break;
		default: 
			token_list.clear();
			token_number_list.clear();
			fprintf(stderr, "goto L%d;", tmp_number);
			shouldPrintSemicolon = false;
			parsetree.drawepsilon(); 
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Assignment()
{
	parsetree.push(NT_Assignment);
	switch ( scanner.next_token() )
	{
		case T_m:
		eat_token(T_m);
		switch ( scanner.next_token() )
		{
			case T_opensquare:
				eat_token(T_opensquare);
				Expression();
				switch ( scanner.next_token () )
				{ 
					case T_closesquare:
						eat_token(T_closesquare);
						switch ( scanner.next_token () )
						{ 
							case T_equals:
								eat_token(T_equals);
								//prints token list
								print_token_list(false);
								//end printing token list
								Expression();
								break;
							default: syntax_error(NT_Assignment); break;
						}
						break;
					default: syntax_error(NT_Assignment); break;
				}
				break;
			default: syntax_error(NT_Assignment); break;
		}
		break;
		default: syntax_error(NT_Assignment); break;
	}
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Print()
{
	parsetree.push(NT_Print);

	switch ( scanner.next_token() )
	{
		case T_print:
			eat_token(T_print);
			fprintf(stderr, "cout << ");
			//prints token list
			print_token_list(false);
			//end printing token list
			Expression();
			//prints token list
			print_token_list(false);
			//end printing token list

			fprintf(stderr, " << endl;");
			shouldPrintSemicolon = false;
			break;
		default: syntax_error(NT_Print); break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Expression()
{
	parsetree.push(NT_Expression);

	Term();
	ExpressionPrime();
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::ExpressionPrime()
{
	parsetree.push(NT_ExpressionPrime);
	
	switch ( scanner.next_token() )
	{
		case T_plus:
			eat_token(T_plus);
			Term();
			ExpressionPrime();
			break;
		case T_minus:
			eat_token(T_minus);
			Term();
			ExpressionPrime();
			break;
		default: 
			parsetree.drawepsilon();
			break;
	}
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Term()
{
	parsetree.push(NT_Term);
	Factor();
	TermPrime();
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::TermPrime()
{
	parsetree.push(NT_TermPrime);
	int x = 0;

	switch ( scanner.next_token() ) 
	{
		case T_times: 
			eat_token(T_times);
			Factor();
			TermPrime();
			break;
		case T_divide:
			eat_token(T_divide);
			Factor();
			TermPrime();
			break;
		case T_power:
			//prints token list
			if (shouldPrintPowerSet)
				print_token_list(true);
			else
				shouldPrintPowerSet = true;
			//end printing token list
			eat_token(T_power);
			fprintf(stderr, "pow(");
			//prints token list
			print_token_list(false);
			//end printing token list
			fprintf(stderr, ",");
			Factor();
			TermPrime();
			//prints token list
			print_token_list(false);
			//end printing token list
			fprintf(stderr, ")");
			break;
		default: parsetree.drawepsilon(); break;
	}
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Factor()
{
	parsetree.push(NT_Factor);

	switch ( scanner.next_token() )
	{
		case T_openparen:
			eat_token(T_openparen);
			Expression();
			switch ( scanner.next_token() )
			{
				case T_closeparen:
					eat_token(T_closeparen);
					shouldPrintPowerSet = false;
					break;
				default: syntax_error(NT_Factor); break;
			}
			break;
		case T_num:
			eat_token(T_num);
			break;
		case T_m:
			eat_token(T_m);
			AfterM();
			break;
		default: syntax_error(NT_Factor); break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::AfterM()
{
	parsetree.push(NT_AfterM);

	switch ( scanner.next_token() )
	{
		case T_opensquare:
			eat_token(T_opensquare);
			Expression();
			switch ( scanner.next_token() )
			{
				case T_closesquare:
					eat_token(T_closesquare);
					shouldPrintPowerSet = false;
					break;
				default:
					syntax_error(NT_AfterM);
					break;
			}
		default: 
			parsetree.drawepsilon(); 
			break;
	}
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}


/*** Main ***********************************************/

int main()
{
	//istringstream iss("print m[5]");
	//istringstream iss("goto 1b");
	//istringstream iss("m[0] = 5\mm[1] = 2\mm[10] = 3 * 5\nm[100] = 5 + (6)\nm[50] = 5 + (4 - 6)\nm[51] = 4 - 6\nm[0] = 5 - (4 - 6)\nm[60] = 4\nL1:\nm[60] = m[60] - 1\nm[0] = 5 / 2 * 3\nm[4] = 2 + 2\nm[0] = 5 - (m[4] - 6)\nm[0] = 5 / 2 * 3\nm[0] = 2 + m[2]\nm[0] = 5 - m[4 - 6]\nm[3] = 5 - (4 - 6)\nm[2] = 5 - m[(6 - 4)]\nm[2] = m[m[3]]\nprint m[3]\ngoto 1 if m[60]\nprint m[2]\ngoto 2\nprint m[0]\nL2:\nprint m[0]");
	//istringstream iss("m[2] = 5**5**m[m[3]]**4");
	//cin.rdbuf(iss.rdbuf());

	parser_t parser;
	parser.parse();

	return 0;
}
