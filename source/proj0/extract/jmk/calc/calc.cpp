#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <fstream>
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
	NT_ExpressionP,
	NT_Statements,
	NT_Statement,
	NT_MoreStatement,
	NT_Label,
	NT_Jump,
	NT_Assignment,
	NT_Print,
	NT_Term,
	NT_TermP,
	NT_Factor,
	NT_FactorP,
	NT_Exp

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
		  case NT_ExpressionP: strncpy(buffer,"ExpressionP",MAX_SYMBOL_NAME_SIZE); break;

		  //WRITEME: add the other nonterminals you need here
    	  case NT_Statements: strncpy(buffer,"Statments",MAX_SYMBOL_NAME_SIZE); break; 
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_MoreStatement: strncpy(buffer,"More_Statement",MAX_SYMBOL_NAME_SIZE); break;

		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_TermP: strncpy(buffer,"TermP",MAX_SYMBOL_NAME_SIZE); break;

		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
  		  case NT_FactorP: strncpy(buffer,"FactorP",MAX_SYMBOL_NAME_SIZE); break;

		  case NT_Exp: strncpy(buffer,"Exp",MAX_SYMBOL_NAME_SIZE); break;

		  default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
		}
	return buffer;
}

/*** Scanner Class ***********************************************/

class scanner_t {
  public:
  	int lineNumber;
	//eats the next token and prints an error if it is not of type c
	void eat_token(token_type c);

	//peeks at the lookahead token
	token_type next_token();

	//return line number for errors
	int get_line();

	//
	double concatenate(double x, int y);

	//constructor 
	scanner_t();

	//get num for c stuff
	double get_num();


  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  
  	int Numlength;
  	double finalNum;


	void set_num(double x);

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
	//error message and exit for number too big or smalll
	void range_error(string x);

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	token_type bogo_token;

};

void scanner_t::set_num(double x){
	finalNum = x;	
}

double scanner_t::get_num(){
	return finalNum;
}

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
	double eat;
	string numba;
	double Num;

	char token = cin.peek();

	// gets rid of whitespace
	while(token == ' '){
		eat = getchar();

		token = cin.peek();

	}
	//gets rid of new line also increments the new line
		while(token == '\n'){
		eat = getchar();

		token = cin.peek();
		lineNumber++;
	}

		if(isdigit(token)){

			while(isdigit(cin.peek())){
				numba += cin.peek();
				// cout<<"This is numba: "<<numba<<endl;
				eat = getchar();

				Num = atoi(numba.c_str());
				if(Num >= 2147483648 || Num < 0){
						range_error(numba);
					}

			}//while
			Numlength = numba.length()-1;
			for(int i = Numlength; i >= 0; i--){
				// cout<<"putting back"<<numba[i]<<endl;
				cin.putback(numba[i]);
			}
			set_num(Num);
			token = T_num;
		}//if

	// 	if(isdigit(token)){
	// 		numba = cin.peek() - '0';
	// 		cout<< "numba = "<< numba <<endl;
	// 		if(numba == 0){
	// 			cout<<"PAsed equal test mutha fucka"<<endl;
	// 			eat = getchar();
	// 			set_num(numba);
	// 		}
	// 		else{

	// 	// token = T_num;
	// 			while(isdigit(cin.peek())){

	// 				numba = cin.peek() - '0';
	// 				Num = concatenate(Num, numba);
	// 				//cout<<"this is the final num: "<<Num <<endl;
	// 				eat = getchar();
	// 				set_num(Num);
	// 				if(Num >= 2147483648 || Num < 0){
	// 					range_error(Num);
	// 				}

	// 			}//while
	// 		}//else
	// 	cin.putback(eat);
	// 	token = T_num;
	// }

