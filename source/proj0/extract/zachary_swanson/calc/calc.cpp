//Zachary Swanson. CS160 First Project.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
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
    NT_States,
    NT_State,
    NT_States_Prime,
    NT_Label,
    NT_Jump,
    NT_Assignment,
    NT_Print,
    NT_Jump_Prime,
	NT_Expression,
    NT_T,
    NT_E_Prime,
    NT_T_Prime,
    NT_G,
    NT_G_Prime,
    NT_F
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
        //WRITEME: add the other nonterminals you need here
        case NT_States: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
        case NT_State: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
        case NT_States_Prime: strncpy(buffer,"Statements'",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Jump_Prime: strncpy(buffer,"Jump'",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
        case NT_T: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
        case NT_E_Prime: strncpy(buffer,"Expression'",MAX_SYMBOL_NAME_SIZE); break;
        case NT_T_Prime: strncpy(buffer,"Term'",MAX_SYMBOL_NAME_SIZE); break;
        case NT_G: strncpy(buffer,"G",MAX_SYMBOL_NAME_SIZE); break;
        case NT_G_Prime: strncpy(buffer,"G'",MAX_SYMBOL_NAME_SIZE); break;
        case NT_F: strncpy(buffer,"F",MAX_SYMBOL_NAME_SIZE); break;
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
    
    //get the most recently eaten number
    int get_num();

	//constructor 
	scanner_t();
    
    //flag to say if EOF has been hit
    bool EOF_hit;

  private:
    
    //keeps track of the current line (Number of newlines eaten by next_token)
    int line_num;
    
    //stores teh most recently eaten number
    int next_num;

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
    //made my own error message for num out of range
    void range_error (float d);

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	token_type bogo_token;

};

//helper function
//returns true is the character represented by the integer c is a numeric digit
bool is_num(int c)
{
    switch(c)
    {
        case '0': return true;
        case '1': return true;
        case '2': return true;
        case '3': return true;
        case '4': return true;
        case '5': return true;
        case '6': return true;
        case '7': return true;
        case '8': return true;
        case '9': return true;
        default: return false;
    }
}

//gets the most recently eaten number
int scanner_t::get_num()
{
    return next_num;
}

//returns the next token in the input stream without eating it
token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
    
    
    //first check for eof
    if(EOF_hit == true)
    {
       return T_eof;
    }
    
    //read the next character into c. then replace it into the input stream
    int c = getchar();
    cin.putback(c);
    
    //need to figure out which type of token c is
    switch(c)
    {
        //eat spaces
        case ' ':
            getchar();
            return next_token();
        case EOF: 
        {
            EOF_hit = true;
            return T_eof;
        }
        case '+': return T_plus;
        case '-': return T_minus;
        case '*':
        {
            //check if this is a power token
            int c1 = getchar();
            int c2 = getchar();
            if(c2 == '*')
            {
                cin.putback(c2);
                cin.putback(c1);
                return T_power;
            }
            else
            {
                cin.putback(c2);
                cin.putback(c1);
                return T_times;
            }
        }
        case '/': return T_divide;
        case '=': return T_equals;
        case '(': return T_openparen;
        case ')': return T_closeparen;
        case '[': return T_opensquare;
        case ']': return T_closesquare;
        case 'm': return T_m;
        case 'g':
        {
            //make sure all the characters in goto are present
            int c1 = getchar();
            int c2 = getchar();
            int c3 = getchar();
            int c4 = getchar();
            if(c1 == 'g' && c2 == 'o' && c3 == 't' && c4 == 'o')
            {
                cin.putback(c4);
                cin.putback(c3);
                cin.putback(c2);
                cin.putback(c1);
                return T_goto;
            }
            else
            {
                cin.putback(c4);
                cin.putback(c3);
                cin.putback(c2);
                cin.putback(c1);
                //error
                scan_error(c);
                return bogo_token;
            }
        }
        case 'i':
        {
            //make sure all the characters in if are present
            int c1 = getchar();
            int c2 = getchar();
            if(c2 == 'f')
            {
                cin.putback(c2);
                cin.putback(c1);
                return T_if;
            }
            else
            {
                cin.putback(c2);
                cin.putback(c1);
                //error
                scan_error(c2);
                return bogo_token;
            }
        }
        case 'p': 
        {
            //make sure all the characters in print are present
            int c1 = getchar();
            int c2 = getchar();
            int c3 = getchar();
            int c4 = getchar();
            int c5 = getchar();
            if(c1 == 'p' && c2 == 'r' && c3 == 'i' && c4 == 'n' && c5 == 't')
            {
                cin.putback(c5);
                cin.putback(c4);
                cin.putback(c3);
                cin.putback(c2);
                cin.putback(c1);
                return T_print;
            }
            else
            {
                cin.putback(c5);
                cin.putback(c4);
                cin.putback(c3);
                cin.putback(c2);
                cin.putback(c1);
                //error
                scan_error(c);
                return bogo_token;
            }
        }
        case 'L': return T_label;
        case ':': return T_colon;
        //eat newlines
        case '\n': 
            getchar();
            line_num++;
            return next_token();
        default: //check if it is a num, if its not then return an error
        {
            if(is_num(c))
            {
                char the_num[25];
                char C = getchar();
                int i = 0;
                while(is_num(C))
                {
                    the_num[i] = C;
                    C = getchar();
                   
                    i++;
                }
                cin.putback(C);//putback the last character since we determined it wasnt a  number
                the_num[i] = '\0';
                int len = strlen(the_num);
               
                //push those characters back on to stdin since we dont want to have eaten the number
                for(i = len - 1; i >=0 ; i--)
                {
                    cin.putback(the_num[i]);
                }
                
                double the_outcome = atof(the_num);
                
                //check the range of the int
                if((the_outcome >= 0.0) && (the_outcome <= 2147483647.0))
                    return T_num;
                else
                {
                    //out of range? what error type to return?
                    range_error(the_outcome);
                    return bogo_token;
                }
            }
            else
            {
                scan_error(c);
                return bogo_token;
            }
                
        }
    }
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )
    //checking for a match doesnt work right for EOF, so dont check. it seems to get permanently eaten even if i use cin.putback(EOF) after peeking at it
    if(c != T_eof)
    {
        //check for mismatch errors
        if(c != next_token())
        {
            mismatch_error(c);
        }
        //else eat the correct number or characters from the stream
        else
        {
            
            switch(c)
            {
                case T_power:
                    getchar();
                    getchar();
                    break;
                case T_if:
                    getchar();
                    getchar();
                    break;
                case T_goto:
                    getchar();
                    getchar();
                    getchar();
                    getchar();
                    break;
                case T_print:
                    getchar();
                    getchar();
                    getchar();
                    getchar();
                    getchar();
                    break;
                case T_num:
                {

                    char the_num[25];
                    char C = getchar();
                    int i = 0;
                    while(is_num(C))
                    {
                        the_num[i] = C;
                        C = getchar();
                        i++;
                    }
                    cin.putback(C);
                    the_num[i] = '\0';
                    //int len = strlen(the_num);
                    
                    //set the eaten number into next_num so that the parser can grab it
                    int the_outcome = atoi(the_num);
                    next_num = the_outcome;
                    break;
                }
                //for all other cases we eat just one character
                default:
                    getchar();
                    break;
            }
        }
    }
    else
    {
        if(EOF_hit == true)
            return;
    }
    return;
}

