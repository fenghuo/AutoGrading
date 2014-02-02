// Omar Masri - CS160 Project 0
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <iostream> 
#include <stack>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25

//all of the terminals in the language
typedef enum {
	T_eof = 0,			// 0: end of file
	T_num,				// 1: numbers
	T_plus,				// 2: +
	T_minus,			// 3: -
	T_times,			// 4: *
	T_divide,			// 5: /
	T_power, 			// 6: **
	T_equals,			// 7: =
	T_openparen,		// 8: (
	T_closeparen, 		// 9: )
	T_opensquare,		// 10: [
	T_closesquare,		// 11: ]
	T_m,				// 12: m
	T_goto,				// 13: goto
	T_if,				// 14: if
	T_print,			// 15: print
	T_label,			// 16: L
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
		case T_eof:				strncpy(buffer,"eof",MAX_SYMBOL_NAME_SIZE); break;
		case T_num:				strncpy(buffer,"num",MAX_SYMBOL_NAME_SIZE); break;
		case T_plus:			strncpy(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
		case T_minus:			strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
		case T_times:			strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
		case T_divide:			strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
		case T_power:			strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
		case T_equals:			strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
		case T_openparen:		strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
    	case T_closeparen:		strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
    	case T_opensquare:		strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
    	case T_closesquare:		strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
    	case T_m:				strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
    	case T_goto:			strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
    	case T_if:				strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
    	case T_print:			strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
    	case T_label:			strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
    	case T_colon:			strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
		default: 				strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
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
	NT_Goto,
	NT_Num,
	NT_Assignment,
	NT_Print,
	NT_ExpressionP,
	NT_Term,
	NT_TermP,
	NT_Factor,
	NT_Pow,
	NT_PowP

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
    		case NT_Goto: strncpy(buffer,"Goto",MAX_SYMBOL_NAME_SIZE); break;
    		case NT_Num: strncpy(buffer,"Num",MAX_SYMBOL_NAME_SIZE); break;
   		 	case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
    		case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
    		case NT_ExpressionP: strncpy(buffer,"ExpressionP",MAX_SYMBOL_NAME_SIZE); break;
    		case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
   	 		case NT_TermP: strncpy(buffer,"TermP",MAX_SYMBOL_NAME_SIZE); break;
  	  		case NT_Pow: strncpy(buffer,"Pow",MAX_SYMBOL_NAME_SIZE); break;
    		case NT_PowP: strncpy(buffer,"PowP",MAX_SYMBOL_NAME_SIZE); break;
    		case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;	
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

	string num;		// stores numbers as a string
	bool newln;     

  private:

  	int linenumber; // used to keep track of the line on which an error occurs (if any)
  	char tok;		// stores the current token

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);


	

};

token_type scanner_t::next_token()
{

	while(true){

		tok = cin.peek();
		switch(tok){
			case EOF:			return T_eof;
			case '0' ... '9':	return T_num;	// checked in eat_token()
			case '+': 	 		return T_plus;
			case '-': 	 		return T_minus;
			case '*':  
				
				tok = cin.get(); 
				if(cin.peek() == '*'){
					cin.putback('*');
					return T_power;
				}
				else{
					cin.putback('*');
					return T_times;
				}

			case '=':	return T_equals;
			case '/':  	return T_divide;
			case '(':  	return T_openparen;
			case ')':  	return T_closeparen;
			case '[':	return T_opensquare;
			case ']':	return T_closesquare;
			case 'm':	return T_m;
			case 'g':	return T_goto; 			// checked in eat_token()
			case 'i':	return T_if;			// checked in eat_token()
			case 'p':	return T_print;			// checked in eat_token()
			case 'L':	return T_label; 		// checked in eat_token()
			case ':':	return T_colon;
			
			case '\n':
				tok = cin.get();
				newln = true;
				break;

			case ' ': 
			case '\t': 
				tok = cin.get(); 
				break;

			default:	scan_error(tok);
		}
	}


}

