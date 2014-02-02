#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <queue>
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
    //WRITEME: add symbolic names for your non-terminals here
    NT_Statements,
    NT_Statement,
    NT_Label,
    NT_Jump,
    NT_Assignment,
    NT_Print,
    NT_Expr,
    NT_Term,
    NT_Ter,
    NT_Factor,
    NT_Number,
    NT_Integer
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
        case NT_Expr: strncpy(buffer,"Expr",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Ter: strncpy(buffer,"Ter",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Number: strncpy(buffer,"Number",MAX_SYMBOL_NAME_SIZE); break;
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

    //print contents
    void print();

    // get stored integer
    int get_number();
    void read();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  

    queue<token_type> *tokens;
    queue<int> *nums;
    queue<int> *newlines;
    int currentline;
    int currentindex;

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
    if(!tokens->empty())
        return tokens->front();
    else return T_eof;
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eat_token
    //if ( rand()%10 < 8 ) bogo_token = T_plus;
    //else bogo_token = T_eof;
    token_type token;
    if (!tokens->empty())
    {
        token = tokens->front();
        // mismatch error
        if(c!=token)
            this->mismatch_error(c);
        tokens->pop();
        // advance line number if necessary
        currentindex++;
        if(currentindex == newlines->front())
        {
            newlines->pop();
            currentline++;
        }
    } else {fprintf(stderr, "Unexpected end of file - no tokens remain.\n"); exit(1);} // unexpected end of file (no EOF)
}

scanner_t::scanner_t()
{
    //WRITEME: this is the scanner constructor
    tokens = new queue<token_type>();
    nums = new queue<int>();
    newlines = new queue<int>();
    currentline = 1;
    currentindex = 0;
}

// destructively prints the set of tokens
void scanner_t::print()
{
    token_type token;
    token = tokens->front();
    while(token != T_eof)
    {
        tokens->pop();
        fprintf(stderr, token_to_string(token));
        fprintf(stderr, " ");
        currentindex++;
        if(currentindex == newlines->front())
        {
            newlines->pop();
            currentline++;
            fprintf(stderr, "\n");
        }
        token = tokens->front();
    }
}

// pops and returns the next number in queue
// throws error if queue is empty
int scanner_t::get_number()
{
    int i;
    if(!nums->empty())
    {
        i = nums->front();
        nums->pop();
        return i;
    } else {fprintf(stderr, "Out of stored numbers!"); exit(1);}
}

void scanner_t::read()
{
    int i, c, number;
    c = getchar();
    int numtokens = 0;
    while (c != EOF)
    {
        number = -1;

        // if a numeric char, build an int!
        while(c>47&&c<58)
        {
            if(number == -1) number = 0;
            // append new digit to existing number
            i = c - 48;
            number = number*10 + i;
            c = getchar();
        }
        // push number, if one was created
        if(number != -1)
        {
            tokens->push(T_num);
            nums->push(number);
            numtokens++;
        }

        // switch for non-numeric chars
        switch(c)
        {
            case EOF:
                break; // in case reading a number pushes us to the eof
            case ' ':
                c = getchar(); break; // remove whitespace
            case '\n':
                newlines->push(numtokens); c = getchar(); break;
            case '+':
                tokens->push(T_plus); numtokens++; c = getchar(); break;
            case '-':
                tokens->push(T_minus); numtokens++; c = getchar(); break;
            case '*':
                c = getchar();
                if(c != '*') tokens->push(T_times);
                else {tokens->push(T_power); c = getchar();}
                numtokens++;
                break;
            case '/':
                tokens->push(T_divide); numtokens++; c = getchar(); break;
            case '=':
                tokens->push(T_equals); numtokens++; c = getchar(); break;
            case '[':
                tokens->push(T_opensquare); numtokens++; c = getchar(); break;
            case ']':
                tokens->push(T_closesquare); numtokens++; c = getchar(); break;
            case '(':
                tokens->push(T_openparen); numtokens++; c = getchar(); break;
            case ')':
                tokens->push(T_closeparen); numtokens++; c = getchar(); break;
            case ':':
                tokens->push(T_colon); numtokens++; c = getchar(); break;
            case 'L':
                tokens->push(T_label); numtokens++; c = getchar(); break;
            case 'm':
                tokens->push(T_m); numtokens++; c = getchar(); break;
            case 'g':
                c = getchar(); if(c != 'o') this->scan_error(c);
                c = getchar(); if(c != 't') this->scan_error(c);
                c = getchar(); if(c != 'o') this->scan_error(c);
                tokens->push(T_goto); numtokens++; c = getchar(); break;
            case 'i':
                c = getchar(); if(c != 'f') this->scan_error(c);
                tokens->push(T_if); numtokens++; c = getchar(); break;
            case 'p':
                c = getchar(); if(c != 'r') this->scan_error(c);
                c = getchar(); if(c != 'i') this->scan_error(c);
                c = getchar(); if(c != 'n') this->scan_error(c);
                c = getchar(); if(c != 't') this->scan_error(c);
                tokens->push(T_print); numtokens++; c = getchar(); break;
            default:
                this->scan_error(c); break;
        }
    }
    tokens->push(T_eof);
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
    return currentline;
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
    FILE* fileout;
	scanner_t scanner;
	parsetree_t parsetree;
	void eat_token(token_type t);
	void syntax_error(nonterm_type);

	void Start();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)
    void Statements();
    void Statement();
    void Label();
    void Jump();
    void Assignment();
    void Print();
    void Expression();
    void Expr();
    void Term();
    void Ter();
    void Factor();
    void Number();
    void Integer();

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
//to parse the entire input, all of which can be derived from the start
//symbol
void parser_t::parse()
{
    fileout=fopen("output.c", "w");
    if(fileout==NULL)
        perror("Error writing to 'output.c'\n");
    fprintf(fileout, "#include <stdio.h>\n#include <stdlib.h>\n\n");
    fprintf(fileout, "int main()\n{\n");
    fprintf(fileout, "\tint addr;\n\tint* m;\n");
    scanner.read();
    Start();
    fprintf(fileout, "\n\treturn 0;\n}\n");
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
		default:
            Statements();
			break;
	}

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here
void parser_t::Statements()
{
    parsetree.push(NT_Statements);
    switch (scanner.next_token())
    {
        case T_eof:
            parsetree.drawepsilon();
            break;
        default:
            Statement(); Statements();
    }
    parsetree.pop();
}
void parser_t::Statement()
{
    parsetree.push(NT_Statement);
    switch (scanner.next_token())
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
    }
    parsetree.pop();
}
void parser_t::Label()
{
    parsetree.push(NT_Label);
    scanner.eat_token(T_label);
    scanner.eat_token(T_num);
    fprintf(fileout, "\tl%i", scanner.get_number());
    scanner.eat_token(T_colon);
    fprintf(fileout, ":\n");
    parsetree.pop();
}
void parser_t::Jump()
{
    parsetree.push(NT_Jump);
    scanner.eat_token(T_goto);
    scanner.eat_token(T_num);
    if(scanner.next_token()==T_if)
    {
        scanner.eat_token(T_if);
        int i = scanner.get_number();
        fprintf(fileout, "\tif(");
        Expression();
        fprintf(fileout, " != 0)");
        fprintf(fileout, "\t\tgoto l%i;\n", i);
    } else {
        fprintf(fileout, "\tgoto l%i;\n", scanner.get_number());
    }
    parsetree.pop();
}
void parser_t::Assignment()
{
    parsetree.push(NT_Assignment);
    scanner.eat_token(T_m);
    scanner.eat_token(T_opensquare);
    fprintf(fileout, "\tm[ addr = ( ");
    Expression();
    scanner.eat_token(T_closesquare);
    scanner.eat_token(T_equals);
    fprintf(fileout, ") ] = ");
    Expression();
    fprintf(fileout, ";\n");
    parsetree.pop();
}
void parser_t::Print()
{
    parsetree.push(NT_Print);
    scanner.eat_token(T_print);
    fprintf(fileout, "\tprintf(");
    fputc('"', fileout);
    fputc('%', fileout);
    fputc('i', fileout);
    fputc('\\', fileout);
    fputc('n', fileout);
    fputc('"', fileout);
    fprintf(fileout, ", ( ");
    Expression();
    fprintf(fileout, "));\n");
    parsetree.pop();
}
void parser_t::Expression()
{
    parsetree.push(NT_Expression);
    switch(scanner.next_token())
    {
        case T_openparen:
        case T_num:
        case T_m:
            Term(); Expr();
            break;
        default:
            syntax_error(NT_Expression);
    }
    parsetree.pop();
}
void parser_t::Expr()
{
    parsetree.push(NT_Expr);
    switch(scanner.next_token())
    {
        case T_plus:
            scanner.eat_token(T_plus);
            fprintf(fileout, "+ ");
            Term(); Expr();
            break;
        case T_minus:
            scanner.eat_token(T_minus);
            fprintf(fileout, "- ");
            Term(); Expr();
            break;
        default:
            parsetree.drawepsilon();
            break;
    }
    parsetree.pop();
}
void parser_t::Term()
{
    parsetree.push(NT_Term);
    switch(scanner.next_token())
    {
        case T_openparen:
        case T_num:
        case T_m:
            Factor(); Ter();
            break;
        default:
            syntax_error(NT_Term);
    }
    parsetree.pop();
}
void parser_t::Ter()
{
    parsetree.push(NT_Ter);
    switch(scanner.next_token())
    {
        case T_times:
            scanner.eat_token(T_times);
            fprintf(fileout, "* ");
            Factor(); Ter();
            break;
        case T_divide:
            scanner.eat_token(T_divide);
            fprintf(fileout, "/ ");
            Factor(); Ter();
            break;
        default:
            parsetree.drawepsilon();
            break;
    }
    parsetree.pop();
}
void parser_t::Factor()
{
    parsetree.push(NT_Term);
    switch(scanner.next_token())
    {
        case T_openparen:
        case T_num:
        case T_m:
            Number();
            if(scanner.next_token()==T_power)
            {
                scanner.eat_token(T_power);
                fprintf(fileout, "^ ( ");
                Expression();
                fprintf(fileout, ") ");
            }
            break;
        default:
            syntax_error(NT_Factor);
    }
    parsetree.pop();
}
void parser_t::Number()
{
    parsetree.push(NT_Term);
    switch(scanner.next_token())
    {
        case T_openparen:
            scanner.eat_token(T_openparen);
            fprintf(fileout, "( ");
            Expression();
            scanner.eat_token(T_closeparen);
            fprintf(fileout, ") ");
            break;
        case T_num:
            scanner.eat_token(T_num);
            fprintf(fileout, "%i ", scanner.get_number());
            break;
        case T_m:
            scanner.eat_token(T_m);
            scanner.eat_token(T_opensquare);
            fprintf(fileout, "m[ addr = ( ");
            Expression();
            scanner.eat_token(T_closesquare);
            fprintf(fileout, ") ] ");
            break;
        default:
            syntax_error(NT_Number);
    }
    parsetree.pop();
}


/*** Main ***********************************************/

int main()
{
    //DEBUG: instantiate a scanner and dump the tokens it finds
    //scanner_t* scanner = new scanner_t();
    //fprintf(stderr, "Tokens found in file:\n");
    //scanner->print();


    parser_t parser;
    parser.parse();
	return 0;
}
