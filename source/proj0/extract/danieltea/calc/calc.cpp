#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
//#include <iostream>


// these defines are used to check for valid tokens later. an extra character
// is added to consider the nul ('\0') character
#define PRINT_BUFFER 6
#define GOTO_BUFFER 5
#define IF_BUFFER 3 
#define POWER_BUFFER 3
#define MAX_INT_BUFFER 10

using namespace std;


bool DEBUG = 1;


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
	NT_Start, // 1
    NT_Statements, // 2
    NT_Statement, // 3

	//WRITEME: add symbolic names for you non-terminals here
    NT_Label, // 4
    NT_Jump, // 5    
    NT_Assignment, // 6
    NT_Print, // 7
    NT_Expression, //8
    NT_R,
    NT_E1
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

		  //WRITEME: add the other nonterminals you need here
		  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
  		  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;		
          case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
          case NT_R: strncpy(buffer,"R",MAX_SYMBOL_NAME_SIZE); break;
          case NT_E1: strncpy(buffer,"E1",MAX_SYMBOL_NAME_SIZE); break;

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

    // constructor with program as parameter
    //scanner_t( input stream string );

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  
    bool is_digit(int c);
    bool is_valid_integer(char *c);
    bool another_token(int c);
    bool is_space(int c);
    bool is_single(int c);



	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

    // error message and exit for integer overflow
    void integer_error();

    // error message and exit for range error
    void range_error();

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	token_type bogo_token;


	token_type token;
    int test_token;
    int line_number;

};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 

    //printf("\n SCANNING NOW... \n");


    test_token = getchar();
    char *keyword;
    char *number;

    //printf("checking: (%c)\n", test_token);


    // loop until the next non-whitespace character is found
    while(is_space(test_token))
    {
    	//printf("  space or newline encountered\n");
        //printf("  line count is now: %i\n", get_line());

    	test_token = getchar();
    }


    // if test_token is a digit, start building a number token
    if(is_digit(test_token))
    {
    	//printf("consumed?: (%i)\n", test_token-'0');
    	//ungetc(test_token, stdin);    	

    	int t = getchar();

        // is it a single digit?
        if(is_single(t) || another_token(t))
        {
        	//printf("single digit line num: %i\n", get_line());
        	ungetc(t, stdin);
        	ungetc(test_token, stdin);
        	return T_num;
        }
 
        number = new char[MAX_INT_BUFFER];
        number[0] = test_token;
    	int count = 1;
        while(is_digit(t))
        {
        	if(count >= MAX_INT_BUFFER)
        	{
        		integer_error();
        		break;
            }
        	number[count] = t;
        	t = getchar();
        	count++;
            //printf("QQQ: %i\n", t - '\0');

        }
        // put the last character read back to the stream, since it is not a digit
        ungetc(t, stdin);

        //printf("the number in string form: %s\n", number);
        int number_int = atoi(number);
    	int length = strlen(number);
    	//printf("the length of the string is: %i\n", length);
        //printf("the number in integer form: %i\n", number_int);

        if(length > 9)
        {
            if(!is_valid_integer(number))
            {
                delete [] number;
                integer_error();
            }
        }

       
        // add the integer we read back into stdin by looping through
        // the end of the character array and sticking each value back into
        // the stream
        //ungetc('\0', stdin);
        for(int i=length-1; i>=0; i--)
        {
        	//printf("adding %c into stdin\n", number[i]);
            ungetc(number[i], stdin);
        }
        delete [] number;
    	return T_num;
    }

    switch(test_token)
    {
    	case '+':
   	    	//printf("consumed: %s\n", token_to_string(T_plus));
    	    ungetc(test_token, stdin);    	
    	    return T_plus;
    	    break;

    	case '-':
    	    //printf("consumed: %s\n", token_to_string(T_minus));
    	    ungetc(test_token, stdin);
    	    return T_minus;
    	    break;

    	case '*':
    	    //printf("consumed: %s\n", token_to_string(T_times));

            // check if it is the POWER operator
            int p;
            p = getchar();
            if(p == '*')
            {
                //printf("consumed: %s\n", token_to_string(T_times));
        	    ungetc(p, stdin);
        	    ungetc(test_token, stdin);
                return T_power;
            }

            ungetc(p, stdin);
    	    ungetc(test_token, stdin);
    	    return T_times;
    	    break;

    	case '/':
    	    //printf("consumed: %s\n", token_to_string(T_divide));
    	    ungetc(test_token, stdin);
    	    return T_divide;
    	    break;

    	case '=':
            //printf("consumed: %s\n", token_to_string(T_equals));
            ungetc(test_token, stdin);
    	    return T_equals;
    	    break;

    	case 'm':
    	    //printf("consumed: %s\n", token_to_string(T_m));
    	    ungetc(test_token, stdin);
    	    return T_m;
    	    break;
        
        case '[':
      	    //printf("consumed: %s\n", token_to_string(T_opensquare));
    	    ungetc(test_token, stdin);      	
      	    return T_opensquare;     
      	    break;

        case ']':
      	    //printf("consumed: %s\n", token_to_string(T_closesquare));
      	    ungetc(test_token, stdin);
      	    return T_closesquare;
      	    break;

        case '(':
            //printf("consumed: %s\n", token_to_string(T_openparen));
            ungetc(test_token, stdin);      	
      	    return T_openparen;
      	    break;

        case ')':
      	    //printf("consumed: %s\n", token_to_string(T_closeparen));
            ungetc(test_token, stdin);      	
      	    return T_closeparen;  
      	    break;      

        case 'L':
            //printf("consumed: %s\n", token_to_string(T_label));
            ungetc(test_token, stdin);      	      	
      	    return T_label;
      	    break;

        case ':':
            //rintf("consumed: %s\n", token_to_string(T_colon));
            ungetc(test_token, stdin);      	
    	    return T_colon;
    	    break;
        
        case 'i':
            // first put that 'i' back into stdin
    	    ungetc(test_token, stdin);

    	    // build the buffer
    	    keyword = new char[IF_BUFFER];

    	    fgets(keyword, IF_BUFFER, stdin);
    	    //printf("%s\n", keyword);
	    	if(strncmp(keyword, "if", IF_BUFFER) == 0)
	    	{
	    		//ungetc('\0', stdin);
	            ungetc('f', stdin);
	            ungetc('i', stdin);
	    	}
	    	else
	    	{
	            // NEED TO PRINT CORRECT ERROR MESSAGE
	        	//mprintf("asdtoken not recognized\n");
	        	exit(1);
	    	}

	    	delete [] keyword;
	    	return T_if;   
	    	break;
        
        
        case 'g':
            ungetc(test_token, stdin);

	        // build the buffer!
	        keyword = new char[GOTO_BUFFER];


	        // then check if the next token is actually 'goto' or not
	        fgets(keyword, GOTO_BUFFER, stdin);
	        if(strncmp(keyword, "goto", GOTO_BUFFER) == 0)
	        {
	            //ungetc('\0', stdin);
	            ungetc('o', stdin);
	            ungetc('t', stdin);
	            ungetc('o', stdin);
	            ungetc('g', stdin);
	        }
	        else
	        {
	        	// NEED TO PRINT CORRECT ERROR MESSAGE
	        	//printf("ggtoken not recognized\n");
	        	exit(1);
	        }
       
            delete [] keyword;
            return T_goto;
            break;
        
        case 'p':
            ungetc(test_token, stdin);

	    	keyword = new char[PRINT_BUFFER];

	    	fgets(keyword, PRINT_BUFFER, stdin);
	    	if(strncmp(keyword, "print", PRINT_BUFFER) == 0)
	    	{
	    		//ungetc('\0', stdin);
	            ungetc('t', stdin);
	            ungetc('n', stdin);
	    		ungetc('i', stdin);
	            ungetc('r', stdin);
	            ungetc('p', stdin);            
	    	}
	    	else
	    	{
	    		// NEED TO PRINT CORRECT ERROR MESSAGE
	        	//printf("asdptoken not recognized\n");
	        	exit(1);
	    	}

	        delete [] keyword;
	        return T_print;
	        break;
        
        case -1:
    	    //printf("reached end of file.\n");
    	    return T_eof;
    	    break;
        
        default:
            scan_error(test_token);
            return T_eof;
            break;        	
    }

    //scan_error(test_token);
    //return T_eof; // should not be called since we called scan_error, which exits

}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

    //printf("\n EATING NOW... \n");
    int eat;

    eat = getchar();

    // lame way to avoid reading whitespace and newlines without
    // incrementing the line count
    while(is_single(eat))
    {
    	eat = getchar();
    }

    //mprintf("token to eat: %s\n", token_to_string(c));
    switch(c)
    {
		case T_num:
		    // if the first character is not a digit, return a mismatch error.
		    // otherwise, keep eating the characters that are digits.
		    //printf("the first number is: %i\n", eat-'0');
	    	if(!is_digit(eat))
	    	{
	    		ungetc(eat, stdin);
	    		mismatch_error(T_num);
	        }
	        else
	        {
	        	//eat = getchar(); // grab the next char
	    		//printf(" @@@@: %i\n", eat);
                while(is_digit(eat))
                {
                    //printf("loop read: %i\n", eat-'0');

                	eat = getchar();
                }

 
                // put the last character that is NOT a digit back to stdin
                ungetc(eat, stdin);
	        }

	        break;

		case T_plus:
            if(eat != '+')
            {
                ungetc(eat, stdin);
                mismatch_error(T_plus);
            }
            break;

		case T_minus:
            if(eat != '-')
            {
                ungetc(eat, stdin);
                mismatch_error(T_minus);
            }
            break;

		case T_times:
            if(eat != '*')
            {
                ungetc(eat, stdin);
                mismatch_error(T_times);
            }
            break;

		case T_divide:
            if(eat != '/')
            {
                ungetc(eat, stdin);
                mismatch_error(T_divide);
            }
            break;

		case T_power:
            if(eat != '*')
    	    {
    		    ungetc(eat, stdin);
    		    mismatch_error(T_power);
            }

            eat = getchar();
            if(eat != '*')
        	    mismatch_error(T_power);
             break;

		case T_equals:
		    if(eat != '=')
    	    {   
    		    ungetc(eat, stdin);
    		    mismatch_error(T_equals);
            }
            break;

		case T_openparen:
            if(eat != '(')
            {
                ungetc(eat, stdin);
                mismatch_error(T_openparen);
            }
            break;

		case T_closeparen:
            if(eat != ')')
            {
                ungetc(eat, stdin);
                mismatch_error(T_closeparen);
            }
            break;


		case T_opensquare:
		    if(eat != '[')
    	    {
    		    ungetc(eat, stdin);
    		    mismatch_error(T_opensquare);
            }
            break;

		case T_closesquare:
	    	if(eat != ']')
	    	{
	    		ungetc(eat, stdin);
	    		mismatch_error(T_closesquare);
	        }
	        break;

		case T_m:
    	    if(eat != 'm')
    	    {
    		    ungetc(eat, stdin);
    		    mismatch_error(T_m);
            }
            break;

		case T_goto:
	    	// consume 'g' 'o' 't' 'o' '\0'
	    	if(eat != 'g')
	    		mismatch_error(T_goto);

	        eat = getchar();
	    	if(eat != 'o')
	    		mismatch_error(T_goto);

	        eat = getchar();
	    	if(eat != 't')
	    		mismatch_error(T_goto);

	        eat = getchar();
	    	if(eat != 'o')
	    		mismatch_error(T_goto);

	        //eat = getchar();
	    	//if(eat != '\0')
	    	//	mismatch_error(T_goto);
	        //printf("consumed: goto\n");  		
	        break;

		case T_if:
	    	// consume 'i' 'f' '\0'
	    	if(eat != 'i')
	    		mismatch_error(T_if);

	        eat = getchar();
	    	if(eat != 'f')
	    		mismatch_error(T_if);

	        //eat = getchar();
	    	//if(eat != '\0')
	    	//	mismatch_error(T_if);
	        //printf("consumed: if\n"); 
	        break;

		case T_print:
	    	// consume 'p' 'r' 'i' 'n' 't' '\0'
	    	if(eat != 'p')
	    		mismatch_error(T_print);

	        eat = getchar();
	    	if(eat != 'r')
	    		mismatch_error(T_print);

	        eat = getchar();
	    	if(eat != 'i')
	    		mismatch_error(T_print);

	        eat = getchar();
	    	if(eat != 'n')
	    		mismatch_error(T_print);

	        eat = getchar();
	    	if(eat != 't')
	    		mismatch_error(T_print);

	        //eat = getchar();
	    	//if(eat != '\0')
	    	//	mismatch_error(T_print);

	        //printf("consumed: print\n");  
	        break;

		case T_label:
            if(eat != 'L')
            {
                ungetc(eat, stdin);
                mismatch_error(T_label);
            }
            break;

		case T_colon:
            if(eat != ':')
            {
                ungetc(eat, stdin);
                mismatch_error(T_colon);
            }
            break;	

		default:
		    printf("some kind of error while eating\n");
		    break;

    }


    //printf(" EAT SUCCESSFUL \n");
	//WRITEME: cut this bogus stuff out and implement eat_token
	//printf("%i\n ", rand()%10 < 8);
	if ( rand()%100 < 50 ) 
		bogo_token = T_plus;
	else 
		bogo_token = T_eof;

}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	line_number = 1;
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting

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

