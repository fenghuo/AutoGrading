#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <ctype.h>
#include <stack>
#include <cctype>
#include <iostream>
#include <sstream>

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
	T_closeparen,	 	// 9: )
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
		case T_eof:			strncpy(buffer,"eof",MAX_SYMBOL_NAME_SIZE); break;
		case T_num:			strncpy(buffer,"num",MAX_SYMBOL_NAME_SIZE); break;
		case T_plus:		strncpy(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
		case T_minus:		strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
		case T_times:		strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
		case T_divide:		strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
		case T_power:		strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
		case T_equals:		strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
		case T_openparen:	strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
    	case T_closeparen:	strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
    	case T_opensquare:	strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
    	case T_closesquare:	strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
    	case T_m:			strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
    	case T_goto:		strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
    	case T_if:			strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
    	case T_print:		strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
    	case T_label:		strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
    	case T_colon:		strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
		default: 			strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
	}
	return buffer;
}

//all of the non-terminals in the grammar (you need to add these in
//according to your grammar.. these are used for printing the thing out)
//please follow the convention we set up so that we can tell what the heck you
//are doing when we grade
typedef enum {
	epsilon = 100,		// 100: epsilon
	NT_Start,			// 101: Start
	NT_S1,			//102: S1
	NT_A1,		// 103: A1
	NT_Label,			// 104: Label
	NT_Jump,			// 105: Jump
	NT_Assignment,		// 106: Assignment
	NT_Print,			// 107: Print
	NT_Expression,		// 108: Expression
	NT_E1,
	NT_E2,
	NT_E3,
	NT_E4,
	NT_I,
	NT_alpha1,
	NT_alpha2,
	NT_alpha3
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
		  case NT_S1: strncpy(buffer,"S1",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_A1: strncpy(buffer,"A1",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_E1: strncpy(buffer,"E1",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_E2: strncpy(buffer,"E2",MAX_SYMBOL_NAME_SIZE); break;
		 	case NT_I: strncpy(buffer,"I",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_E3: strncpy(buffer,"E3",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_E4: strncpy(buffer,"E4",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_alpha1: strncpy(buffer,"alpha1",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_alpha2: strncpy(buffer,"alpha2",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_alpha3: strncpy(buffer,"alpha3",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
		}
	return buffer;
}

/*** List Class ***********************************************/

class node{
public:
	char c;
	node *next;

	node(){
		next = NULL;
	}	
	node(char _c){
		c = _c;
		next = NULL;
	}
};

class queue{
public: 
	node *head;

	queue(){
		head = NULL;
	}

	node *pop(){
		node *tmp = head;
		if (head->next != NULL) head = head->next;
		return tmp;
	}

	void push(char key){
		if (head==NULL){
			head = new node(key);
		}
		else{
			node *p = head;
			while(p->next!=NULL){
				p = p->next;
			}
			p->next = new node(key);
		}	
	}
};

class stringnode{
public:
	string c;
	stringnode *next;

	stringnode(){
		next = NULL;
	}	
	stringnode(string _c){
		c = _c;
		next = NULL;
	}
};

class stringlist{
public: 
	stringnode *head;

	stringlist(){
		head = NULL;
	}
	void powit(){
		if(head != NULL ){
		string temp = head->c;
		head->c = "pow(";
		head->c += temp;
		head->c += ",";
	}

	}
	void pop(){
		if (head == NULL){
			//cerr <<"empty head";
		}
		else if (head->next == NULL){
			cerr << head->c;
			head = NULL;
		}
		else {
			head->next->c += head->c; 
			head = head->next;
		}
		//stringnode *tmp = head;
		//if (head->next != NULL) head = head->next;
		//return tmp;
	}

	void push(string key){
		if (head==NULL){
			head = new stringnode(key);
		}
		else{
			stringnode *tmp = head;
			head = new stringnode(key);
			head->next = tmp;
		}	
	}
	void append(string key){
		if(head == NULL){
			head = new stringnode(key);
		}
		else {head->c += key;}
	}
};

/*** Scanner Class ***********************************************/

class scanner_t {
  public:

	//eats the next token and prints an error if it is not of type c
	void eat_token(token_type c);
	
	//peeks at the lookahead token
	token_type next_token();

	//return line number for errors
	int get_line();

	int get_num();

	int eat_num();

	//constructor 
	scanner_t();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  

  	queue *toqenz;

  	int line_number;
  	int num_length;
  	int num;

  	token_type powerScan();
  	token_type ifScan();
  	token_type gotoScan();
  	token_type printScan();
  	token_type spaceScan();
  	token_type numberScan(node* n);

  	void pop_and_add_char(int nums);

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
};

int scanner_t::get_num(){
	return num;
}

token_type scanner_t::gotoScan(){
	if (toqenz->head->next->c == 'o' &&
		toqenz->head->next->next->c == 't' &&
		toqenz->head->next->next->next->c == 'o'){
		return T_goto;
	}else scan_error(toqenz->head->next->c);
}

token_type scanner_t::printScan(){
	if (toqenz->head->next->c == 'r' &&
		toqenz->head->next->next->c == 'i' &&
		toqenz->head->next->next->next->c == 'n' &&
		toqenz->head->next->next->next->next->c == 't'){
		return T_print;
	}else scan_error(toqenz->head->next->c);
}

token_type scanner_t::ifScan(){
	if (toqenz->head->next->c == 'f'){
		return T_if;
	}else{
		scan_error(toqenz->head->next->c);
	}
}

token_type scanner_t::powerScan(){
	if (toqenz->head->next->c == '*'){
		return T_power;
	}else{
		return T_times;
	}
}
token_type scanner_t::spaceScan(){
	if(toqenz->head->c == ' '){
		pop_and_add_char(1);
		return spaceScan();
	}
	else
		return next_token();
}
token_type scanner_t::numberScan(node* n){
	//cout<<"entered numberscan. num_length = "<<num_length<<endl;
	num_length++;
	if(n->next != NULL && isdigit(n->next->c)){
		return numberScan(n->next);
	}
	else{
		return T_num;
	}
}


token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
	
	if (isdigit(toqenz->head->c)){
		//cout <<"we have a digit here \n";
		num_length = 0;
		return numberScan(toqenz->head);
	}
	else
	{
		//cout<<"line number is " << line_number << "\n";
		switch(toqenz->head->c)
		{
			case '\n': line_number++; pop_and_add_char(1); return next_token();
			case ' ': return spaceScan();
			case '+': return T_plus;
			case '-': return T_minus;
			case '/': return T_divide;
			case '=': return T_equals;
			case '(': return T_openparen;
			case ')': return T_closeparen;
			case '[': return T_opensquare;
			case ']': return T_closesquare;
			case ':': return T_colon;
			case 'm': return T_m;
			case 'L': return T_label;
			case '*': return powerScan();//add to buffer, check for "**"
			case 'g': return gotoScan(); // goto
			case 'p': return printScan(); // print
			case 'i': return ifScan(); // if
			case '$': return T_eof;
			default: scan_error(toqenz->head->c);
		}
	}

}

void scanner_t::pop_and_add_char(int nums){
	for (int i = 0; i < nums; i++){
		toqenz->pop();
		char c;
		if (cin.get(c)) {
			//cout <<"current char is :"; cout << c; cout <<"\n";
			toqenz->push(c);
					
		}else{
			toqenz->push('$');
		}

	}
}

void scanner_t::eat_token(token_type c)
{
	//cout << "num length: "<< num_length <<endl;
	switch(c)
		{
			case T_plus: pop_and_add_char(1); break;
			case T_minus: pop_and_add_char(1); break;
			case T_times: pop_and_add_char(1); break;
			case T_divide: pop_and_add_char(1); break;
			case T_equals: pop_and_add_char(1);  break;
			case T_openparen: pop_and_add_char(1); break;
			case T_m: pop_and_add_char(1); break;
			case T_num: pop_and_add_char(num_length); num_length = 0; break;
			case T_label: pop_and_add_char(1); break;
			case T_closeparen: pop_and_add_char(1); break;
			case T_opensquare: pop_and_add_char(1); break;
			case T_closesquare: pop_and_add_char(1); break;
			case T_colon: pop_and_add_char(1); break;
			case T_eof: toqenz->pop(); break;
			case T_goto: pop_and_add_char(4); break;
			case T_print: pop_and_add_char(5); break;
			case T_if: pop_and_add_char(2); break;
			case T_power: pop_and_add_char(2); break;
			default: mismatch_error(c);
		}


	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
}

scanner_t::scanner_t()
{
	num_length = 0;
	num = 0;
	toqenz = new queue();
	char c;
	for (int i = 0; i<MAX_SYMBOL_NAME_SIZE; i++)
	{
		if (cin.get(c)) {
			toqenz->push(c);					
		}
		else {
			toqenz->push('$');
			break;
		}
	}
	line_number = 1;
}

int scanner_t::eat_num(){
	node *n = toqenz->head;
	string s = "";
	int i = 0;
	while (i < num_length){
		i++;
		s += n->c;
		n = n->next;
	}
	eat_token(T_num);
	return atoi(s.c_str());
}

int scanner_t::get_line()
{
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
	//Grammar:
	void Start();
	void S1();
	void A1();
	void E();
	void E1();
	void E2();
	void E3();
	void E4();
	void I();
	void alpha1();
	void alpha2();
	void alpha3();
	//Helper methods:
	int eatNum(); //returns the actual value of the T_num
	stringlist stringz; //keeps track of the 

  public:
	void parse();
};

//this function not only eats the token (moving the scanner forward one
//token), it also makes sure that token is drawn in the parse tree 
//properly by calling push and pop.
void parser_t::eat_token(token_type t)
{
	parsetree.push(t);
	scanner.next_token();
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
	cerr<<"#include <math.h>\n";
	cerr<<"#include <stdio.h>\n";
	cerr<<"int main (){\n";
	cerr<<"int m[101] = {0};\n";
	Start();
	cerr<<"return 0;\n}\n";
}

void parser_t::Start(){
	parsetree.push(NT_Start);
	S1();
	A1();
	parsetree.pop();
}

void parser_t::A1(){
	parsetree.push(NT_A1);

	switch( scanner.next_token() ) 
	{
		case T_label:
			eat_token(T_label);
			if (scanner.next_token() == T_num) {   cerr<<"L"<<eatNum(); }
			else syntax_error(NT_S1);
			if (scanner.next_token() == T_colon) { eat_token(T_colon); cerr<<":\n";}
			else syntax_error(NT_S1);
			A1();
			break;
		case T_goto:
			eat_token(T_goto);
			int num;
			if (scanner.next_token() == T_num) { num = eatNum();}
			else syntax_error(NT_S1);
			I();
			cerr<<"goto L"<<num<<";\n";
			A1();
			break;
		case T_m:
			eat_token(T_m); cerr<<"m";
			if (scanner.next_token() == T_opensquare) { eat_token(T_opensquare); cerr<<"[";}
			else syntax_error(NT_S1);
			E();
			if (scanner.next_token() == T_closesquare) { eat_token(T_closesquare); cerr<<"]";}
			else syntax_error(NT_S1);
			if (scanner.next_token() == T_equals){eat_token(T_equals); cerr<<"=";}
			else syntax_error(NT_S1);
			E();
			cerr<<";\n";
			A1();
			break;
		case T_print:
			eat_token(T_print);
			cerr<<"printf(\"%i\\n\", ";
			E();
			cerr<<");\n";
			A1();
			break;
		default:
			parsetree.drawepsilon();
			break;
	}

	parsetree.pop();
}

void parser_t::S1()
{
	parsetree.push(NT_S1);

	switch( scanner.next_token() ) 
	{
		case T_label:
			eat_token(T_label);
			if (scanner.next_token() == T_num) {   
				cerr<<"L"<<eatNum(); 
			}
			else if (scanner.next_token() == T_colon) {
				eat_token(T_colon);
				cerr<<":\n";
			}
			else syntax_error(NT_S1);
			break;
		case T_goto:
			eat_token(T_goto);
			int num;
			if (scanner.next_token() == T_num) {
				num = eatNum();
			}
			else syntax_error(NT_S1);
			I();
			cerr<<"goto L"<<num<<";";
			break;
		case T_m:
			eat_token(T_m); cerr<<"m";
			if (scanner.next_token() == T_opensquare) {
				eat_token(T_opensquare); 
				cerr<<"[";
			}
			else syntax_error(NT_S1);
			E();
			if (scanner.next_token() == T_closesquare) {
				eat_token(T_closesquare);
				cerr<<"]";
			}
			else syntax_error(NT_S1);
			if (scanner.next_token() == T_equals){
				eat_token(T_equals);
				cerr<<"=";
			}
			else syntax_error(NT_S1);
			E();
			cerr<<";\n";
			break;
		case T_print:
			eat_token(T_print);
			cerr<<"printf(\"%i\", ";
			E();
			cerr<<");\n";
			break;
		default:
			syntax_error(NT_Start);
			break;
	}

	parsetree.pop();
}

int parser_t::eatNum(){
	parsetree.push(T_num);
	scanner.next_token();
	parsetree.pop();
	int num = scanner.eat_num();
	if (num >= 0){
		return num;
	}
	else syntax_error(NT_E4);
}

void parser_t::I(){
	parsetree.push(NT_I);
	if (scanner.next_token() == T_if){
		eat_token(T_if); cerr<<"if(";
		E();
		cerr<<") ";
	}
	parsetree.pop();
}

void parser_t::E(){
	parsetree.push(NT_Expression);
	E1(); alpha1();
	parsetree.pop();
}

void parser_t::E1(){
	parsetree.push(NT_E1);
	E2(); alpha2();
	parsetree.pop();
}

void parser_t::E2(){
	parsetree.push(NT_E3);
	stringz.pop();
	E3(); alpha3();
	parsetree.pop();
}

void parser_t::E3(){
	parsetree.push(NT_E3);
	if (scanner.next_token() == T_m){
		stringz.append("m");
		eat_token(T_m);
		if(scanner.next_token() == T_opensquare){
			stringz.append("[");
			eat_token(T_opensquare);
		}
		else syntax_error(NT_E3);
		E();
		if(scanner.next_token() == T_closesquare){
			stringz.append("]");
			eat_token(T_closesquare);
		}
		else syntax_error(NT_E3);
	}
	else E4();
	parsetree.pop();
}

void parser_t::E4(){
	parsetree.push(NT_E4);
	if (scanner.next_token() == T_openparen){
		stringz.append("(");
		eat_token(T_openparen);
		E();
		if (scanner.next_token() == T_closeparen) {
			stringz.append(")");
			eat_token(T_closeparen);
		}
		else syntax_error(NT_E4);
	}
	else if (scanner.next_token() == T_num){
			stringstream s;
			s << eatNum();
			stringz.push(s.str());
	}else{
		syntax_error(NT_E4);
	}
	parsetree.pop();
}

void parser_t::alpha1(){
	parsetree.push(NT_alpha1);
	if (scanner.next_token()==T_plus || scanner.next_token()==T_minus){
		token_type token = scanner.next_token();
		stringz.append(token_to_string(token));
		eat_token(token);
		E1(); alpha1();
	}
	else parsetree.drawepsilon();
	parsetree.pop();
}

void parser_t::alpha2(){
	parsetree.push(NT_alpha2);
	if (scanner.next_token()==T_times || scanner.next_token()==T_divide){
		token_type token = scanner.next_token();
		stringz.append(token_to_string(token));
		eat_token(token);
		E2(); alpha2();
	}
	else parsetree.drawepsilon();
	parsetree.pop();
}

void parser_t::alpha3(){
	parsetree.push(NT_alpha3);
	if (scanner.next_token() == T_power){
		stringz.powit(); 
		eat_token(T_power);
		E3();
		alpha3();
		stringz.append(")");
	}
	else parsetree.drawepsilon();
	stringz.pop();
	parsetree.pop();
}

/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();

	return 0;
}
