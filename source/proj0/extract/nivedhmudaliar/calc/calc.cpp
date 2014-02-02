#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25

#define MAX_NUMBER 2147483647

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
	NT_S,
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_A,
	NT_Assignment,
	NT_Print, 
	NT_Expression,
	NT_C,
	NT_Goto,
	NT_M,
	NT_D, 
	NT_F,
	NT_Term,
	NT_Factor,
	NT_Val
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
		  case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_S: strncpy(buffer,"S",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_A: strncpy(buffer,"A",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_C: strncpy(buffer,"C",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_D: strncpy(buffer,"D",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_F: strncpy(buffer,"F",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Val: strncpy(buffer,"Val",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Goto: strncpy(buffer,"Goto",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_M: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;

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
	int getCurrentNumber();


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

	void range_error(char *num_string);


	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.

	int currentNumber;

	int lineNumber; 	//current line number
	char *currentLineOfInstructions; //string of the current line being scanned


};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 

	if(feof(stdin)){
		return T_eof;
	}
	switch(currentLineOfInstructions[0]){
		case ' ': 
					currentLineOfInstructions++;
					return next_token();
		case '+': 	return T_plus;
		case  '-': 	return T_minus;
		case '/': 	return T_divide;
		case '*': 	if(currentLineOfInstructions[1] == '*'){
						return T_power;
				  	}
				  	else{
				  		return T_times;
				  	}
				  	
		case '=': 	return T_equals;
		case '(': 	return T_openparen;
		case ')': 	return T_closeparen;
		case '[': 	return T_opensquare;
		case ']': 	return T_closesquare;
		case 'm': 	return T_m;
		case 'g': if(currentLineOfInstructions[1] != 'o')
					scan_error(currentLineOfInstructions[1]);
				  if(currentLineOfInstructions[2] != 't')
					scan_error(currentLineOfInstructions[2]);
				  if(currentLineOfInstructions[3] != 'o')
					scan_error(currentLineOfInstructions[3]);
				  return T_goto;

		case 'i': if(currentLineOfInstructions[1] != 'f')
					scan_error(currentLineOfInstructions[1]);
				  return T_if;
		case 'p': if(currentLineOfInstructions[1] != 'r')
					scan_error(currentLineOfInstructions[1]);
				  if(currentLineOfInstructions[2] != 'i')
				  	 scan_error(currentLineOfInstructions[2]);
				  if(currentLineOfInstructions[3] != 'n')
				  	scan_error(currentLineOfInstructions[3]);
				  if(currentLineOfInstructions[4] != 't')
				  	scan_error(currentLineOfInstructions[4]);
				  return T_print;
		case 'L': return T_label;
		case ':': return T_colon;

		
		//if there line is ending
		case '\n': 
				lineNumber++; //increment total lines
				fgets(currentLineOfInstructions, 1000, stdin); // read in the next line of up to 1000 characters
				return next_token();
				//return T_eof;
	}

	//if its none of these cases, then check for number
	if(currentLineOfInstructions[0] >= '0' || currentLineOfInstructions[0] <= '9'){
			return T_num;
	}
		
	//if its none of the above cases then its an error
		scan_error(currentLineOfInstructions[0]);

}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eat_token
	token_type tokenToEat = next_token();

	if(tokenToEat != c){
		mismatch_error(c);
	}
	
	switch(tokenToEat){
		case T_print: currentLineOfInstructions+=5; break; //if it is a print statement, then move 5 characters over because print is 5 characters
		case T_goto: currentLineOfInstructions+=4; break;
		case T_if: currentLineOfInstructions+=2;	break;
		case T_power: currentLineOfInstructions +=2; break;
		case T_num: {
					int j;
    				long int number;
					j = 0;
					//finding out the length of the number so we can increment the location in currentLineOfInstructions accordingly 
					while(currentLineOfInstructions[j] >= '0' && currentLineOfInstructions[j] <= '9'){
						j++;
					}
					char numberAsChar[j];
					strncpy(numberAsChar, currentLineOfInstructions, j);
					number = atol(numberAsChar);

					if(number > MAX_NUMBER || number < 0)
						range_error(numberAsChar);

					currentNumber = number;
					currentLineOfInstructions +=j;

					break;
				}
		default: currentLineOfInstructions++; break;
	}

}

int scanner_t::getCurrentNumber(){
	return currentNumber;
}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	lineNumber = 1; //initialize line number to be the first line

	currentLineOfInstructions = (char*)malloc(1000*sizeof(char)); //create a 1000 element character array
	fgets(currentLineOfInstructions, 1000, stdin); //reads characters from standard input and stores them as a C string in currentLineOfInstructions
									 // until 1000 characters have been read or a new line or EOF is reached

}

int scanner_t::get_line()
{
	return lineNumber;
}

void scanner_t::range_error(char *numString){
	printf("range error: num '%s' out of range - line %d\n", numString, get_line());
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
	void Expression();

	void Term();
	void C();
	void S();
	void D();
	void F();
	void Statements();
	void Statement();
	void Label();
	void Goto();
	void A(int currentNumber);
	void M();
	void Print();


	void Factor();

	void Val();
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
	fprintf(stderr,"#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>\n\nint main() {\n\tint m[101]\
;\n");

	Start();
	fprintf(stderr,"return 0;}\n");

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

	
	parsetree.push(NT_Start);

	switch( scanner.next_token() ) 
	{
		case T_label:
		case T_goto:
		case T_m:
		case T_print: 

			Statements();
			break;
		default: syntax_error(NT_Start); break;

	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

void parser_t::Statements(){
	parsetree.push(NT_Statements);

	switch(scanner.next_token()){
		case T_label:
		case T_goto:
		case T_m:
		case T_print: 
			Statement();
			S();
			break;
		default: syntax_error(NT_Statements);
	}

	parsetree.pop();
}

void parser_t::Statement(){
	parsetree.push(NT_Statement);


	switch( scanner.next_token() ) 
	{
		case T_label:
			Label();
			break;
		case T_goto:
			Goto();
			break;
		case T_m:
			M();
			break;
		case T_print: 
			Print();
			break;

		default: syntax_error(NT_Statement);

	}

	parsetree.pop();

}

void parser_t::Label(){
	parsetree.push(NT_Label);


	switch( scanner.next_token() ) 
	{
		case T_label:
			eat_token(T_label);
			eat_token(T_num);
			eat_token(T_colon);
			fprintf(stderr, "L");
			fprintf(stderr,"%d",scanner.getCurrentNumber());
			fprintf(stderr, ":\n");
			break;

		default: syntax_error(NT_Label);

	}

	parsetree.pop();

}
void parser_t::Goto(){
	parsetree.push(NT_Goto);


	switch( scanner.next_token() ) 
	{

		 
		case T_goto:
			eat_token(T_goto);
		 	//fprintf(stderr,"goto L");
			eat_token(T_num);
			//fprintf(stderr,"%d",scanner.getCurrentNumber());
			A(scanner.getCurrentNumber());
			break;

		default: syntax_error(NT_Goto);

	}

	parsetree.pop();

}

void parser_t::A(int currentNumber){
	parsetree.push(NT_A);


	switch( scanner.next_token() ) 
	{
		case T_if:
			eat_token(T_if);
			fprintf(stderr, "if(");
			Expression();
			fprintf(stderr,")\n\t\t");

			fprintf(stderr,"goto ");
			fprintf(stderr,"%d",currentNumber);
            fprintf(stderr,";\n");

			break;
		//epsilon cases
		case T_eof:
			fprintf(stderr,"goto ");
			fprintf(stderr,"%d",currentNumber);
            fprintf(stderr,";\n");
			parsetree.drawepsilon();
			break;
		case T_label:
			fprintf(stderr,"goto ");
			fprintf(stderr,"%d",currentNumber);
            fprintf(stderr,";\n");
			parsetree.drawepsilon();
			break;
		case T_goto:
			fprintf(stderr,"goto ");
			fprintf(stderr,"%d",currentNumber);
            fprintf(stderr,";\n");
			parsetree.drawepsilon();
			break;
		case T_equals:
			fprintf(stderr,"goto ");
			fprintf(stderr,"%d",currentNumber);
            fprintf(stderr,";\n");
			parsetree.drawepsilon();
			break;
		case T_print:
			fprintf(stderr,"goto ");
			fprintf(stderr,"%d",currentNumber);
            fprintf(stderr,";\n");
			parsetree.drawepsilon();
			break;

		default: syntax_error(NT_A);

	}

	parsetree.pop();
}

void parser_t::M(){
	parsetree.push(NT_M);


	switch( scanner.next_token() ) 
	{
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			fprintf(stderr, "m[");
			Expression();
			eat_token(T_closesquare);
			eat_token(T_equals);
			fprintf(stderr, "] = ");
			Expression();
			fprintf(stderr,";\n");

			break;

		default: 
				syntax_error(NT_M);

	}

	parsetree.pop();

}

void parser_t::Print(){
	parsetree.push(NT_Print);


	switch( scanner.next_token() ) 
	{
		case T_print:
			eat_token(T_print);
            fprintf(stderr,"printf(\"%%d\\n\", ");
			Expression();
			fprintf(stderr,");\n");
			break;

		default: syntax_error(NT_Print);

	}

	parsetree.pop();

}

void parser_t::S(){
	parsetree.push(NT_S);


	switch( scanner.next_token() ) 
	{
		case T_label:
		case T_goto:
		case T_m:
		case T_print: 

			Statement();
			S();
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;

		default: syntax_error(NT_S);

	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();

}

void parser_t::Expression(){

	parsetree.push(NT_Expression);

	Term();
	C();



	parsetree.pop();
}

void parser_t::C(){
	parsetree.push(NT_C);

	switch(scanner.next_token()){
		case T_plus:
			eat_token(T_plus);
			fprintf(stderr, " + ");
			Expression();
			break;
		case T_minus:
			eat_token(T_minus);
			fprintf(stderr, " - ");
			Expression();
			break;
		case T_label:
		case T_goto:
		case T_print:
		case T_closeparen:
		case T_closesquare:
		case T_eof:
		case T_m:
			  parsetree.drawepsilon(); 
		      break; //this is the epsilon case 
				
			
		 default:
				syntax_error(NT_C);
				
	}

	parsetree.pop();


}

void parser_t::Term(){
	parsetree.push(NT_Term);

	Factor();
	D(); 


	parsetree.pop();
}

void parser_t::D(){
	parsetree.push(NT_D);

	switch(scanner.next_token()){
		case T_times:
			eat_token(T_times);
			fprintf(stderr, " * ");
			Term();
			break;
		case T_divide:
			eat_token(T_divide);
			fprintf(stderr, " / ");
			Term();

		case T_plus:
		case T_minus:
		case T_m:
		case T_closeparen:
		case T_label:
		case T_closesquare:
		case T_eof:
		case T_print:
		case T_goto:


			parsetree.drawepsilon();break;
		
	 	default:
			syntax_error(NT_D);

			
		
		

	}

	parsetree.pop(); 
}

void parser_t::Factor(){
	parsetree.push(NT_Factor);

	Val();
	F();
	parsetree.pop(); 

}

void parser_t::F(){
	parsetree.push(NT_F);

		switch(scanner.next_token()){
			case T_power:
				eat_token(T_power);
				fprintf(stderr,"pow(%d,",scanner.getCurrentNumber());

 				Factor();

 				fprintf(stderr,")");

 				break;

 			case(T_times):

 			case(T_divide):

 			case(T_plus):

 			case(T_minus):

 			case(T_closeparen):

 			case(T_closesquare):

 			case(T_m):

 			case(T_label):

 			case(T_print):

 			case(T_goto):

 			case(T_eof):
 					parsetree.drawepsilon();  break; //this is to deal with epsilon case

 			
 	
 			
			default:	
					syntax_error(NT_F);

		}
			
		
	parsetree.pop();

}

void parser_t::Val(){
	parsetree.push(NT_Val);

	switch(scanner.next_token()){
		case T_openparen:
			eat_token(T_openparen);
			fprintf(stderr, "(");
			Expression();
			eat_token(T_closeparen);
			fprintf(stderr, ")");
			break;
		case T_m:
			eat_token(T_m);
			eat_token(T_opensquare);
			fprintf(stderr, "m[");
			Expression();
			eat_token(T_closesquare);
			fprintf(stderr, "]");
			break;
		case T_num:
			eat_token(T_num);
			fprintf(stderr,"%d",scanner.getCurrentNumber());


			break;
		default: syntax_error(NT_Val);

	}

	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

int main(int argc, char* argv[])
{
		parser_t parsetree;
		parsetree.parse();

	

	
}