//cout<<"token = "<< token <<endl;
	switch(token){
		case EOF: return T_eof;			// 0: end of file
		case T_num: return T_num;					// 1: numbers
		case '+'  : return T_plus;				// 2: +
		case '-'  : eat = getchar();
					if(isdigit(cin.peek())) {
						range_error("-1");}
					else{
						cin.putback(eat);	
						return T_minus;}				// 3: -
		case T_power : return T_power;			// 6: **
		case '*'  : eat = getchar();
					if(cin.peek() == '*'){
						cin.putback('*');

						return T_power;}
					else
						cin.putback('*');
						return  T_times;				// 4: *
		case '/'  : return T_divide;			// 5: /
		case '='  : return T_equals;			// 7: =
		case '('  : return T_openparen;		// 8: (
		case ')'  : return T_closeparen; 	// 9: )
		case '['  : return T_opensquare;	// 10: [
		case ']'  : return T_closesquare;	// 11: ]
		case 'm'  : return T_m;						// 12: m
		case 'g': eat = getchar();
					 if(cin.peek() == 'o'){
					 	eat = getchar();
					 	if(cin.peek() == 't'){
					 		eat = getchar();
					 		if(cin.peek()== 'o'){
					 			eat = getchar();
					 			// cout<<"Ahoy there mateys"<<endl;
					 			cin.putback('o');
					 			cin.putback('t');
					 			cin.putback('o');
					 			cin.putback('g');
					 			//cout<<"this is my peek"<< char(cin.peek())<< endl;
					 			return T_goto;
					 		}
					 		else
					 			 scan_error(cin.peek());
					 	}
					 	else
					 		 scan_error(cin.peek());
					 }
					 else{
					 	 //cin.putback(eat);
					 	 scan_error(cin.peek());
					 	 }				// 13: goto
		case 'i'  : eat = getchar();
					 if(cin.peek() == 'f'){
					 	eat = getchar();
					 	cin.putback('f');
					 	cin.putback('i');
					 	return T_if;
					 	}
					 else
					 	 scan_error(cin.peek());					// 14: if
		case 'p': eat = getchar();
					  if(cin.peek() == 'r'){
					  	eat = getchar();
					  	if(cin.peek() == 'i'){
					  		eat = getchar();
					  		if(cin.peek()== 'n'){
					  			eat = getchar();
					  			if(cin.peek() == 't'){
					  				eat = getchar();
					 				cin.putback('t');
					 				cin.putback('n');
					 				cin.putback('i');
					 				cin.putback('r');
					 				cin.putback('p');
					  				return T_print;
					  			}
					  			else
					  				 scan_error(cin.peek());
					  		}
					  		else
					  			 scan_error(cin.peek());					 
					  	}
					  	else 
					  		 scan_error(cin.peek());
					  }
					  else
					  	 scan_error(cin.peek());
					  				// 15: print
		case 'L'   : return T_label;				// 16: L
		case ':'   : return T_colon;
		default : scan_error(token);		
 
	}

	// if ( bogo_token!=T_plus && bogo_token!=T_eof ) return T_plus;
	// else return T_eof;
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eat_token
	// if ( rand()%10 < 8 ) bogo_token = T_plus;
	// else bogo_token = T_eof;

//	cout<< "this is c:"<<c<< " This ispeek:"<<next_token() << endl;

	if (next_token()== c){
		switch(c){

			case T_goto:
				getchar();
				getchar();
				getchar();
				//getchar();
				break;

			case T_print:
				cerr<<"printf(\"%f \",";
				getchar();
				getchar();
				getchar();
				getchar();
				break;

			case T_if:
				cerr<<" if( ";
				getchar();
				break;

			case T_power:
				getchar();
				break;				

			case T_num:
				for(int i = Numlength-1; i >= 0; i--){
					getchar();
				}
				break;

		}
		getchar();
		}
	else

		mismatch_error(c);

}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	lineNumber = 1;
}

int scanner_t::get_line()
{
	return lineNumber;
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

void scanner_t::range_error(string x)
{
	//printf("range error: %lf is outside of alloted range -- line %d\n", x, get_line());
	cout<<"range error: "<<x<< " is outside of alloted range -- line "<<get_line()<<endl;
	exit(1);

}
//concatenation function for digits, i took this  from internet user *Mooing Duck*

double scanner_t::concatenate(double x, int y) {
    double pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        
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
	void eat_token(token_type t);
	void syntax_error(nonterm_type);

	void Start();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)
	void Statements();
	void Statement();
	void MoreStatement();
	void Label();
	void Jump();
	void Assignment();
	void Print();
	void Expression();
	void ExpressionP();
	void Term();
	void TermP();
	void Factor();
	void FactorP();
	void Exp();

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

void parser_t::Start(){
	
	parsetree.push(NT_Start);
	cerr <<"#include<stdio.h>"<<endl;
	cerr <<"#include<math.h>\n"<<endl;
	cerr<<"main()\n{\n"<<endl;
	cerr<<"int m[101];\n"<<endl;



	Statements();

	parsetree.pop();
	cerr<<"}"<<endl;

}

void parser_t::Statements(){
	parsetree.push(NT_Statements);


	Statement();
	MoreStatement();

	parsetree.pop();
}

void parser_t::MoreStatement(){
	parsetree.push(NT_MoreStatement);

	switch(scanner.next_token()){

		case T_label:
		case T_m:
		case T_goto:
		case T_print: 
			Statement();
			MoreStatement();
			break;


		case T_eof:
			parsetree.drawepsilon();
			break;

		default:
			syntax_error(NT_MoreStatement);
			break;

	}

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
			eat_token(T_m);
			Assignment();
			break;

		case T_label:
			Label();
			//eat_token(T_label);

			scanner.next_token();
			break;

		case T_goto:
			//eat_token(T_goto);
			Jump();
			scanner.next_token();
			break;

//COME BACK TO FIX WHAT PRINT prints out ~!~!~!~!~!~!~!~!~!~!~!~!~!~~!!~~!!~!
		case T_print:
			//eat_token(T_print);
			Print();
			scanner.next_token();
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Statement);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	//scanner.next_token();
	parsetree.pop();
}

void parser_t::Label(){

	parsetree.push(NT_Jump);
	eat_token(T_label);
	cerr<<"L";
	eat_token(T_num);
	cerr<<scanner.get_num();
	cerr<<":\n";
	eat_token(T_colon);

	parsetree.pop();

}