void scanner_t::integer_error()
{
	printf("invalid integer was read in\n");
	exit(1);
}

void scanner_t::range_error()
{
	printf("error: integer out of range \n");
	exit(1);
}



// returns true if the char is a digit
bool scanner_t::is_digit(int c)
{
	if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' ||
	   c == '6' || c == '7' || c == '8' || c == '9') 
	{
        return true;
	}
	else
	{
		return false;
	}
}


// valid integer range should be from [0, 2^31-1 = 2,147,483,647]
// since 2147483647 is 10 digits, check if the length of the string is greater
// than 10. if so, the string is already greater than the max value. 
// this is only called when the scanner reads in integer of length 10 or
// greater
// note: this is a super lame way to check the validitiy of the string
bool scanner_t::is_valid_integer(char *c)
{
    int length = strlen(c);
    char max_val[MAX_INT_BUFFER+1] = "2147483647";
    if(length > 10)
    	return false;
    else
    {
    	for(int i=0; i<11; i++)
    	{
            if(c[i]-'0' < max_val[i]-'0')
                return true;
    	    else if(c[i]-'0' > max_val[i]-'0')
    	    	return false;
    	    else
    	    	continue; // we need more testing
    	}
    }

    return true;
}


// returns true if the char is a space. increments the line number
// if a newline is encountered
bool scanner_t::is_space(int c)
{
	if(c == '\n')
	{
		line_number++;
		return true;
	}

	if(c == ' ') 
        return true;
	else
		return false;
}


