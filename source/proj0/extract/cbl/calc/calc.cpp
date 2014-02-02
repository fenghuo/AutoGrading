#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sstream>
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
	NT_Statements,
	NT_Statement,
	NT_StatementsP,
	NT_Print,
	NT_Jump,
	NT_Label,
	NT_Assignment,
	NT_Expression,
	NT_ExpressionP,
	NT_Factor,
	NT_FactorP,
	NT_Term,
	NT_TermP,
	NT_Alpha

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
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;

		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_ExpressionP: strncpy(buffer,"ExpressionP",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_FactorP: strncpy(buffer,"FactorP",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_TermP: strncpy(buffer,"TermP",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Alpha: strncpy(buffer,"Alpha",MAX_SYMBOL_NAME_SIZE); break;	
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

	int fullNumber;
	string fullNumberString;



  private:
	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

	void range_error(double num);


	int lineNumber;

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.


};

token_type scanner_t::next_token()
{

	double number = 0;
	double finalNumber = 0;
	char holder;
	 char token = cin.peek();
	while(token == ' '){
	 	//cerr<<' ';
	 	getchar();
	 	token = cin.peek();
	 }

	 while(token == '\n'){
	 	lineNumber++;
	 	getchar();
	 	token = cin.peek();
	 }
	 	
	if (isdigit(token)){	
		return T_num;	
	}
			
	switch(token){
		case EOF     :  return T_eof; 

		case '+'       :  return T_plus;

		case '-'       :  return T_minus;

		case '*'       :  
				getchar(); 
				if (cin.peek() == '*') {
					cin.putback('*');
					return T_power;
				}
					
				else{
					cin.putback('*');					
					return T_times;
				}
				 

		case '/'       :  return T_divide;

		case '='       :  return T_equals;

		case '('	   :  return T_openparen;

		case ')'       :  return T_closeparen;

		case '['       :  return T_opensquare;

		case ']'       :  return T_closesquare;

		case 'm'       :  return T_m;

		case 'g'       :
				getchar();
				if(cin.peek() == 'o'){
					token + getchar();			 				
					if(cin.peek() == 't'){
						token + getchar();
						if(cin.peek() == 'o'){
							getchar(); 
							if(cin.peek() == ' '){
							getchar(); 
							cin.putback(' ');
							cin.putback('o');
							cin.putback('t');
							cin.putback('o');
							cin.putback('g');
							return T_goto;
							}	
							else 
								scan_error(token); break;
							}
						else 
							scan_error(token); break;
					}
					else 
						scan_error(token); break;
				}
				else 
					scan_error(token); break;
		case 'i'       :
				getchar();
				if(cin.peek() == 'f'){
					getchar();	
					cin.putback('f');
					cin.putback('i');
					return T_if;		 	
				}
				else
					scan_error(token); break;

		case 'p'       :
				getchar();
				if(cin.peek() == 'r'){
					token + getchar();			 				
					if(cin.peek() == 'i'){
						token + getchar();
						if(cin.peek() == 'n'){
							token + getchar(); 	
							if(cin.peek() == 't'){
								getchar(); 
								if(cin.peek() == ' '){
								getchar(); 
								cin.putback(' ');
								cin.putback('t');
								cin.putback('n');
								cin.putback('i');
								cin.putback('r');
								cin.putback('p');
								return T_print;		
							}
							else 
								scan_error(token); break;		
							}
							else 
								scan_error(token); break;
						}
						else 
							scan_error(token); break;
					}
					else 
						scan_error(token); break;
					}
				else 
					scan_error(token); break;
			
		case 'L'       :  return T_label;

		case ':'       :  return T_colon;	

		default        :  scan_error(token); break;
	}

}


void scanner_t::eat_token(token_type c)
{
	double number = 0;
	double finalNumber = 0;
	char holder;
	if(c == next_token()){
		if(c == T_print){
			getchar();
			getchar();
			getchar();
			getchar();
			getchar();
		}
		else if(c == T_goto){
			getchar();
			getchar();
			getchar();
			getchar();
		}
		else if(c == T_if){
			getchar();
			getchar();
		}
		else if(c == T_power){
			getchar();
			getchar();
		}
		else if(c == T_num){
					string tempS;
		int finalNum;
		 while(isdigit(cin.peek())){
		 	number = cin.peek() - '0';
		 	stringstream ss;
			string s;
			char c = cin.peek();
			ss << c;
			ss >> s;
		 	tempS +=s;
		 	getchar();
			}
			fullNumberString = tempS;
			fullNumber = atoi(tempS.c_str());
			if(fullNumber > 2147483647 || fullNumber < 0){
		 		range_error(finalNumber);
		 	}
			fullNumber = atoi(tempS.c_str());
		}
		else
			getchar();
	}
		
	else
		mismatch_error(c);

}

scanner_t::scanner_t(){
	lineNumber = 1;
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

void scanner_t::range_error(double num){
	printf("number out of range '%lf' -line %d\n", num, get_line());
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

namespace parserT{

class parser_t {
  private:
	scanner_t scanner;
	parsetree_t parsetree;
	void eat_token(token_type t);
	void syntax_error(nonterm_type);

	void Start();
	void parseStatements();
	void parseStatement();
	void parseStatementsP();
	void parseLabel();
	void parseJump();
	void parseAssignment();
	void parsePrint();
	void parseExpression();
	void parseExpressionPrime();
	void parseTerm();
	void parseTermPrime();
	void parseFactor();
	void parseFactorPrime();
	void parseAlpha();
	bool printFlag;
	int storedNum;
	bool holding;
	bool holdingM;
	int parenCount;
	string exprHolder;
	void exponentParens();
	string storedNumS;


  public:	
	void parse();
};

void parser_t::exponentParens(){
		if(holding)
			cerr<<storedNum;

		for(int i = 0; i<parenCount; i++){
				cerr<<")";
		}
		parenCount = 0;
		holding = false;
}

void parser_t::eat_token(token_type t)
{
	parsetree.push(t);
	scanner.eat_token(t);
	parsetree.pop();
}


void parser_t::syntax_error(nonterm_type nt)
{
	printf("syntax error: found %s in parsing %s - line %d\n",
		token_to_string( scanner.next_token()),
		nonterm_to_string(nt),
		scanner.get_line() ); 
	exit(1); 
}

void parser_t::parse()
{
	Start();
}

void parser_t::Start()
{
	parsetree.push(NT_Start);
	parseStatements();
	parsetree.pop();
}

void parser_t::parseStatements(){
	parsetree.push(NT_Statements);
	parseStatement();
	parseStatementsP();
	parsetree.pop();
}

void parser_t::parseStatementsP(){
	parsetree.push(NT_StatementsP);
	if(scanner.next_token() == T_label || scanner.next_token() == T_goto ||scanner.next_token() == T_m ||scanner.next_token() == T_print){
		parseStatement();
		parseStatementsP();
	}
	else if(scanner.next_token() == T_eof){		
		parsetree.drawepsilon();
	}
	else{
		syntax_error(NT_StatementsP);
	}
	parsetree.pop();
}

void parser_t::parseStatement(){
	parsetree.push(NT_Statement);
		switch( scanner.next_token() ) 
	{
		case T_label:			
			parseLabel();
			break;
		case T_goto:
			parseJump();
			break;
		case T_m:
			parseAssignment();  
			break;
		case T_print:
			parsePrint();
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
void parser_t::parseLabel(){
	parsetree.push(NT_Label);
	eat_token(T_label);
	cerr<<"L";
	eat_token(T_num);
	cerr<<scanner.fullNumber;
	eat_token(T_colon);
	cerr<<":"<<endl;
	parsetree.pop();
}

void parser_t::parseAssignment(){
	parsetree.push(NT_Assignment);
	switch( scanner.next_token() ) 
	{
		case T_m:
			eat_token(T_m);
			cerr<<"m";
			scanner.next_token();
			eat_token(T_opensquare);
			cerr<<"[";
			parseExpression();
			scanner.next_token();
			eat_token(T_closesquare);
			cerr<<storedNum;
			cerr<<"]";
			holding = false;
			scanner.next_token();
			eat_token(T_equals);
			cerr<<"=";
			parseExpression();
			if (holding){
				cerr<<storedNum;
			}

				for(int i = 0; i<parenCount; i++){
					cerr<<")";
				}
			
			cerr<<";"<<endl;
			parenCount = 0;
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Assignment);
			break;
	}
	parsetree.pop();
}

void parser_t::parsePrint(){
	parsetree.push(NT_Print);
	eat_token(T_print);
	cerr<<"printf(\"%d\",";
	parseExpression();
	cerr<<");"<<endl;
	parsetree.pop();
}
	
void parser_t::parseJump(){
	parsetree.push(NT_Jump);
	switch( scanner.next_token() ) 
	{
		int labelNum ;
		case T_goto:
			eat_token(T_goto);
			scanner.next_token(); 
			eat_token(T_num);	
			labelNum = scanner.fullNumber;	
			if(scanner.next_token() == T_if){ 
				eat_token(T_if);
				cerr<<"if(";
				parseExpression();
				cerr<<")"<<endl;
				cerr<<"goto ";
				cerr<<"L"<<labelNum<<";"<<endl;
			}
			else{
				cerr<<"goto ";
				cerr<<"L"<<labelNum<<";"<<endl;
			}
				
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;			
		default:
			syntax_error(NT_Jump);
			break;
	}
	parsetree.pop();
}	

void parser_t::parseExpression(){
	parsetree.push(NT_Expression);
	parseTerm();
	parseExpressionPrime();
	parsetree.pop();
	}

void parser_t::parseExpressionPrime(){
	parsetree.push(NT_ExpressionP);
	switch( scanner.next_token() ) 
	{

		case T_plus:
			eat_token(T_plus);	
			exponentParens();
			cerr<<"+";
			parseTerm();
			parseExpressionPrime();	
			break;
		case T_minus:
			eat_token(T_minus);
			exponentParens();
			cerr<<"-";
			parseTerm();
			parseExpressionPrime();	
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}

void parser_t::parseTerm(){
	parsetree.push(NT_Term);
	parseFactor();
	parseTermPrime();
	parsetree.pop();
}

void parser_t::parseTermPrime(){
	parsetree.push(NT_TermP);
	switch( scanner.next_token() ) 
	{
		case T_times:
			eat_token(T_times);	
			exponentParens();
			cerr<<"*";
			parseFactor();
			parseTermPrime();	
			break;
		case T_divide:
			eat_token(T_divide);
			exponentParens();
			cerr<<"/";
			parseFactor();
			parseTermPrime();	
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}

void parser_t::parseFactor(){
	parsetree.push(NT_Factor);
	parseAlpha();
	parseFactorPrime();
	parsetree.pop();
}

void parser_t::parseFactorPrime(){
	parsetree.push(NT_FactorP);
	switch(scanner.next_token()){
		case T_power:
			eat_token(T_power);
				cerr<<"pow("<<storedNum<<",";
			holding = false;
			holdingM = false;
			parenCount++;
			parseAlpha();
			parseFactorPrime();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}
	parsetree.pop();
}

void parser_t::parseAlpha(){
	parsetree.push(NT_Alpha);
	switch( scanner.next_token() ) 
	{
		case T_openparen:
			eat_token(T_openparen);	
			cerr<<"(";
			parseExpression();
			scanner.next_token();
			eat_token(T_closeparen);	
			cerr<<storedNum;
			holding = false;
			cerr<<")";
			break;
		case T_m:
			cerr<<"m";
			eat_token(T_m);
			scanner.next_token();
			eat_token(T_opensquare);
			cerr<<"[";	
			parseExpression();
			scanner.next_token();
			eat_token(T_closesquare);
			cerr<<storedNum;
			holding = false;
			cerr<<"]";
			break;
		case T_num:
			eat_token(T_num);
			storedNum = scanner.fullNumber;
			storedNumS = scanner.fullNumberString;
			holding = true;
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Alpha);
			break;
	}
	parsetree.pop();
}
}


/*** Main ***********************************************/

int main()
{

	cerr<<"#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>"<<endl;
	cerr<<"int main(){"<<endl;
	cerr<<"int m[101];"<<endl;
	parserT::parser_t parser;
	parser.parse();

	cerr<< "return 0;\n }" <<endl;
			
	return 0;
}
