#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <limits>
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
	NT_Expression,
    NT_Statements,
    NT_Statement,
    NT_Label,
    NT_Jump,
    NT_Forward,
    NT_AnotherForward,
    NT_Assignment,
    NT_Print,
    NT_Factor,
    NT_Expressionha,
    NT_Term,
    NT_Exponent,
    NT_Factorha,
    NT_Termha,
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
		  //WRITEME: add the other nonterminals you need here		
        case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Forward: strncpy(buffer,"Forward",MAX_SYMBOL_NAME_SIZE); break;
        case NT_AnotherForward: strncpy(buffer,"AnotherForward",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Expressionha: strncpy(buffer,"Expressionha",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Exponent: strncpy(buffer,"Exponent",MAX_SYMBOL_NAME_SIZE); break;

        case NT_Factorha: strncpy(buffer,"Factorha",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Termha: strncpy(buffer,"Termha",MAX_SYMBOL_NAME_SIZE); break;

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
    int myToken;
	//constructor 
	scanner_t();
    int myLine;
    int nextNumber;

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

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	token_type bogo_token;

};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
	//if ( bogo_token!=T_plus && bogo_token!=T_eof ) return T_plus;
	//else return bogo_token;
    //token_type* nextToken;
    int nextToken;
    if(myToken != -1){
        return (token_type)myToken;
    }
    int haha;
    while(true){
        //haha = cin.peek();
        haha = cin.get();
        if(haha == EOF){
            nextToken = T_eof;
        }else if(haha == '\n'){
            myLine++;
            continue;
        }else if((haha == ' ') || (haha == '\t') || (haha =='\v') || (haha =='f') || (haha == '\r')){
            continue;
        }else if(haha == '+'){
            nextToken = T_plus;
        }else if(haha == '-'){
            nextToken = T_minus;
        }else if(haha == '/'){
            nextToken = T_divide;
        }else if(haha == '='){
            nextToken = T_equals;
        }else if(haha == '('){
            nextToken = T_openparen;
        }else if(haha == ')'){
            nextToken = T_closeparen;
        }else if(haha == '['){
            nextToken = T_opensquare;
        }else if(haha == ']'){
            nextToken = T_closesquare;
        }else if(haha == 'm'){
            nextToken = T_m;
        }else if(haha == 'L'){
            nextToken = T_label;
        }else if(haha == ':'){
            nextToken = T_colon;
        }else if(haha == '*'){
            //oh oh! could be two different stuff
            if(cin.peek() =='*'){
                haha = cin.get();
                nextToken = T_power;
            }else{
                nextToken = T_times;
            }
        }else if(haha>='0' && haha <='9'){
            char myCharacter[11];
            myCharacter[0] = haha;
            int i = 1;
            for(i = 1; i < 11; i++){
                haha = cin.peek();
                if(haha>='0' && haha<='9'){
                    haha = cin.get();
                    myCharacter[i] = haha;
                }else{
                    break;
                }
            }
            myCharacter[i] = '\0';
            if(cin.peek() >= '0' && cin.peek()<='9'){
                scan_error(cin.peek());
            }
            long int holder = atol(myCharacter);
            int imax = std::numeric_limits<int>::max();
            if(holder >=0 && holder<= imax){
                nextNumber = (int)holder;
                nextToken = T_num;
            }else{
                scan_error(myCharacter[0]);
            }
            
        }else if(haha == 'i'){
            haha = cin.get();
            if( haha == 'f'){
                nextToken = T_if;
            }else{
                scan_error(haha);
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////goto//////print///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }else if(haha == 'g'){
            haha = cin.get();
            if(haha == 'o'){
                haha = cin.get();
                if(haha == 't'){
                    haha = cin.get();
                    if(haha == 'o'){
                        haha = cin.get();
                        nextToken = T_goto;
                    }else{
                        scan_error(haha);
                    }
                }else{
                    scan_error(haha);
                }
            }else{
                scan_error(haha);
            }
        }else if(haha == 'p'){
            haha = cin.get();
            if(haha == 'r'){
                haha = cin.get();
                if(haha == 'i'){
                    haha = cin.get();
                    if(haha == 'n'){
                        haha = cin.get();
                        if(haha == 't'){
                            haha = cin.get();
                            nextToken = T_print;
                        }else{
                            scan_error(haha);
                        }
                    }else{
                        scan_error(haha);
                    }
                }else{
                    scan_error(haha);
                }
            }else{
                scan_error(haha);
            }
        }
        else{
            scan_error(haha);//other characters
        }
        if(nextToken != -1){
            break;//how to break out of this silly loop
        }
    }
    myToken = nextToken;
    return (token_type)nextToken;
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eaT_token
	//if ( rand()%10 < 8 ) bogo_token = T_plus;
	//else bogo_token = T_eof;
    if(myToken == -1)
        next_token();
    if(myToken !=(int)c)
        mismatch_error(c);
    else
        myToken = -1;

}

scanner_t::scanner_t()
{
    myToken = -1;
    nextNumber = 0;
    myLine = 1;
	//WRITEME: this is the scanner constructor
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
    return myLine;
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
	void eat_token(token_type t);
	void syntax_error(nonterm_type);

	void Start();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)
    void Statements();
    void Statement();
    string Label(string s);
    string Jump(string s);
    string Forward(string s);
    string AnotherForward(string s);
    string Assignment(string s);
    string Print(string s);
    string Factor(string s);
    string Expression(string s);
    string Expressionha(string s);
    string Term(string s);
    string Exponent(string s);
    string Factorha(string s);
    string Termha(string s);

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
    cerr << "#include <stdio.h>" << endl;
    cerr << "#include <math.h>" << endl;
    cerr << "int main(){" << endl;
    cerr << "int m[101];" << endl;
	Start();
    cerr << "}" << endl;
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
	/*parsetree.push(NT_Start);

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
     */
    
    token_type watch = scanner.next_token();
    parsetree.push(NT_Start);
    if((watch == T_label) || (watch == T_goto) || (watch == T_m)||(watch == T_print)){
        Statement();
        Statements();
    }else{
        syntax_error(NT_Start);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
            break;
        default:
            syntax_error(NT_Start);
            break;
    }
    parsetree.pop();
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
}

//WRITEME: you will need to put the rest of the procedures here
void parser_t::Statements()
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Statements);
    if((watch == T_label) || (watch == T_goto) || (watch == T_m)||(watch == T_print)){
        Statement();
        Statements();
    }else if(watch == T_eof){
        parsetree.drawepsilon();
    }else{
        syntax_error(NT_Statements);
    }
    parsetree.pop();
}