void parser_t::Jump(){
	int temp;
	parsetree.push(NT_Jump);
	eat_token(T_goto);
	//cerr<<"goto ";
	scanner.next_token();
	temp = 	scanner.get_num();
	eat_token(T_num);

	scanner.next_token();
	if(scanner.next_token()== T_if){
		eat_token(T_if);
	//	cerr<<"if( ";
		//scanner.next_token();
		Expression();
		cerr<<" );\n";
		cerr<<"goto L";
		cerr<<temp;
		cerr<<"; \n";
		}
	else{
		cerr<<"goto L";
		cerr<<temp;
		cerr<<"; \n";
		}
	parsetree.pop();
}

void parser_t::Print(){

	parsetree.push(NT_Print);
	scanner.next_token();
	eat_token(T_print);
//	scanner.next_token();
	
	Expression(); 
	cerr<<");\n"<<endl;
	parsetree.pop();

}

void parser_t::Assignment(){

	parsetree.push(NT_Assignment);

	switch( scanner.next_token() ) 
	{
		case T_opensquare:
			cerr<<"m";
			eat_token(T_opensquare);
			cerr<<"[ ";
			Expression();
			scanner.next_token();
			eat_token(T_closesquare);
			cerr<<" ] ";
			scanner.next_token();
			eat_token(T_equals);
			cerr<<" = ";
			Expression();
			cerr<<"; \n";
			scanner.next_token();
			break;

		// case T_eof:
		// 	parsetree.drawepsilon();
		// 	break;
		default:
			syntax_error(NT_Assignment);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

void parser_t::Expression(){

	parsetree.push(NT_Expression);

	scanner.next_token();
	Term();
	ExpressionP();

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

void parser_t::Term(){

	parsetree.push(NT_Term);

	scanner.next_token();
	Factor();
	TermP();

	parsetree.pop();
}

void parser_t::ExpressionP(){

	parsetree.push(NT_ExpressionP);

	switch( scanner.next_token() ) 
	{
		case T_plus:
			eat_token(T_plus);
			cerr<<" + ";
			Term();
			ExpressionP();
			break;

		case T_minus:
			eat_token(T_minus);
			cerr<< " - ";
			Term();
			ExpressionP();
			break;

		case T_eof:
			parsetree.drawepsilon();
			break;

		default:
			parsetree.drawepsilon();
			// syntax_error(NT_ExpressionP);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

void parser_t::Factor(){

	parsetree.push(NT_Factor);

	scanner.next_token();
	Exp();
	FactorP();

	parsetree.pop();
}

void parser_t::TermP(){

	parsetree.push(NT_TermP);

	switch( scanner.next_token() ) 
	{
		case T_times:
			eat_token(T_times);
			cerr<<" * ";
			Factor();
			TermP();
			break;

		case T_divide:
			eat_token(T_divide);
			cerr<<" / ";
			Factor();
			TermP();
			break;

		case T_eof:
			parsetree.drawepsilon();
			break;

		default:
			parsetree.drawepsilon();
			// syntax_error(NT_ExpressionP);
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

void parser_t::FactorP(){
	int temp = scanner.get_num();
	parsetree.push(NT_FactorP);
	switch(scanner.next_token()){
		case T_power:

			eat_token(T_power);
			Exp();

			FactorP();
			break;

		case T_eof:
			parsetree.drawepsilon();
			break;

		default:
			parsetree.drawepsilon();
			// syntax_error(NT_ExpressionP);
			break;

	}
	parsetree.pop();

}

void parser_t::Exp(){

	parsetree.push(NT_Exp);

	switch (scanner.next_token()){

		case T_openparen:
			eat_token(T_openparen);
			cerr<<" ( ";
			Expression();
			//scanner.next_token();
			eat_token(T_closeparen);
			cerr<< " ) ";
			break;

		case T_m:
			eat_token(T_m);
			cerr<<"m";
			scanner.next_token();
			eat_token(T_opensquare);
			cerr<<"[ ";
			Expression();
			//scanner.next_token();
			eat_token(T_closesquare);
			cerr<<" ]";
			break;

		case T_num:
			eat_token(T_num);
			cerr<<scanner.get_num();
			break;

		case T_eof:
			parsetree.drawepsilon();
			break;

		default:
			syntax_error(NT_Exp);
			break;
	}


	parsetree.pop();
}



/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();

	 scanner_t scanner;
	// while(!cin.eof()){
	 // 	scanner.eat_token(scanner.next_token());
		// cout<< "This is your token:  " <<token_to_string(scanner.next_token()) << endl;
		// if(scanner.next_token() == T_num)
		//  	cout<<"num = "<< scanner.get_num() << endl;
		//  cout<< "This is your token2:  " <<token_to_string(scanner.next_token()) << endl;
		//  if(scanner.next_token() == T_num)
		//  	cout<<"num = "<< scanner.get_num() << endl;
		// scanner.eat_token(scanner.next_token());


	//scanner.eat_token(scanner.next_token());

	//}
	return 0;
}