//scanner constructor
scanner_t::scanner_t()
{
    line_num = 1;
    EOF_hit = false;
}

//returns the number of newlines (+1) that the scanner has seen
int scanner_t::get_line()
{
    return line_num;
}

void scanner_t::scan_error (char x)
{
	printf("scan error: unrecognized character '%c' -line %d\n",x, get_line());
	exit(1);

}

void scanner_t::range_error (float d)
{
	printf("range error: '%f' -line %d\n", d, get_line());
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
    void eat(token_type t);

	void Start();
    string match(nonterm_type);
    bool isValidFirst(nonterm_type nt, token_type t);

  public:	
	void parse();
};

//to be called for those nonterminals that have epsilon transformations
bool parser_t::isValidFirst(nonterm_type nt, token_type t)
{
    switch(nt)
    {
        case(NT_G_Prime):
        {
            if(t == T_plus || t == T_minus || t == T_closeparen || t == T_closesquare || t == T_eof || t == T_times || T_divide || T_power ||
               t == T_label || t == T_print || t == T_goto || t == T_m)
            {
                return true;
            }
            else return false;
        }
        case(NT_T_Prime):
        {
            if(t == T_plus || t == T_minus || t == T_closeparen || t == T_closesquare || t == T_eof || t == T_times || T_divide ||
               t == T_label || t == T_print || t == T_goto || t == T_m)
            {
                return true;
            }
            else return false;
        }
        case(NT_E_Prime):
        {
            if(t == T_plus || t == T_minus || t == T_closeparen || t == T_closesquare || t == T_eof ||
               t == T_label || t == T_print || t == T_goto || t == T_m)
            {
                return true;
            }
            else return false;
        }
        case(NT_Jump_Prime):
        {
            if(t == T_eof || t == T_label || t == T_print || t == T_goto || t == T_m || t == T_if)
            {
                return true;
            }
            else return false;
        }
        case(NT_States_Prime):
        {
            if(t == T_eof || t == T_label || t == T_print || t == T_goto || t == T_m)
            {
                return true;
            }
            else return false;
        }
        default:
        {
            //should be impossible
            return true;
        }
    }
}