bool scanner_t::is_single(int c)
{
	if(c == '\n' || c == ' ' || c == -1)
		return true;
	else
		return false;
}


// used for helping figure out valid numbers. checks if the character
// right after an integer would be valid or not. this is used so that 
// the program can decide whether the digit is a single-digit integer or an
// n-digit integer 
// num: is valid
// num+ is valid
// num- is valid
// num* is valid
// num/ is valid
// num] is valid
// num) is valid
bool scanner_t::another_token(int c)
{
	if(c == '+')
		return true;
	if(c == '-')
		return true;
	if(c == '*')
		return true;
	if(c == '/')
		return true;
	if(c == ']')
		return true;
	if(c == ')')
		return true;
	if(c == ':')
		return true;
	else
		return false;
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
		printf( "}\n" ); // note: i removed the semicolon after the } to get rid
		                 // of the warning when compiling with g++
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
	void Label();
	void Jump();
	void Assignment();
	void Print();
	void Expression();

    // special methods to ensure LL(1)
    void R();
    void E1();

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
void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);

	switch( scanner.next_token() ) 
	{
		//case T_plus:
			//eat_token(T_plus);
			//Start();
			//break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
		    Statements();
			//syntax_error(NT_Start);
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


    Statement();
    R();

    parsetree.pop();
}