void scanner_t::eat_token(token_type c)
{
	if(newln) { newln = false; this->linenumber++; }

	switch(c){
		case 0: 							// T_eof
		case 1:								// T_num
			num = "";
			num += cin.get();
			while(next_token() == 1){
				num += cin.get();
			}
			if(atof(num.c_str()) > 2147483647){
				cout << "number out of range, maximum is 2^31 - 1\n";
				exit(1);
			}
			else
				break;

		case T_plus: 						// T_plus
			tok = cin.get();
			break;

		case T_minus: 						// T_minus
			tok = cin.get();
			break;

		case T_times: 						// T_times
			tok = cin.get();
			break;

		case T_divide: 						// T_divide
			tok = cin.get();
			break;

		case T_power: 						// T_power
			tok = cin.get();
			tok = cin.get();
			break;

		case T_equals: 						// T_equals
			tok = cin.get();
			break;

		case T_openparen: 					// T_openparen
			tok = cin.get();
			break;

		case T_closeparen: 					// T_closeparen
			tok = cin.get();
			break;

		case T_opensquare: 					// T_opensquare
			tok = cin.get();
			break;

		case T_closesquare: 				// T_closesquare
			tok = cin.get();
			break;

		case T_m: 							// T_m
			tok = cin.get();
			break;

		case T_goto: 						// T_goto
			tok = cin.get();
			if(cin.peek() == 'o'){
				tok = cin.get();
				if(cin.peek() == 't'){
					tok = cin.get();
					if(cin.peek() == 'o'){
						tok = cin.get();
						break;
					}
					else scan_error(cin.peek());
				}
				else scan_error(cin.peek());
			}
			else scan_error(cin.peek());

		case T_if:
			tok = cin.get();
			if(cin.peek() == 'f'){
				tok = cin.get();
				break;
			}
			else scan_error(cin.peek());
		
		case T_print:
			tok = cin.get();
			if(cin.peek() == 'r'){
				tok = cin.get();
				if(cin.peek() == 'i'){
					tok = cin.get();
					if(cin.peek() == 'n'){
						tok = cin.get();
						if(cin.peek() == 't'){
							tok = cin.get();
							break;
						}
						else scan_error(cin.peek());
					}
					else scan_error(cin.peek());
				}
				else scan_error(cin.peek());
			}
			else scan_error(cin.peek());

		
		case T_label:
			tok = cin.get();
			break;

		case T_colon:
			tok = cin.get();
			break;

		default: 
			mismatch_error(c);

	}
}

scanner_t::scanner_t(){
	this->linenumber = 1;
	newln = false;
}

int scanner_t::get_line(){
	return this->linenumber;
}

void scanner_t::scan_error (char x){
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
	//string cur_line;

	void eat_token(token_type t);
	void syntax_error(nonterm_type);

	void Start();
	void Statements();
	void Statement();
	void Label(string &s);
  	void Jump(string &s);
 	void Goto(string &s);
  	void Num(string &s);
  	void Assignment(string &s);
 	void Print(string &s);
 	void Expression(string &s);
  	void ExpressionP(string &s);
  	void Term(string &s);
  	void TermP(string &s);
  	void Pow(string &s);
  	void PowP(string &s);
  	void Factor(string &s);



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
	int l = scanner.get_line();
	//if(scanner.newln) l--;
	
	printf("syntax error: found %s in parsing %s - line %d\n",	
		token_to_string( scanner.next_token()),
		nonterm_to_string(nt),
			l ); 
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
	
	parsetree.push(NT_Start);

	int t = scanner.next_token();

	if(t == T_print || t == T_m || t == T_goto || t == T_label){
		Statement();
		Statements();
	}
	else
		syntax_error(NT_Start);

	parsetree.pop();

}

void parser_t::Statements()
{

	parsetree.push(NT_Statements);

	int t = scanner.next_token();

	if(t == T_print || t == T_m || t == T_goto || t == T_label){
		Statement();
		Statements();
	}
	else if(t == T_eof)
		parsetree.drawepsilon();
	else
		syntax_error(NT_Statements);

	parsetree.pop();
}



void parser_t::Statement()
{

	parsetree.push(NT_Statement);
	string cur_line;

	if(scanner.next_token() == T_print)
		Print(cur_line);
	else if(scanner.next_token() == T_m)
		Assignment(cur_line);
	else if(scanner.next_token() == T_label)
		Label(cur_line);
	else if(scanner.next_token() == T_goto)
		Jump(cur_line);
	else
		syntax_error(NT_Statement);
	
	parsetree.pop();

	cerr << cur_line << ";\n";
}