//this function not only eats the token (moving the scanner forward one
//token), it also makes sure that token is drawn in the parse tree 
//properly by calling push and pop.
void parser_t::eat_token(token_type t)
{
	parsetree.push(t);
	scanner.eat_token(t);
	parsetree.pop();
}

//same as the above function, but quicker to type
void parser_t::eat(token_type t)
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

//recursive function to match the next expected expresion, eat whatever tokens should be, build the parsetree, and output teh genereated c code to stderr
//the function returns strings in order to deal with writing the power function in c code. since we dont know if we are doing exponentiation until after the first argument is complete
string parser_t::match(nonterm_type nt)
{
    parsetree.push(nt);
    switch(nt)
    {
        case NT_Start:
        {
            match(NT_States);
            parsetree.pop();
            return "";
        }
        case NT_States:
        {
            //States -> State States'
            match(NT_State);
            //kill getline
            match(NT_States_Prime);
            parsetree.pop();
            return "";
        }
        case NT_States_Prime:
        {
			token_type t = scanner.next_token();
            //kill getline
            if(!isValidFirst(nt, t)) syntax_error(nt);
            //States' -> State States'
            if(t != T_eof)
            {
                match(NT_State);
                match(NT_States_Prime);
                parsetree.pop();
                return "";
            }
            //States' -> epsilon
			else
			{
           		parsetree.drawepsilon();
                parsetree.pop();
                return "";
			}
            break;
        }
        case NT_State:
        {
            token_type t = scanner.next_token();
            switch(t)
            {
                //-> Label
                case T_label:
                {
                    cerr << match(NT_Label) << endl;
                    parsetree.pop();
                    return "";
                }
                //-> Jump
                case T_goto:
                {
                    cerr << match(NT_Jump) << endl;
                    parsetree.pop();
                    return "";
                }
                //-> Assignment
                case T_m:
                {
                    cerr << match(NT_Assignment) << endl;
                    parsetree.pop();
                    return "";
                }
                //->Print
                case T_print:
                {
                    cerr << match(NT_Print) << endl;
                    parsetree.pop();
                    return "";
                }
                //error
                default: 
                {
                    syntax_error(nt);
                    parsetree.pop();
                    return "";
                }
                
            }
            break;
        }
        case NT_Label:
        {
            //-> L number :
            eat(T_label);
            
            eat(T_num);
            int num = scanner.get_num();
            
            eat(T_colon);
            char numstr[21]; 
            sprintf(numstr, "%i", num);
            string str(numstr);

            parsetree.pop();
            return "L"+str+":\n";
        }
        case NT_Jump:
        {
            //-> goto number J2
            eat(T_goto);
            eat(T_num);
            string str = match(NT_Jump_Prime);
            parsetree.pop();
            return str;
        }
        case NT_Assignment:
        {
            //-> m[Expression] = Expression
            eat(T_m);
            eat(T_opensquare);
            string nt_exp1 = match(NT_Expression);   
            eat(T_closesquare);
            eat(T_equals); 
            string nt_exp2 = match(NT_Expression);
            parsetree.pop();
            return " m["+nt_exp1+"] = "+nt_exp2+";\n";
        }
        case NT_Print:
        {
            //-> "print" Expression
            eat(T_print);
            
            string s = match(NT_Expression);
            parsetree.pop();
            return "printf(\"%i\\n\","+s+");\n";
        }
        case NT_Jump_Prime:
        {
            token_type t = scanner.next_token();
            if(!isValidFirst(nt, t)) syntax_error(nt);
            //-> "if" Expression
            if(t == T_if)
            {
                eat(T_if);
                
                int num = scanner.get_num();
                char numstr[21]; 
                sprintf(numstr, "%i", num);
                
                string s = match(NT_Expression);
                parsetree.pop();
                return "if("+s+") goto L"+numstr+";\n";
            }
            //-> epsilon
            else
            {
                int num = scanner.get_num();
                
                char numstr[21]; 
                sprintf(numstr, "%i", num);
                string str(numstr);
                
                parsetree.drawepsilon();
                parsetree.pop();
                return "goto L"+str+";\n";
            }
            break;
        }
        case NT_Expression:
        {
            //->T E'
            string nt_str = match(NT_T);
            string nt_e_p = match(NT_E_Prime);
            parsetree.pop();
            return nt_str + nt_e_p;
        }
        case NT_E_Prime:
        {
            token_type t = scanner.next_token();
            if(!isValidFirst(nt, t)) syntax_error(nt);
            //-> + T E'
            if(t == T_plus)
            {
                eat(T_plus);
                
                string s1 = match(NT_T);
                string s2 = match(NT_E_Prime);
                parsetree.pop();
                return " + " + s1 + s2;
            }
            //-> - T E'
            else if(t == T_minus)
            {
                eat(T_minus);
                
                string s1 = match(NT_T);
                string s2 = match(NT_E_Prime);
                parsetree.pop();
                return " - " + s1 + s2;
            }
            //-> epsilon
            else
            {
                parsetree.drawepsilon();
                parsetree.pop();
                return "";
            }
            break;
        }
        case NT_T:
        {
            //-> G T'
            string s1 = match(NT_G);
            string s2 = match(NT_T_Prime);
            parsetree.pop();
            return s1 + s2;
        }
        case NT_T_Prime:
        {
            token_type t = scanner.next_token();
            if(!isValidFirst(nt, t)) syntax_error(nt);
            //-> * G T'
            if(t == T_times)
            {
                eat(T_times);
                
                string s1 = match(NT_G);
                string s2 = match(NT_T_Prime);
                parsetree.pop();
                return " * " + s1 + s2;
            }
            //-> / G T'
            else if(t == T_divide)
            {
                eat(T_divide);
                
                string s1 = match(NT_G);
                string s2 = match(NT_T_Prime);
                parsetree.pop();
                return " / " + s1 + s2;
            }
            //-> epsilon
            else
            {
                parsetree.drawepsilon();
                parsetree.pop();
                return "";
            }
            break;
        }
        case NT_G:
        {
            //->F G'
            //store the products of F to a buffer so that we can decide where to put them when we expand G_Prime
            string f = match(NT_F);
            string g_prime = match(NT_G_Prime);
            
            //i put these two characters (**) at teh start of the string returned by
                //match(NT_G_Prime) if it is doing an exponentiation so that i know if i should do pow or not or not
            if(g_prime.length() >= 2 && g_prime[0] == '*' && g_prime[1] == '*')
            {
                //strip the 2 flag characters off
                g_prime.erase(0,2);
                parsetree.pop();
                return "(int)pow("+f+","+g_prime+")";
            }
            else
            {
                parsetree.pop();
                return f;
            }
            
            //break;
        }
        case NT_G_Prime:
        {
            token_type t = scanner.next_token();
            if(!isValidFirst(nt, t)) syntax_error(nt);
            //if power then we should put some flag characters at the front of the string so that our above recursive call can identify it we took the power transfomration or the epsilon transformation
            if(t == T_power)
            {
                eat(T_power);
                string s = match(NT_G);
                parsetree.pop();
                return "**"+ s;
                
            }
            //-> epsilon
            //if epsilon then we should output the buffer to stderr now, and clear the buffer
            else
            {
                parsetree.drawepsilon();
                parsetree.pop();
                return "";
            }
            break;
        }
        case NT_F:
        {
            token_type t = scanner.next_token();
            //-> m[Expression]
            if(t == T_m)
            {
                eat(T_m);
                
                eat(T_opensquare);
                
                string nt_match = match(NT_Expression);
                eat(T_closesquare);
                parsetree.pop();
                return "m["+nt_match+"]";
            }
            //-> (Expression)
            else if(t == T_openparen)
            {
                eat(T_openparen);
                
                string nt_match = match(NT_Expression);
                eat(T_closeparen);
                    
                parsetree.pop();
                return "("+nt_match+")";
            }
            //-> num
            else if(t == T_num)
            {
                eat(T_num);
                int num = scanner.get_num();
                
                char numstr[21]; 
                sprintf(numstr, "%i", num);
                string str(numstr);
                parsetree.pop();
                return str;
            }
            //error
            else
            {

                syntax_error(nt);
            }
            break;
        }
        //should be impossible
        default:
        {
            //cout << "unknown token" << endl;
            break;
        }
    }
    parsetree.pop();
    return "";
}

//starts teh recursive descent
void parser_t::Start()
{
    
    match(NT_Start);
}

//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

int main()
{
    fprintf(stderr, "#include <stdio.h> \n #include \"math.h\" \n");
    fprintf(stderr, "int main()\n { \n int m[101]; \n");
	parser_t parser;
	parser.parse();
    fprintf(stderr, "\n return 0; \n }");
	return 0;
    
}