void parser_t::Statement()
{
    parsetree.push(NT_Statement);
    switch(scanner.next_token())
    {
    	case T_m:
    	    Assignment();
    	    break;

        case T_label:
            Label();
            break;

        case T_print:
            Print();
            break;

        case T_goto:
            Jump();
            break;

    	default:
    		syntax_error(NT_Start);
    		break;

    }
	parsetree.pop();
}


void parser_t::R()
{
    parsetree.push(NT_R);
	//printf("token: %s\n", token_to_string(scanner.next_token()));

    if(scanner.next_token() != T_eof)
    {
        Statement();
        R();
    }
    else
    {
        parsetree.drawepsilon();
    }	

    parsetree.pop();

}

//
// Label --> 'L' num ':'
//
void parser_t::Label()
{
	parsetree.push(NT_Label);

	eat_token(T_label);
	eat_token(T_num);
	eat_token(T_colon);

    parsetree.pop();
}

void parser_t::Jump()
{
    parsetree.push(NT_Jump);


    parsetree.pop();
}


//
// Assignment --> 'm' '[' Expression ']' '=' Expression 
//
void parser_t::Assignment()
{
	parsetree.push(NT_Assignment);
    
    eat_token(T_m);
    eat_token(T_opensquare);
    Expression();
    eat_token(T_closesquare);
    eat_token(T_equals);
    Expression();

	parsetree.pop();
}