void parser_t::Label(string &s)
{
	
	parsetree.push(NT_Label);
	
	eat_token(T_label);
	int t = scanner.next_token();
	
	if(t != T_num)
		syntax_error(NT_Label);

	
	
	eat_token(T_num);
	s += "L" + scanner.num;

	t = scanner.next_token();
	if(t != T_colon)
		syntax_error(NT_Label);

	s += ":";
	eat_token(T_colon);
	
	t = scanner.next_token();
	if( t == T_print || t == T_goto || t == T_label || t == T_m)
		parsetree.pop();
	else
		syntax_error(NT_Label);

}

void parser_t::Jump(string &s) {
  parsetree.push(NT_Jump);

  eat_token(T_goto);
  Goto(s);

  int t = scanner.next_token();
	if( t == T_print || t == T_goto || t == T_label || t == T_m)
		parsetree.pop();
	else
		syntax_error(NT_Jump);

}

void parser_t::Goto(string &s) {
  	
  	string lhs;
	parsetree.push(NT_Goto);
	eat_token(T_num);
	string n = scanner.num;
	n.erase(0,1);
	int t = scanner.next_token();

	if(t == T_if){
		lhs += "if(";
		Num(lhs);
		lhs += "){";
		lhs += "goto L" + n + ";";
		lhs += "}";
	}
	else{
		Num(lhs);
		lhs += "goto L" + n;
	}

	s += lhs;

	parsetree.pop();

}

void parser_t::Num(string &s) {
	parsetree.push(NT_Num);
	int t = scanner.next_token();
    
    if (t == T_if){
      eat_token(T_if);
      Expression(s);
    }

  	t = scanner.next_token();
	if( t == T_print || t == T_goto || t == T_label || t == T_m || t == T_eof){
		parsetree.drawepsilon();
		parsetree.pop();
	}
	else
		syntax_error(NT_Num);

}
// Assignment --> 'm' '[' Expression ']' '=' Expression
void parser_t::Assignment(string &s) {
	parsetree.push(NT_Assignment);
	eat_token(T_m);
	s += "m";

	if(scanner.next_token() == T_opensquare){
		eat_token(T_opensquare);
		s += "[";
	}
	else syntax_error(NT_Assignment);
	
	Expression(s);

	if(scanner.next_token() == T_closesquare){
		eat_token(T_closesquare);
		s += "]";
	}
	else syntax_error(NT_Assignment);

	if(scanner.next_token() == T_equals){
		eat_token(T_equals);
		s += "=";
	}
	else syntax_error(NT_Assignment);

	Expression(s);

	int t = scanner.next_token();
	if( t == T_print || t == T_goto || t == T_label || t == T_m || t == T_eof){
		parsetree.pop();
	}
	else
		syntax_error(NT_Assignment);
}

// Print --> 'p' Expression
void parser_t::Print(string &s)
{
	parsetree.push(NT_Print);
	eat_token(T_print);


	s += "printf(\"%d\\n\", ";
	Expression(s);
	s += ")";


	int t = scanner.next_token();
	if( t == T_print || t == T_goto || t == T_label || t == T_m || t == T_eof){
		parsetree.pop();
	}
	else
		syntax_error(NT_Print);

}

// Expression --> Term ExpressionP
void parser_t::Expression(string &s)
{
	parsetree.push(NT_Expression);
	int t = scanner.next_token();
	if(t == T_openparen || t == T_m || t == T_num){
		Term(s);
	}
	else syntax_error(NT_Expression);

	ExpressionP(s);

	t = scanner.next_token();
	if(t == T_closeparen || t == T_closesquare || t == T_label
		|| t == T_goto || t == T_print || t == T_m || t == T_eof){
		parsetree.pop();
	}
	else syntax_error(NT_Expression);

}

// ExpressionP --> '+' Term ExpressionP | '-' Term ExpressionP | epsilon
void parser_t::ExpressionP(string &s)
{
	parsetree.push(NT_ExpressionP);

	if(scanner.next_token() == T_plus){
		eat_token(T_plus);
		s += "+"; Term(s); ExpressionP(s);
	}
	if(scanner.next_token() == T_minus){
		eat_token(T_minus);
		s += "-"; Term(s); ExpressionP(s);
	}

	int t = scanner.next_token();
	if(t == T_closeparen || t == T_closesquare || t == T_label
		|| t == T_goto || t == T_print || t == T_m || t == T_eof){
		parsetree.drawepsilon();
		parsetree.pop();
	}
	else syntax_error(NT_ExpressionP);

}