void parser_t::Statement()
{
    string s;
    token_type watch = scanner.next_token();
    parsetree.push(NT_Statement);
    if(watch == T_label){
        s = Label(s);
    }else if(watch == T_goto){
        s = Jump(s);
    }else if(watch == T_m){
        s = Assignment(s);
    }else if(watch == T_print){
        s = Print(s);
    }else{
        syntax_error(NT_Statement);
    }
    cerr<<s<<";"<<endl;
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Statement);
            break;
    }
    parsetree.pop();
}








string parser_t::Label(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Label);
    if(watch == T_label){
        s.append("L");
        eat_token(T_label);
        eat_token(T_num);
        char buf[100];
        snprintf(buf, 100, "%d", scanner.nextNumber);
        s.append(buf);
        s.append(":");
        eat_token(T_colon);
    }else{
        syntax_error(NT_Label);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Label);
            break;
    }
    parsetree.pop();
    return s;
}







string parser_t::Jump(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Jump);
    if(watch == T_goto){
        eat_token(T_goto);
        s = Forward(s);
    }else{
        syntax_error(NT_Jump);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Jump);
            break;
    }
    parsetree.pop();
    return s;
}




string parser_t::Forward(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Forward);
    if(watch == T_num){
        eat_token(T_num);
        char buf[100];
        snprintf(buf, 100, "%d", scanner.nextNumber);
        watch = scanner.next_token();
        if(watch == T_if){
            s.append("if(");
        }
        s = AnotherForward(s);
        if(watch == T_if){
            s.append("){");
        }
        s.append("goto L");
        s.append(buf);
        if(watch == T_if){
            s.append(";}");
        }
    }else{
        syntax_error(NT_Forward);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Forward);
            break;
    }
    parsetree.pop();
    return s;
}   






string parser_t::AnotherForward(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_AnotherForward);
    switch(watch){
        case T_if:
            eat_token(T_if);
            s = Expression(s);
            break;
        case T_eof:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            parsetree.drawepsilon();
            break;
        default:
            syntax_error(NT_AnotherForward);
            break;
    }
    parsetree.pop();
    return s;
}






string parser_t::Assignment(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Assignment);
    if(watch == T_m){
        s.append("m");
        eat_token(T_m);
        s.append("[");
        eat_token(T_opensquare);
        s = Expression(s);
        s.append("]");
        eat_token(T_closesquare);
        s.append("=");
        eat_token(T_equals);
        s = Expression(s);
    }else{
        syntax_error(NT_Print);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Print);
            break;
    }
    parsetree.pop();
    return s;
}





