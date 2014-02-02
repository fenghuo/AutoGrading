#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <sstream>

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
	//WRITEME: add symbolic names for you non-terminals here
	NT_STATEMENTS,
	NT_SPRIME,
	NT_STATEMENT,
	NT_LABEL,
	NT_JUMP,
	NT_ASSIGN,
	NT_PRINT,
	NT_EXPRPRIME,
	NT_EXPRPRIMEPRIME,
	NT_STARSTARPRIME,
	NT_JUMPAGAIN

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
		  case NT_STATEMENTS: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_SPRIME: strncpy(buffer,"Statements Prime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_STATEMENT: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_LABEL: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_JUMP: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_ASSIGN: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_PRINT: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_EXPRPRIME: strncpy(buffer,"Expresison Prime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_EXPRPRIMEPRIME: strncpy(buffer,"Expression prime prime",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_STARSTARPRIME: strncpy(buffer,"Star-Star",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_JUMPAGAIN: strncpy(buffer,"Jump", MAX_SYMBOL_NAME_SIZE); break;		
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

	char* getnum();
	int number = 0;

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  
	bool canwepeek;
	token_type peeker;
	int line;
	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

};

char* scanner_t::getnum(){
	//int digits = 0;
	//if(digits > 10){ scan_error(
	char* res = new char[25];
	sprintf(res, "%d", number);
	return res;	
}

token_type scanner_t::next_token()
{
	//not acitually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 

	if(canwepeek) return peeker;	
	//cout << "made it" << endl;
	int c = cin.get();
	//cout << "hello" << endl;
	switch(c){
		case '\n': line++; return next_token(); break;
		case ' ': return next_token(); break;
		case EOF: peeker = T_eof; break;
		case '+': peeker = T_plus; break;
		case '-': peeker = T_minus; break;
		case '*': { 
			int c2 = cin.get(); 
			if(c2 == '*'){peeker = T_power; break;} 
			else{peeker = T_times; break;}
			}
		case '/': peeker = T_divide; break;
		case '=': peeker = T_equals; break;
		case '(': peeker = T_openparen; break;
		case ')': peeker = T_closeparen; break;
		case '[': peeker = T_opensquare; break;
		case ']': peeker = T_closesquare; break;
		case 'm': peeker = T_m; break;
		//need multiple levels to determine, throw immediately if incorrect, else just put all back at end
		case 'g': {
			int lookforo = cin.get();
			if(lookforo != 'o'){scan_error(lookforo); break;}
			else{
				int lookfort = cin.get();
				if(lookfort != 't'){scan_error(lookfort); break;}
				else{
					int lookforoo = cin.get();
					if(lookforoo != 'o'){scan_error(lookforoo); break;}
					//cin.unget();
					//cin.putback('o');
				}
				//cin.unget();
				//cin.putback('t');
			}
			//cin.unget();
			//cin.putback('o');
			peeker = T_goto; break;
		}
		case 'i': {
			int lookforf = cin.get();
			if(lookforf != 'f'){scan_error(lookforf); break;}
			//cin.unget();
			peeker = T_if; break;
		}
		case 'p': {
			int lookforr = cin.get();
			if(lookforr != 'r'){scan_error(lookforr); break;}
			else{
				int lookfori = cin.get();
				if(lookfori != 'i'){scan_error(lookfori); break;}
				else{
					int lookforn = cin.get();
					if(lookforn != 'n'){scan_error(lookforn); break;}
					else{
						int lookforte = cin.get();
						if(lookforte != 't'){scan_error(lookforte); break;}
						//cin.unget();
					}
					//cin.unget();
				}
				//cin.unget();
			}
			//cin.unget();
			peeker = T_print; break;
		}
		case 'L': peeker = T_label; break;
		case ':': peeker = T_colon; break;
		/*case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': getnum(); peeker = T_num; break;
		*/
		default:
		//to avoid overflow, we need something bigger than an INT
		long num = c - '0';
		long x = 10;
		if(c >= '0' && c <= '9'){
			peeker = T_num;
			//int n = cin.get();
			int n = cin.get();
			while(n >= '0' && n <= '9'){
				//n = cin.get();
				//if(n < '0' && n > '9'){
					//cin.unget();
					//break;
				//}
				//num += x * (n - '0');
				num = num*x + (n-'0');
				//x = x * 10;
				n = cin.get();
				if(num > 2147483647) scan_error(c); //2^31 - 1, max possible int
			}
			number = (int)num;
			//cin.unget();
		} else {scan_error(c);}
		break;				
	}
	cin.unget();
	canwepeek = true;
	return peeker;
	
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
	token_type t = c;
	if(c != next_token()){
		mismatch_error(c);
		canwepeek = false;
	}
	else{
		switch(t){
			case T_print:{
				for(int i=0; i<3;i++){ cin.get(); }
				cin.unget();
				break;
			}
			case T_goto:{
				//while(next == 'o' || next == 't' || 'g'){
				//	next = cin.get();
				//}
				for(int i=0; i<2;i++){ cin.get(); }
				cin.unget();
				break;
			}
			case T_if:{
				for(int i=0;i<2;i++){cin.get();}
				cin.unget();
				break;
			}
			case T_num:{
				//delete all digits
				int next = cin.get();
				while(next >= '0' && next <= '9'){
					next = cin.get();
				}
				cin.unget();
				break;
			}
			default:
				getchar();
		}
		canwepeek = false;
	}
}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	line = 1;
	canwepeek = false;
}

int scanner_t::get_line()
{
	//this function should return the current line number
	return line;
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
	void statement();
	void statements();
	void label();
	void jump();
	void jumpagain(std::string);
	void assign();
	void print();
	std::string expr();
	std::string exprprime(std::string);
	std::string exprprimeprime(std::string);	
	

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
	fprintf(stderr,"#include <stdlib.h> \n#include <stdio.h> \n#include <math.h>\nint main(){ \n \tint m[101]; \n");

	Start();

	fprintf(stderr,"\treturn 0; \n } \n");
}


//NEED FIRST AND FOLLOWS SETS
//Here is an example
void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);
	token_type t = scanner.next_token();

	switch( t ) 
	{
		case T_label:
		case T_goto:
		case T_m:
		case T_print:
			statements();
			break;
		//case T_num:
		//case T_openparen:
			//statements();
			//eat_token(???);
			//statementsprime();
		//	break;
	
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


void parser_t::statements()
{
	parsetree.push(NT_STATEMENTS);
	bool wedonehere = true;
	while(wedonehere){
		token_type t = scanner.next_token();
		switch(t){
			case T_goto:
			case T_label:
			case T_print:
			case T_m:
				statement();
				break;
			case T_eof: wedonehere = false; break;
			default:
				syntax_error(NT_STATEMENTS);
				break;
		}
	}
	parsetree.pop();
}

void parser_t::statement(){
	parsetree.push(NT_STATEMENT);
	token_type t = scanner.next_token();
	switch(t){
		case T_label: label(); break;
		case T_goto: jump(); break;
		case T_m: assign(); break;
		case T_print: print(); break;
		default:
			syntax_error(NT_STATEMENT);
	}
	fprintf(stderr, "\n");
	parsetree.pop();
}

void parser_t::label(){
	parsetree.push(NT_LABEL);
	//num get moved up here 
	//char* num = scanner.getnum();
	eat_token(T_label);
	//char* num = scanner.getnum();
	eat_token(T_num);
	eat_token(T_colon);
	char* num = scanner.getnum();
	fprintf(stderr,"\tlabel%s:", num);

	parsetree.pop();
}

void parser_t::jump(){
	parsetree.push(NT_JUMP);
	eat_token(T_goto);
	//char* num = scanner.getnum();
	eat_token(T_num);
	char* num = scanner.getnum();
	jumpagain(num);
	parsetree.pop();
}

void parser_t::jumpagain(std::string num){
	parsetree.push(NT_JUMPAGAIN);
	std::string jumpu;
	token_type t = scanner.next_token();
	
	switch(t){
		case T_if: eat_token(T_if); jumpu = expr(); fprintf(stderr, "\tif ( %s ) goto label%s;", jumpu.c_str(), num.c_str());break;
		case T_goto:
		case T_equals:
		case T_label:
		case T_print:
		case T_eof: parsetree.drawepsilon(); fprintf(stderr,"\tgoto label%s;", num.c_str());break;
		default:
			syntax_error(NT_JUMPAGAIN);
	}
	parsetree.pop();
}

void parser_t::assign(){
	parsetree.push(NT_ASSIGN);
	//eat_token(T_opensquare);
	//cout << token_to_string(scanner.next_token());
	eat_token(T_m);
	eat_token(T_opensquare);
	std::string first = expr();
	eat_token(T_closesquare);
	eat_token(T_equals);
	std::string second = expr();

	fprintf(stderr, "\tm[%s] = %s;", first.c_str(), second.c_str());

	parsetree.pop();
}

void parser_t::print(){
	parsetree.push(NT_PRINT);
	eat_token(T_print);
	std::string percent = "%d";
	fprintf(stderr, "\tprintf(\"%s\",%s);", percent.c_str(), expr().c_str());
	parsetree.pop();	
}

std::string parser_t::expr(){
	
	parsetree.push(NT_Expression);
	std::string first("");
	std::string res("");
	std::ostringstream s;

	token_type t = scanner.next_token();
	switch(t){
		case T_openparen: eat_token(T_openparen); first = expr(); eat_token(T_closeparen); s << "(" << first << ")"; res = s.str(); break;
		case T_m: eat_token(T_m); eat_token(T_opensquare); first = expr(); eat_token(T_closesquare); s << "m[" << first << "]"; res = s.str(); break;
		case T_num: res = scanner.getnum(); eat_token(T_num); break;
		default:
			syntax_error(NT_Expression);
			break;
	}
	res = exprprime(res);
	parsetree.pop();
	return res;
}

std::string parser_t::exprprime(std::string first){
	std::string res("");
	std::string second("");
	std::ostringstream s;
	parsetree.push(NT_EXPRPRIME);
	token_type t = scanner.next_token();
	switch(t){
		case T_plus: eat_token(T_plus); second = expr(); s << first << "+" << second; res = s.str(); break;
		case T_minus: eat_token(T_minus); second = expr(); s << first << " - " << second; res = s.str(); break;
		case T_times:
		case T_divide:
		case T_power: res = exprprimeprime(first); break;
		default:
			res = first;
			parsetree.drawepsilon();
			
	}
	parsetree.pop();
	string si(res);
	return res;
}

std::string parser_t::exprprimeprime(std::string first){

	std::string res;
	std::string second;
	std::ostringstream s;
	parsetree.push(NT_EXPRPRIMEPRIME);
	token_type t = scanner.next_token();
	switch(t){
		case T_times: eat_token(T_times); second = expr(); s << first << " * " << second; res = s.str(); break;
		case T_divide: eat_token(T_divide); second = expr(); s << first << " / " << second; res = s.str(); break;	
		case T_power: eat_token(T_power); second = expr();  s << "pow(" << first << ", " << second << ")"; res = s.str();break;
		default:
			syntax_error(NT_EXPRPRIMEPRIME);	
	}
	parsetree.pop();

	return res;
}


/*** Main ***********************************************/

int main()
{
	//test scanner--odd case of 3*2 returns num * instead of num * num, other cases work
	
	/*scanner_t s;
	token_type tok = s.next_token();
	while(tok != T_eof){
		s.eat_token(tok);
		cout << token_to_string(tok) << endl;
		tok = s.next_token();
	}
	//cout << token_to_string(tok) << endl;
	*/
	parser_t parser;
	parser.parse();
	return 0;
}
