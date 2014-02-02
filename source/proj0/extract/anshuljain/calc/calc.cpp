#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <cstring>
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
	default: 	strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
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
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Assignment,
	NT_Print,
	NT_ExpPrime,
	NT_Term,
	NT_TermPrime,
	NT_Power,
	NT_PowerPrime,
	NT_Parenthesis,
	NT_Factor,
	NT_LabelStatements,
	NT_LabelStatement
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
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;		  
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_ExpPrime: strncpy(buffer,"Expression_Complement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_TermPrime: strncpy(buffer,"Term_Complement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Power: strncpy(buffer,"Power",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_PowerPrime: strncpy(buffer,"Power_Complement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Parenthesis: strncpy(buffer,"Parenthesis",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_LabelStatements: strncpy(buffer, "Label Statements", MAX_SYMBOL_NAME_SIZE); break;
		  case NT_LabelStatement: strncpy(buffer,"Label Statement", MAX_SYMBOL_NAME_SIZE); break;
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

	//return string value of current token
	std::string get_token_value();

	//constructor 
	scanner_t();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  
	int line_number;
	int index; //Index of beginning of current token
	int end_of_token;
	std::string file;
	token_type next;
	ssize_t read;

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	token_type bogo_token;

};

//Return string value of current token
std::string scanner_t::get_token_value() 
{
	

	return file.substr(index, end_of_token-index+1);


}


token_type scanner_t::next_token()
{
	
	end_of_token = index; 
	// printf("index in Next Token = %d\n", index);
	if(index >= file.length()) { return T_eof; }

	switch(file[index])
	{
		case '+':  return T_plus; break;
		case '-': return T_minus; break;
		case '*': 
			if(file[index+1] == '*') { end_of_token = index + 1; return T_power; }
			else { return T_times; }
			break;
		case '/': return T_divide; break;
		case '=': return T_equals; break;
		case '(': return T_openparen; break;
		case ')': return T_closeparen; break;
		case '[': return T_opensquare; break;
		case ']': return T_closesquare; break;
		case 'm': return T_m; break;
		case 'L': return T_label; break;
		case ':': return T_colon; break;
		case 'g':
			if(file[index+1] == 'o' 
				&& file[index+2] == 't'
				&& file[index+3] == 'o')
			{
				end_of_token = index+3;
				return T_goto;
			}
		case 'p': 
			if(file[index+1] == 'r' && 
				file[index+2] == 'i' &&
				file[index+3] == 'n' &&
				file[index+4] == 't') 
			{ 
				end_of_token = index+4;
				return T_print;
			}
			else
			{
				scan_error(file[index]);
			}
			break;
		case 'i':
			if(file[index+1] == 'f') {
				end_of_token = index + 1;
				return T_if;
			}
			else {
				scan_error(file[index]);
			}
			break;
		default: 
			if(isdigit(file[index]) != 0) {
				//Check rest of the number, make sure it's an integer
				// printf("Should see this\n");
				while(isdigit(file[end_of_token+1]) != 0) {
					end_of_token++;
				}
				return T_num;
			}
			else {
				scan_error(file[index]);
			}
	}
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
	if(next_token() != c) { mismatch_error(c); }
	else	
	{
		index = end_of_token + 1;
		while(file[index] == ' ') { index++; }
		if(file[index] == '\n') { line_number++; index++;}

		
	}

}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	line_number = 1;
	index = 0;
	// size_t nbytes = 256;
	std::string tmp;
	ssize_t current_read;

	// current_read = getline(&buf, &nbytes, stdin);
	 // printf("have i gotten here\n");
	// file = malloc(sizeof(char) * 1);

	getline(cin, file);

	while( getline(cin, tmp))
	{
		file = file + "\n" + tmp;
	}

	// printf("file is:\n%s\n", file.c_str());

}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
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
		printf( "}\n" );
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
	std::string c_code_main;
	std::string c_code_pre;
	std::string current;
	void eat_token(token_type t);
	void syntax_error(nonterm_type);

	void Start();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)

	void Statements();
	void Statement();
	void Label();
	void LabelStatements();
	void LabelStatement();
	void Jump();
	void Assignment();
	void Print();
	void Expression();
	void ExpressionPrime();
	void Term();
	void TermPrime();
	void Power();
	void PowerPrime();
	void Parenthesis();
	void Factor();

	//finds the beginning of a power expression
	int findBeginningOfExpression();

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
	c_code_pre = "#include <math.h>\n#include <stdio.h>\n";//" int pow(int *num, count) { 	int i; int result = num[0]; for(i=1; i<count; i++) { result = pow(result, num[i]); } return result; }";

	c_code_main = "int main() {\n  int m[101];\n ";
	parsetree.push(NT_Start);

	switch( scanner.next_token() ) 
	{
		case T_label:
		case T_goto:
		case T_m:
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

	//Now that the parsing is done, print out the code to stderr
	// c_code_main = current;
	c_code_main = c_code_main + "return 0; }";
	fprintf(stderr, "%s\n%s", c_code_pre.c_str(), c_code_main.c_str());

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here

void parser_t::Statements()
{
	parsetree.push(NT_Statements);


	switch(scanner.next_token())
	{
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			Statement();
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

void parser_t::Statement()
{
	parsetree.push(NT_Statement);
	current = c_code_main;
	// printf("%s\n", current.c_str());
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
			syntax_error(NT_Statement);
			break;
	}

	c_code_main = current;
	// printf("%s\n", c_code_main.c_str());
	parsetree.pop();
}

void parser_t::Label() 
{
	parsetree.push(NT_Label);

	if(scanner.next_token() == T_label) 
	{
		eat_token(T_label);
		current = c_code_main;
		scanner.next_token();
		current = current + "\n L" + scanner.get_token_value() + ":\n ";
				// printf("current:\n%s\n", current.c_str());

		eat_token(T_num);
		eat_token(T_colon);
		LabelStatements();
		// printf("current:\n%s\n", current.c_str());

   
		c_code_main = current;
		current = c_code_main;
		printf("%s\n", c_code_main.c_str());

	}
	else { syntax_error(NT_Label); }
	parsetree.pop();
}

void parser_t::LabelStatements()
{
	parsetree.push(NT_LabelStatements);

	switch(scanner.next_token())
	{
		case T_m:
		case T_print:
			LabelStatement();
			LabelStatements();
			break;
		case T_goto:
		case T_label:
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_LabelStatement);
			break;
	}


	parsetree.pop();

}

void parser_t::LabelStatement()
{
	parsetree.push(NT_LabelStatement);
	switch(scanner.next_token())
	{
		
		case T_m:
			Assignment();
			break;
		case T_print:
			Print();
			break;
		case T_goto:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_LabelStatement);
			break;
	}

	parsetree.pop();
}