// Term --> Factor TermP
void parser_t::Term(string &s)
{
	parsetree.push(NT_Term);
	int t = scanner.next_token();

	if(t == T_openparen || t == T_num || t == T_m){
		Factor(s); TermP(s);
	}
	else syntax_error(NT_Term);

	t = scanner.next_token();
	if(t == T_plus || t == T_minus || t == T_closeparen || t == T_closesquare
		|| t == T_label || t == T_goto || t == T_m || t == T_print || t == T_eof){
		parsetree.pop();
	}
	else syntax_error(NT_Term);
}

// TermP --> '*' Pow TermP  | '/' Pow TermP | epsilon
void parser_t::TermP(string &s)
{
	parsetree.push(NT_TermP);

	if(scanner.next_token() == T_times){
		eat_token(T_times);
		s += "*"; Pow(s); TermP(s);
	}
	if(scanner.next_token() == T_divide){
		eat_token(T_divide);
		s += "/"; Pow(s); TermP(s);
	}

	int t = scanner.next_token();
	if(t == T_plus || t == T_minus || t == T_closeparen || t == T_closesquare
		|| t == T_label || t == T_goto || t == T_m || t == T_print || t == T_eof){
		parsetree.drawepsilon();
		parsetree.pop();
	}
	else syntax_error(NT_TermP);


}


void parser_t::Factor(string &s)
{
	parsetree.push(NT_Factor);
	string lhs, rhs;
	int t = scanner.next_token();

	if(t == T_openparen || t == T_m || t == T_num){
		Pow(lhs);
	}
	else syntax_error(NT_Factor);

	if(scanner.next_token() == T_power){
		lhs = "(int)pow(" + lhs;
		rhs += ",";
		PowP(rhs);
		rhs += ")";
	}
	else PowP(rhs);

	s += lhs + rhs;

	t = scanner.next_token();
	if(t == T_plus || t == T_minus || t == T_closeparen || t == T_closesquare
		|| t == T_label || t == T_goto  || t == T_m || t == T_print 
		|| t == T_eof   || t == T_times || t == T_divide){
		parsetree.pop();
	}
	else syntax_error(NT_Factor);
}

// PowP --> 'e' Factor
void parser_t::PowP(string &s)
{
	parsetree.push(NT_PowP);
	if(scanner.next_token() == T_power){
		eat_token(T_power); Factor(s);
	}

	int t = scanner.next_token();
	if(t == T_plus || t == T_minus || t == T_closeparen || t == T_closesquare
		|| t == T_label || t == T_goto  || t == T_m || t == T_print 
		|| t == T_eof   || t == T_times || t == T_divide){
		parsetree.drawepsilon();
		parsetree.pop();
	}
	else syntax_error(NT_PowP);
}

// Pow --> '(' Expression ')' | 'm' '[' Expresssion ']' | 'n'
void parser_t::Pow(string &s)
{
	parsetree.push(NT_Pow);

	if(scanner.next_token() == T_openparen){
		eat_token(T_openparen);
		s += "("; Expression(s);

		if(scanner.next_token() != T_closeparen) syntax_error(NT_Pow);
		eat_token(T_closeparen);
		s += ")";
	}

	else if(scanner.next_token() == T_m){
		eat_token(T_m); s += "m";

		if(scanner.next_token() != T_opensquare) syntax_error(NT_Pow);
		eat_token(T_opensquare); s += "["; Expression(s);

		if(scanner.next_token() != T_closesquare) syntax_error(NT_Pow);	
		eat_token(T_closesquare); s += "]";
	}


	else if(scanner.next_token() == T_num){
		eat_token(T_num); s += scanner.num; 
	}

	int t = scanner.next_token();
	if(t == T_plus || t == T_minus || t == T_closeparen || t == T_closesquare
		|| t == T_label || t == T_goto  || t == T_m || t == T_print 
		|| t == T_eof   || t == T_times || t == T_divide || t == T_power){
		parsetree.pop();
	}
	else syntax_error(NT_Pow);


}

/*** Main ***********************************************/

int main()
{
	scanner_t scanner;
    parser_t parser;
    cerr << "#include <stdio.h>\n#include <math.h>\n"
    	 << "#include <stdlib.h>\nint main() {\nint m[101];\n";
 	parser.parse();
 	cerr << "\n}\n";
	return 0;
}