//
// Print --> 'print' Expression
//
void parser_t::Print()
{
    parsetree.push(NT_Print);

    eat_token(T_print);
    //printf("eat print\n");
    Expression();

    parsetree.pop();
}


void parser_t::Expression()
{
    parsetree.push(NT_Expression);

    switch(scanner.next_token())
    {
    	case T_num:
    	    eat_token(T_num);
    	    break;
    	case T_openparen:
    	    eat_token(T_openparen);
    	    Expression();
    	    eat_token(T_closeparen);
    	    break;
    	case T_m:
    	    eat_token(T_m);
    	    eat_token(T_opensquare);
    	    Expression();
    	    eat_token(T_closesquare);
    	    break;
        /*
    	case T_plus:
    	    break;
    	case T_minus:
    	    break;
    	case T_times:
    	    break;
    	case T_divide:
    	    break;
    	case T_power:
    	    break;
    	*/
    	default:
    	    syntax_error(NT_Expression);
    	    break;
    }
    //printf("next token: %s\n", token_to_string( scanner.next_token() ));

    parsetree.pop();
}

void parser_t::E1()
{

}



/*** Main ***********************************************/

int main()
{
	if(DEBUG)
	{
	    // instantiate the scanner to test
	    scanner_t scanner;

	    token_type t;
	    t = scanner.next_token();
	    printf("token: %s\n", token_to_string(t));

	    while(t != T_eof)
	    {
	        scanner.eat_token(t);
			t = scanner.next_token();
			printf("token: %s\n", token_to_string(t));
        }
        printf("line count: %i\n\n", scanner.get_line());
    }
	else
    {
		parser_t parser;
		parser.parse();
		return 0;
    }
}