void parser_t::Jump()
{
	parsetree.push(NT_Jump);

	if(scanner.next_token() == T_goto)
	{
		eat_token(T_goto);
		scanner.next_token();
		std::string tmp =  scanner.get_token_value();
		eat_token(T_num);

		//If there's an if statement, handle it it.
		if(scanner.next_token() == T_if) {
			eat_token(T_if);
			current = current + "if(";
			Expression();
			current = current + " != 0) { ";
			current = current + "goto L" + tmp + "; }\n ";
		}
		else {
			current = current + " goto L" + tmp + ";\n ";
		}
	}
	else { syntax_error(NT_Jump); }

	parsetree.pop();
}

void parser_t::Assignment()
{
	parsetree.push(NT_Assignment);



	eat_token(T_m);
	eat_token(T_opensquare);
	current = current + " m[";
		// printf("%s\n", current.c_str());

	Expression();
	eat_token(T_closesquare);
	eat_token(T_equals);
	current = current + "]=";
	// printf("%s\n", current.c_str());

	Expression();
	current = current + ";\n ";

	parsetree.pop();
}

void parser_t::Print() 
{
	parsetree.push(NT_Print);

	eat_token(T_print);
	current = current + "printf(\"%d\\n\", ";

	Expression();

	current = current + ");\n ";
	parsetree.pop();
}

void parser_t::Expression()
{
	parsetree.push(NT_Expression);

	Term();
	ExpressionPrime();

	parsetree.pop();
}

