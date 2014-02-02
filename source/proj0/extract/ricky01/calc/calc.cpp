#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <string>


using namespace std;
//Ricky Martinez
//coding buddy Eric Yixin Wang


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

typedef enum {
	epsilon = 100,
	NT_Start,
	NT_Expression,

	NT_Statements,
	NT_More,
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_JumpExp,
	NT_Assignment,
	NT_Print,
	NT_AD,
	NT_Term,
	NT_MD,
	NT_Pow,
	NT_Exponent,
	NT_Fact
	
} nonterm_type;


char* nonterm_to_string(nonterm_type nt)
{
	static char buffer[MAX_SYMBOL_NAME_SIZE];
	switch( nt ) {
		  case epsilon: strncpy(buffer,"e",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Start: strncpy(buffer,"Start",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;

		  case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_More: strncpy(buffer,"More",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_JumpExp: strncpy(buffer,"JumpExp",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
          case NT_AD: strncpy(buffer,"AD",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_MD: strncpy(buffer,"MD",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Pow: strncpy(buffer,"Pow",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Exponent: strncpy(buffer,"Exponent",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Fact: strncpy(buffer,"Fact",MAX_SYMBOL_NAME_SIZE); break;

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
	
	string get_goto_num();
	//string get_power_num();
	string get_prev();////////////////maybe not useful
	int get_int_value();
	string get_current();

  private:

 
	token_type scan();

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
	void overflow(string s);

	int lineNum;

	bool peeked;
	string read_newline;
	string goto_num;
	bool goto_;
	bool power;
	//string power_num;
	token_type the_next_token;
	string current_num;
	string prev_num;
	//int power_number;
	int prev_int;
	

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	token_type bogo_token;

};

string scanner_t::get_current(){return current_num;}

string scanner_t::get_goto_num(){
	string temp=goto_num;
	goto_num="";
	return temp;
}
/*
string scanner_t::get_power_num(){
	string temp=power_num;
	power_num="";
	return temp;
}*/


//not fully funtional, 
void scanner_t::overflow(string s){
	int length=s.length();
	if (length<10)
		return;
	if (length>=11){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}

	if(s[0]>2+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[0]<=1+47)
		return;	



	if(s[1]>48){
		cout<<s[1]<<endl;
	exit(1);
	}
	if(s[1]==0+47)
		return;	

	if(s[2]>4+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[2]<=3+47)
		return;	

	if(s[3]>7+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[3]<=6+47)
		return;	

	if(s[4]>4+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[4]<=3+47)
		return;	

	if(s[5]>8+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[5]<=7+47)
		return;	


	if(s[6]>3+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[6]<=2+47)
		return;	


	if(s[7]>6+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[7]<=5+47)
		return;	

	if(s[8]>4+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[8]<=3+47)
		return;	

	if(s[9]>8+47){
		printf("overflow occured in  -line %d\n", get_line());
	exit(1);
	}
	if(s[9]<=7+47)
		return;	


//2147 48 3648 was the exact imput which is one over the limit, overflow

	printf("overflow occured in  -line %d\n", get_line());
	exit(1);

//2147 48 3648


}

string scanner_t::get_prev(){return prev_num;}

int scanner_t::get_int_value(){return prev_int;}




token_type scanner_t::scan(){
	char c=getchar();


	if(c==EOF)
		return T_eof;
	switch(c){
		case (' '): scan();break;
		case ('\n'): cerr<<read_newline;read_newline=";\n\t";lineNum++;scan();break;
		case ('\t'): scan();break;
		case ('\b'): scan();break;	
		case ('\r'): scan();break;					
		case ('0'):	
		case ('1'): 
		case ('2'):	
		case ('3'):	
		case ('4'):	
		case ('5'):	
		case ('6'):	
		case ('7'):	
		case ('8'):	
		case ('9'):{
			prev_num=current_num;
			current_num=c;
			prev_int=c-47;
			char p=cin.peek();
			while(p<58&&p>47){//char values for 0-9
				p=getchar();
				current_num+=p;
				prev_int*=10;
				prev_int+=(p-47);
				p=cin.peek();//peeking at stream to see if the number has finished
				
			}
			//overflow(current_num);//unfinished
			if(goto_==true)
			goto_num+=current_num;
			else if(power==true){		
				cerr<<current_num;
				}
			else{
			while(cin.peek()==' ')
				p=getchar();
			if(cin.peek()!='*')
				cerr<<current_num;}
			goto_=false;
			power=false;
			return T_num;
		}
		case ('+'):	return(T_plus);
		case ('-'):	return(T_minus);
		case ('*'):{
			char g=cin.peek();//checking to see if next char is * to determine if its power	
			if(g=='*'){
				g=getchar();
				power=true;
				return T_power;
			}
			return(T_times);}
		case ('/'):	return(T_divide);
		case ('='):	return(T_equals);
		case ('('):	return(T_openparen);
		case (')'):	return(T_closeparen);
		case ('['):	return(T_opensquare);
		case (']'):	return(T_closesquare);
		case ('m'):	return(T_m);
		case ('g'):	
			if(getchar()=='o')
				if(getchar()=='t')
					if(getchar()=='o'){
						read_newline=";\n\t";
						goto_=true;
						return(T_goto);}
			mismatch_error(T_goto);
			break;////////
		case ('i'):	
			if(getchar()=='f'){
				read_newline=")\n\t\t";
				return(T_if);}
 			mismatch_error(T_if);break;/////////
		case ('p'):	
			if(getchar()=='r')
				if(getchar()=='i')
						if(getchar()=='n')
							if(getchar()=='t'){
								read_newline=");\n\t";
								return(T_print);}
			mismatch_error(T_print);//////////
		case ('L'):	return(T_label);
		case (':'):	
			read_newline="\n\t";
			return(T_colon);
		
		default: scan_error(c);break;


	}
}


token_type scanner_t::next_token()
{

	
	if(peeked==true)
		return the_next_token;
	peeked=true;
	the_next_token= scan();
	
	return the_next_token;
}

void scanner_t::eat_token(token_type c)
{

	
	if(c!=the_next_token)
		mismatch_error(c);
	peeked=false;
	

}

scanner_t::scanner_t()
{

	lineNum=1;
    peeked=false;
	read_newline=";\n\t";
	goto_=false;
	goto_num="";
	power=false;
	//power_num="";
	current_num="";
	prev_num="";
	prev_int=0;
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
	return lineNum;
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



class parser_t {
  private:
	scanner_t scanner;
	parsetree_t parsetree;
	void eat_token(token_type t);
	void syntax_error(nonterm_type);

	void Start();
	void newStart();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)
	void Statements();
	void More();
	void Statement();
	void Label();
	void Jump();
	void JumpExp();
	void Assignment();
	void Print();
	void Expression();
	void AD();
	void Term();
	void MD();
	void Pow();
	void Exponent();
	void Fact();


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

//original start given to us, used to debug
void parser_t::newStart()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);

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

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree

	parsetree.pop();
}

void parser_t::Start(){
	parsetree.push(NT_Start);
	Statements();
	parsetree.pop();
}

void parser_t::Statements(){
		parsetree.push(NT_Statements);
		Statement();
		More();
		parsetree.pop();
}

void parser_t::More(){/////////////////////////////
	parsetree.push(NT_More);
	switch( scanner.next_token() ) 
	{
		case T_m:
			Statement();
			More();
			break;
		case T_label:
			Statement();
			More();		
			break;	
		case T_goto:
			Statement();
			More();
			break;
		case T_print:
			Statement();
			More();
			break;
		case T_eof:
			eat_token(scanner.next_token());
			break;	
		default:
			syntax_error(NT_Statement);
			break;
	}
	parsetree.pop();	

}

void parser_t::Statement(){
parsetree.push(NT_Statement);

	switch( scanner.next_token() ) 
	{
		case T_m:
			cerr<<"m";
			Assignment();
			break;
		case T_label:
			cerr<<"L";
			Label();		
			break;	
		case T_goto:
			//unfavorable to cerr goto here ubtil i can get more information
			Jump();
			break;		
		case T_print:
			cerr<<"printf(\"%d\\n\",";
			Print();
			//cerr<<");\n";
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
void parser_t::Label(){
	parsetree.push(NT_Label);
	eat_token(scanner.next_token());			
	if(scanner.next_token()!=T_num)
		syntax_error(NT_Label);
	eat_token(scanner.next_token());
	if(scanner.next_token()!=T_colon)
		syntax_error(NT_Label);
			cerr<<":";
	eat_token(scanner.next_token());		
	parsetree.pop();
}


void parser_t::Jump(){
	parsetree.push(NT_Jump);

	eat_token(scanner.next_token());
	if(scanner.next_token()!=T_num)
		syntax_error(NT_Jump);
	eat_token(scanner.next_token());
	char j=cin.peek();

	while(j==' '||j=='\r'){
		j=getchar();
		j=cin.peek();	
	}

	if(j=='\n'){
		cerr<<"goto L";
		cerr<<scanner.get_goto_num();
	}
	
	JumpExp();
	parsetree.pop();

}

void parser_t::JumpExp(){////////////////////////////
	parsetree.push(NT_JumpExp);
	switch(scanner.next_token()){
	case T_if:
		cerr<<"if(";
		eat_token(scanner.next_token());
		Expression();
		cerr<<"goto L"<<scanner.get_goto_num()<<";\n\t";
		break;	
	case T_label:

	case T_goto:

	case T_m:

	case T_print:

	case T_closesquare:

	case T_closeparen:
		parsetree.drawepsilon();
		break;
	case T_eof://////////////////////////////////////////////////////////////////
		parsetree.drawepsilon();
		break;
	default:
		syntax_error(NT_MD);
		break;


}
	parsetree.pop();
}

void parser_t::Assignment(){
	parsetree.push(NT_Assignment);


	
	eat_token(scanner.next_token());
	if(scanner.next_token()!=T_opensquare)
		syntax_error(NT_Assignment);
	cerr<<"[";
	eat_token(scanner.next_token());
	Expression();
	if(scanner.next_token()!=T_closesquare)
		syntax_error(NT_Assignment);
	cerr<<"]";
	eat_token(scanner.next_token());	
	if(scanner.next_token()!=T_equals)
		syntax_error(NT_Assignment);
	cerr<<"=";
	eat_token(scanner.next_token());		
	Expression();	

	parsetree.pop();

}

void parser_t::Print(){//
	parsetree.push(NT_Print);
	eat_token(scanner.next_token());
	Expression();
	parsetree.pop();
}

void parser_t::Expression(){
	parsetree.push(NT_Expression);
	Term();
	AD();
	parsetree.pop();
	


}

void parser_t::AD(){//////////////////////////////////////////////
	parsetree.push(NT_AD);

	switch(scanner.next_token()){
	case T_plus:
		cerr<<"+";
		eat_token(scanner.next_token());
		Term();
		AD();
		break;
	case T_minus:
		cerr<<"-";
		eat_token(scanner.next_token());
		Term();
		AD();
		break;
	case T_label://
		parsetree.drawepsilon();
		break;
	case T_goto:
		parsetree.drawepsilon();
		break;
	case T_if:
		parsetree.drawepsilon();		
		break;	
	case T_m:
		parsetree.drawepsilon();
		break;	
	case T_print:
		parsetree.drawepsilon();
		break;
	case T_closesquare:
		parsetree.drawepsilon();
		break;
	case T_closeparen:
		parsetree.drawepsilon();
		break;
	case T_eof://////////////////////////////////////////////////////////////////
		parsetree.drawepsilon();
		break;
	default:
		syntax_error(NT_MD);
		break;


}
	parsetree.pop();


}

void parser_t::Term(){
	parsetree.push(NT_Term);
	Pow();
	MD();	
	parsetree.pop();
}

void parser_t::MD(){///////////////////////////////////

	parsetree.push(NT_MD);

	switch(scanner.next_token()){
	case T_times:
		cerr<<scanner.get_current()<<"*";
		eat_token(scanner.next_token());
		Pow();
		MD();
		break;
	case T_divide:
		cerr<<"/";
		eat_token(scanner.next_token());
		Pow();
		MD();
		break;
	case T_plus:
		parsetree.drawepsilon();
		break;
	case T_minus:
		parsetree.drawepsilon();
		break;	
	case T_label:
		parsetree.drawepsilon();
		break;
	case T_goto:
		parsetree.drawepsilon();
		break;
	case T_if:
		parsetree.drawepsilon();		
		break;	
	case T_m:
		parsetree.drawepsilon();
		break;	
	case T_print:
		parsetree.drawepsilon();
		break;
	case T_closesquare:
		parsetree.drawepsilon();
		break;
	case T_closeparen:
		parsetree.drawepsilon();
		break;
	case T_eof://////////////////////////////////////////////////////////////////
		parsetree.drawepsilon();
		break;
	default:
		syntax_error(NT_MD);
		break;


}
	parsetree.pop();

}
void parser_t::Pow(){
	parsetree.push(NT_Pow);
	Fact();
	Exponent();	
	parsetree.pop();
}

void parser_t::Exponent(){/////////////////////////////////

	parsetree.push(NT_Exponent);

	switch(scanner.next_token()){
	case T_power:////////////

		eat_token(scanner.next_token());
				cerr<<"pow("<<scanner.get_current()<<",";
		Fact();
		cerr<<")";
		Exponent();
		break;
	case T_divide:
		parsetree.drawepsilon();
		break;
	case T_times:
		parsetree.drawepsilon();
		break;		
	case T_plus:
		parsetree.drawepsilon();
		break;
	case T_minus:
		parsetree.drawepsilon();
		break;			
	case T_label:
		parsetree.drawepsilon();
		break;
	case T_goto:
		parsetree.drawepsilon();
		break;
	case T_if:
		parsetree.drawepsilon();		
		break;	
	case T_m:
		parsetree.drawepsilon();
		break;	
	case T_print:
		parsetree.drawepsilon();
		break;
	case T_closesquare:
		parsetree.drawepsilon();
		break;
	case T_closeparen:
		parsetree.drawepsilon();
		break;
	case T_eof://////////////////////////////////////////////////////////////////
		parsetree.drawepsilon();
		break;
	default:
		syntax_error(NT_MD);
		break;


}
	parsetree.pop();
}

void parser_t::Fact(){
	parsetree.push(NT_Fact);
	switch( scanner.next_token() ){
	case T_m:
		cerr<<"m";
		eat_token(scanner.next_token());
		if(scanner.next_token()!=T_opensquare)
			syntax_error(NT_Fact);
		cerr<<"[";
		eat_token(scanner.next_token());
		Expression();
		if(scanner.next_token()!=T_closesquare)
			syntax_error(NT_Fact);
		cerr<<"]";
		eat_token(scanner.next_token());
		break;
	case T_openparen:
		cerr<<"(";
		eat_token(scanner.next_token());
		Expression();
		if(scanner.next_token()!=T_closeparen)
			syntax_error(NT_Fact);
		cerr<<")";
		eat_token(scanner.next_token());
	break;
	case T_num:
		eat_token(scanner.next_token());
		break;
	case T_eof://///////////////////////////////////////////////////////////
		parsetree.drawepsilon();
		break;
	default:
		syntax_error(NT_Fact);
		break;

	} 
	parsetree.pop();
}







/*** Main ***********************************************/

int main()
{


		
	cerr<<"#include <stdio.h>\n#include<math.h>\nint main(){\n";
	cerr<<"\tint m[101];\n\t";
	cerr<<"int i=0;\n\t";
	cerr<<"for(i=0;i<=100;i++)\n\t\t";
	cerr<<"m[i]=0;\n\t";
	parser_t parser;
	parser.parse();
	cerr<<"return 0;\n}\n";


		


	return 0;
}