string parser_t::Print(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Print);
    if(watch == T_print){
        s.append("printf(\"\%d\\n\",");
        eat_token(T_print);
        s = Expression(s);
        s.append(")");
    }else{
        syntax_error(NT_Print);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Print);
            break;
    }
    parsetree.pop();
    return s;
}






string parser_t::Factor(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Factor);
    if(watch == T_openparen){
        s.append("(");
        eat_token(T_openparen);
        s = Expression(s);
        s.append(")");
        eat_token(T_closeparen);
    }else if(watch == T_num){
        char buf[100];
        snprintf(buf, 100, "%d", scanner.nextNumber);
        s.append(buf);
        eat_token(T_num);
    }else if(watch == T_m){
        s.append("m");
        eat_token(T_m);
        s.append("[");
        eat_token(T_opensquare);
        s = Expression(s);
        s.append("]");
        eat_token(T_closesquare);
    }else{
        syntax_error(NT_Factor);
    }
    
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_power:
        case T_times:
        case T_divide:
        case T_plus:
        case T_minus:
        case T_closeparen:
        case T_closesquare:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Factor);
            break;
    }
    parsetree.pop();
    return s;
}






string parser_t::Expression(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Expression);
    if(watch == T_openparen){
        s = Term(s);
        s = Expressionha(s);
    }else if(watch == T_num){
        s = Term(s);
        s = Expressionha(s);
    }else if(watch == T_m){
        s = Term(s);
        s = Expressionha(s);
    }else{
        syntax_error(NT_Expression);
    }
    
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_closeparen:
        case T_closesquare:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Expression);
            break;
    }
    parsetree.pop();
    return s;
}





string parser_t::Expressionha(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Expressionha);
    if(watch == T_plus || watch == T_minus){
        s.append(token_to_string(watch));
        eat_token(watch);
        s = Term(s);
        s = Expressionha(s);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_closeparen:
        case T_closesquare:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            parsetree.drawepsilon();
            break;
        default:
            syntax_error(NT_Expressionha);
            break;
    }
    parsetree.pop();
    return s;
}






string parser_t::Term(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Term);
    if(watch == T_num || watch == T_m || watch == T_openparen){
        s = Exponent(s);
        s = Termha(s);
    }else{
        syntax_error(NT_Term);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        
        case T_plus:
        case T_minus:
        case T_closeparen:
        case T_closesquare:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Term);
            break;
    }
    parsetree.pop();
    return s;
}





string parser_t::Exponent(string s)
{
    string s2;
    token_type watch = scanner.next_token();
    parsetree.push(NT_Exponent);
    if(watch == T_num || (watch == T_m) || (watch == T_openparen)){
        s2 = Factor(s2);
        watch = scanner.next_token();
        if(watch == T_power){
            s2.insert(0, "(int)pow((int)");
            s.append(s2);
            s.append(",(int)");
        }
        s = Factorha(s);
        if(watch == T_power){
            s.append(")");
        }else{
            s.append(s2);
        }
    }else{
        syntax_error(NT_Exponent);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_times:
        case T_divide:
        case T_plus:
        case T_minus:
        case T_closeparen:
        case T_closesquare:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            break;
        default:
            syntax_error(NT_Exponent);
            break;
    }
    parsetree.pop();
    return s;
}






string parser_t::Factorha(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Factorha);
    if(watch == T_power){
        eat_token(T_power);
        s = Exponent(s);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_times:
        case T_divide:
        case T_plus:
        case T_minus:
        case T_closeparen:
        case T_closesquare:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            parsetree.drawepsilon();
            break;
        default:
            syntax_error(NT_Factorha);
            break;
    }
    parsetree.pop();
    return s;
}






string parser_t::Termha(string s)
{
    token_type watch = scanner.next_token();
    parsetree.push(NT_Termha);
    if(watch == T_divide || watch == T_times){
        s.append(token_to_string(watch));
        eat_token(watch);
        s = Factor(s);
        s = Termha(s);
    }
    watch = scanner.next_token();
    switch(watch){
        case T_eof:
        case T_plus:
        case T_minus:
        case T_closeparen:
        case T_closesquare:
        case T_label:
        case T_goto:
        case T_m:
        case T_print:
            parsetree.drawepsilon();
            break;
        default:
            syntax_error(NT_Termha);
            break;
    }
    parsetree.pop();
    return s;
}






/*** Main ***********************************************/

int main()
{
    /*scanner_t scans;
    token_type t;
    while(true){
        t = scans.next_token();
        cout<<token_to_string(t)<<endl;
        scans.eat_token(t);
        
        if(t == T_eof)
            break;
        
    }
     */
	parser_t parser;
	parser.parse();
	return 0;
}