void parser_t::ExpressionPrime()
{
	parsetree.push(NT_ExpPrime);

	switch(scanner.next_token())
	{
		case T_plus:
			eat_token(T_plus);
			current = current + "+ ";
			Term();
			ExpressionPrime();
			break;
		case T_minus:
			eat_token(T_minus);
			current = current + "-";
			Term();
			ExpressionPrime();
			break;

		// All included in the follow set of ExpPrime
		case T_closesquare:
		case T_closeparen:
		case T_eof:
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			parsetree.drawepsilon();
			break;

		// Otherwise throw syntax error
		default:
			syntax_error(NT_ExpPrime);
			break;
	}

	parsetree.pop();
}

void parser_t::Term()
{
	parsetree.push(NT_Term);

	Power();
	TermPrime();

	parsetree.pop();
}

void parser_t::TermPrime()
{
	parsetree.push(NT_TermPrime);

	switch(scanner.next_token())
	{
		case T_times:
			eat_token(T_times);
			current = current + "*";
			// printf("%s\n", current.c_str());
			Power();
			TermPrime();
			break;

		case T_divide:
			eat_token(T_divide);
			current = current + "/";
			Power();
			TermPrime();
			break;

		//Follow Set of TermPrime
		case T_plus:
		case T_minus:
		case T_closesquare:
		case T_closeparen:
		case T_eof:
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			parsetree.drawepsilon();
			break;

		// Otherwise throw syntax error
		default:
			syntax_error(NT_TermPrime);
			break;
	}

	parsetree.pop();
}


void parser_t::Power()
{
	parsetree.push(NT_Power);

	Parenthesis();
	PowerPrime();

	parsetree.pop();
}

void parser_t::PowerPrime()
{
	parsetree.push(NT_PowerPrime);

	if(scanner.next_token() == T_power)
	{
		//Possibilities:
		//Last character was either a number or a ')'. 
		//If it's a parenthesis, we need to parse backwards to get  the full expression,
		//Either a Pow(,) or a (expr)
		int n = current.length();

		std::string tmp = current;
		
		if(isdigit(current[n-1])) {
			current.erase(0, n-1);
			tmp.erase(n-1, 1);
			current = "pow(" + current;
		}
		else {
			int beginning = findBeginningOfExpression();
			tmp = tmp.substr(0,beginning);
			current = "pow(" + current.substr(beginning, current.length()-beginning);
		}


		eat_token(T_power);
		current = current + ",";
		Parenthesis();


		current = current + ")";
		tmp = tmp + current;

		current = tmp;
		PowerPrime();
	}
	else {
		//Follow Set includes
		// *, /, +, -, ], ), eof, L, goto, m, print
		switch(scanner.next_token()) {
			case T_times:
			case T_divide:
			case T_plus:
			case T_minus:
			case T_closesquare:
			case T_closeparen:
			case T_eof:
			case T_label:
			case T_goto:
			case T_m:
			case T_print:
				parsetree.drawepsilon();
				break;

			default:
				syntax_error(NT_PowerPrime);
		}
	}

	parsetree.pop();
}
int parser_t::findBeginningOfExpression()
{
	int index = current.length() - 2;
	int count = 1;
	// if(current[index] != ')') { return -1; }
	while(count > 0 && index > 0) {
		if(current[index] == ')') { count++; }
		else if(current[index] == '(') { count--;}
		// printf("count: %d, index: %d\n", count, index);

		index--;
	}
	index++;
	
	if(current[index-1] == 'w') {
		index = index-3;
	}

	return index;

}
void parser_t::Parenthesis() 
{
	parsetree.push(NT_Parenthesis);
	if(scanner.next_token() == T_openparen) 
	{
		eat_token(T_openparen);
		current = current + "(";
		Expression();
		current = current + ")";
		eat_token(T_closeparen);
	}
	else 
	{
		Factor();
	}

	parsetree.pop();
}

void parser_t::Factor()
{
	parsetree.push(NT_Factor);

	if(scanner.next_token() == T_num) 
	{
		current = current + scanner.get_token_value();
		// printf("%s\n", current.c_str());
		eat_token(T_num);
	}
	else if(scanner.next_token() == T_m) 
	{
		eat_token(T_m);
		eat_token(T_opensquare);
		current = current + "m[";
		Expression();

		eat_token(T_closesquare);
		current = current + "]";
	}
	else { syntax_error(NT_Factor); }

	parsetree.pop();

}

/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();

	return 0;

}
