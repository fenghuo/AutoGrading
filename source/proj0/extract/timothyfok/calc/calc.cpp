#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <math.h>

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
	//WRITEME: add symbolic names for you non-terminals here
	NT_Statements, 
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Expression,
	NT_ExpressionD,
	NT_Term,
	NT_TermD,
	NT_Factor,
	NT_Assignment,
	NT_Print
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
      case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
      case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
      case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
      case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;  
      case NT_ExpressionD: strncpy(buffer,"ExpressionD",MAX_SYMBOL_NAME_SIZE); break;  
      case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
      case NT_TermD: strncpy(buffer,"TermD",MAX_SYMBOL_NAME_SIZE); break;
      case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;

      case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
      case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
      case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
      default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;

		}
	return buffer;
}

/*** Scanner Class ***********************************************/

class scanner_t {
  public:
  int x;
  int decimalcount;
  char ch;
  char pch;
  char g1ch;
  char g2ch;
  char g3ch;
  char g4ch;
  char pow1ch, pow2ch;
  char p1ch, p2ch, p3ch, p4ch, p5ch;
  char if1ch;
  char if2ch;
  char character;
  char negativenum;
  int linenum;
  token_type mismatch_token;
  string numberstream;
  long numbercheck;
  long gotonum;
  long go;
  string gotostream;
  string labelnum;
  long pow1num;
  long pow2num;
	//eats the next token and prints an error if it is not of type c
	void eat_token(token_type c);

	//peeks at the lookahead token
	token_type next_token();

	//return line number for errors
	int get_line();

	//constructor 
	scanner_t();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  

	//error message and exit for unrecognized character
	void scan_error(char x);
        void range_error(char x);
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
  //cout << "NEXT TOKEN CALLED" << endl;
  numberstream = "";
  decimalcount = 0;
  if (x == 1)
    {
      ch = getchar();
      x = 2;
      while (ch == '\n' || ch == '\t' || ch == ' ') { if (ch == '\n'){linenum++;};ch=getchar(); }
      if (ch == '='){ mismatch_token = T_equals; return T_equals; }
      if (ch == '+'){ mismatch_token = T_plus; return T_plus; }
      if (ch == '-'){ 
	/*	numberstream += ch;
	negativenum = getchar();
	if (negativenum == '0' || negativenum == '1' ||
	    negativenum == '2' || negativenum == '3' ||
	    negativenum == '4' || negativenum == '5' ||
	    negativenum == '6' || negativenum == '7' ||
	    negativenum == '8' || negativenum == '9'  )
	  {
	    range_error(negativenum);
	  }
	else{
	             ungetc(negativenum,stdin);
	*/    mismatch_token = T_minus; return T_minus;/* }*/}
      if (ch == '*'){ 
	pow1ch = '*';
	//cout << "Getting next char" << endl;
	ch = getchar();
	if (ch == '*')
	            { 
		      //	      cout << "power should be returned" << endl;
		      pow2ch = '*';
		            ch = '*';
		      ungetc(pow2ch,stdin);
		      mismatch_token = T_power; return T_power;}
	else
	  { //cout <<"ungetting ch, it is " << ch << endl;
		      ungetc(ch,stdin);
		      ch = '*';
	              mismatch_token = T_times; return T_times;}}
      if (ch == '/'){ mismatch_token = T_divide;  return T_divide; }
      if (ch == '('){ mismatch_token = T_openparen; return T_openparen; }
      if (ch == ')'){ mismatch_token = T_closeparen; return T_closeparen; }
      if (ch == '['){ mismatch_token = T_opensquare; return T_opensquare; }
      if (ch == ']'){ mismatch_token = T_closesquare; return T_closesquare; }
      if (ch == 'm'){ mismatch_token = T_m; return T_m; }
      if (ch == 'i'){
	if1ch = 'i';
	ch = getchar();
	if (ch == 'f'){
	  if2ch = 'f';
	  ch = 'i';
	  ungetc(if2ch,stdin);
	  mismatch_token = T_if; return T_if;	  
	}
	else
	  {
	    ungetc(ch,stdin);
	  }
      }
      if (ch == 'p')
      {
	p1ch = 'p';
	ch = getchar();
	if (ch == 'r')
	{
	  p2ch = 'r';
	  ch = getchar();
	  if (ch == 'i')
	  {
	    p3ch = 'i';
	    ch = getchar();
	    if (ch == 'n')
	    {
	      p4ch = 'n';
	      ch = getchar();
	      if (ch == 't')
		{
		  p5ch = 't';
       		  ch = 'p';
		  ungetc(p5ch,stdin);
		  ungetc(p4ch,stdin); 
		  ungetc(p3ch,stdin);
		  ungetc(p2ch,stdin);
		mismatch_token = T_print; return T_print;
		}
	      else
		{
		ungetc(ch,stdin);
		}
	    }
	    else
	    {
	    ungetc(ch,stdin);  
	    }
	  }
	  else
	  {
	  ungetc(ch,stdin);
	  }
	}
	else
	{
	ungetc(ch,stdin);  
	}
      }
      if (ch == 'L'){ mismatch_token = T_label; return T_label; }
      if (ch == ':'){ mismatch_token = T_colon; return T_colon; }
      if (ch == 'g'){ 
	//cout << "ch is " << ch << endl;
	ch = getchar();
	g1ch = 'g';
	if (ch == 'o')
	{
	  //cout << "ch is " << ch << endl;
	  ch = getchar();
	  g2ch = 'o';
	  if (ch == 't')
	  {
	    //cout << "ch is " << ch << endl;
	    g3ch = 't';
	    ch = getchar();
	    if (ch == 'o')
	    {
	      //cout << "ch is " << ch << endl;
	      g4ch = 'o';
	      ungetc(g4ch,stdin);
	      ungetc(g3ch,stdin);
	      ungetc(g2ch,stdin);
	      //ungetc(g1ch,stdin);
	      	      	      ch = 'g';
	      mismatch_token = T_goto; return T_goto;
	    }
	    else
	    {
	      ungetc(ch,stdin);
	    }  
	  }
	  else
	  {
	    ungetc(ch,stdin);
	  }
	}
	else
	{
	  ungetc(ch,stdin);
	}
      }
      
      if (ch == EOF){ mismatch_token = T_eof; return T_eof;}
      if (ch == '0' || ch == '1' ||
          ch == '2' || ch == '3' ||
          ch == '4' || ch == '5' ||
          ch == '6' || ch == '7' ||
          ch == '8' || ch == '9'  )
	{
	  numberstream += ch;
	        numbercheck = atof(numberstream.c_str());
		gotonum = atof(numberstream.c_str());
		labelnum = atof(numberstream.c_str());
	  mismatch_token = T_num;
	  return T_num;
	}
    }
  else{
    while (ch == '\n' || ch == '\t' || ch == ' ') {if (ch == '\n'){linenum++;}ch = getchar();}
      if (ch == '='){ mismatch_token = T_equals; return T_equals; }
      if (ch == '+'){ mismatch_token = T_plus; return T_plus; }
      if (ch == '-'){ 
	/*	negativenum = getchar();
	if (negativenum == '0' || negativenum == '1' ||
	    negativenum == '2' || negativenum == '3' ||
	    negativenum == '4' || negativenum == '5' ||
	    negativenum == '6' || negativenum == '7' ||
	    negativenum == '8' || negativenum == '9'  )
	  {
	    range_error(negativenum);
	  }
	else{
	              ungetc(negativenum,stdin);
	*/  mismatch_token = T_minus; return T_minus; /*}*/}
      if (ch == '*'){
	//cout << "second time" << endl;
	pow1ch = '*';
	//cout << "ch should be *, it is " << ch << endl;
	ch = getchar();
	//cout << "ch is now " << ch << endl;
	if (ch == '*')
	            { pow2ch = '*';
		      		      ch = '*';
		      ungetc(pow2ch,stdin);
		      mismatch_token = T_power; return T_power;}
	else
	  {  ungetc(ch,stdin); 
	    ch ='*';
	              mismatch_token = T_times; return T_times;}}
      if (ch == '/'){ mismatch_token = T_divide;  return T_divide; }
      if (ch == '('){mismatch_token = T_openparen; return T_openparen; }
      if (ch == ')'){ mismatch_token = T_closeparen; return T_closeparen; }
      if (ch == '['){ mismatch_token = T_opensquare; return T_opensquare; }
      if (ch == ']'){ mismatch_token = T_closesquare; return T_closesquare; }
      if (ch == 'm'){ mismatch_token = T_m; return T_m; }
      if (ch == 'i'){
	if1ch = 'i';
	ch = getchar();
	if (ch == 'f'){
	  if2ch= 'f';
	  ch ='i';
	  ungetc(if2ch,stdin);
	  mismatch_token = T_if; return T_if;
	}
	else
	  {
	    ungetc(ch,stdin);
	  }
      }

      if (ch == 'p')
      {
	//cout << "p secomnd time next token" << endl;
	p1ch = 'p';
	ch = getchar();
	if (ch == 'r')
	{
	  p2ch = 'r';
	  ch = getchar();
	  if (ch == 'i')
	  {
	    p3ch = 'i';
	    ch = getchar();
	    if (ch == 'n')
	    {
	      p4ch = 'n';
	      ch = getchar();
	      if (ch == 't')
		{
		  p5ch = 't';
		  ungetc(p5ch,stdin);
		  ungetc(p4ch,stdin);
		  ungetc(p3ch,stdin);
		  ungetc(p2ch,stdin);
		  ch = 'p';
		mismatch_token = T_print; return T_print;
		}
	      else
		{
		ungetc(ch,stdin);
		}
	    }
	    else
	    {
	    ungetc(ch,stdin);  
	    }
	  }
	  else
	  {
	  ungetc(ch,stdin);
	  }
	}
	else
	{
	ungetc(ch,stdin);  
	}
      }
      if (ch == 'L'){ mismatch_token = T_label; return T_label; }
      if (ch == ':'){ mismatch_token = T_colon; return T_colon; }
      if (ch == 'g'){ 
	//cout << "second time " << endl;
	//cout << "ch is " << ch << endl;
	ch = getchar();
	//	cout << "ch is now " << ch << endl;
	g1ch = 'g';
	//			ch = getchar();
			//ch is now 'o';
	//cout << "ch should be o, is it? it is " << ch << endl;
	if (ch == 'o')
	{
	  // cout << "2ch should be o, is it? it is " << ch << endl;
	  g2ch = 'o';
	  ch = getchar();
	  //cout << "ch should be t, and it is " << ch << endl;
	  if (ch == 't')
	  {
	    g3ch = 't';
	    ch = getchar();
	    if (ch == 'o')
	    {
	      //  cout << "Ch should be o, is it? it is " << ch << endl;
	      g4ch='o';
	      //    cout << "I MADE IT HERE!" << endl;
	      ungetc(g4ch,stdin);
	      ungetc(g3ch,stdin);
	      ungetc(g2ch,stdin);
	      //ungetc(g1ch,stdin);
	      	      ch = 'g';
	      mismatch_token = T_goto; return T_goto;
	    }
	    else
	    {
	      ungetc(ch,stdin);
	    }  
	  }
	  else
	  {
	    ungetc(ch,stdin);
	  }
	}
	else
	{
	  ungetc(ch,stdin);
	}
      }
      
      if (ch == EOF){ mismatch_token = T_eof; return T_eof;}
    if (ch == '0' || ch == '1' ||
        ch == '2' || ch == '3' ||
        ch == '4' || ch == '5' ||
        ch == '6' || ch == '7' ||
        ch == '8' || ch == '9' )
      {
	 numberstream += ch;
	       numbercheck = atof(numberstream.c_str());
	       gotonum = atof(numberstream.c_str());
	       labelnum = atof(numberstream.c_str());
	mismatch_token = T_num;  return T_num;
      }
  }
  decimalcount = 0;
  scan_error(ch);
  exit(1);


}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eat_token
  if (next_token() != c){
    mismatch_error(c);
    }
  
  //check if the next character is a number first. keep
  //itereating through until we get a white space.
  // cout << "at eat token, ch is " << ch << endl;
  //cout << "EAT TOKEN CALLED " << endl;
    // cout << "before eating the token, ch is " << ch << endl;
  while(ch == ' ' || ch == '\n' || ch == '\t'){
    if (ch == '\n') { linenum++; }
    ch = getchar();
  }
  //unique cases for things like [3

  if(ch =='*'){
    // cout << "before eating, ch is " << ch << endl;
    ch = getchar();
    //cout << "after eating once, ch is " << ch << endl;
    if(ch == '*'){
      //cout << "should be a power.Ch is " << ch << endl;
      ch = getchar();
      //cout << "Ch is " << ch << endl;
      return;
    }
    else {/*ungetc(ch,stdin);cout << "CH IS " << ch;*/ return; }
    return;
  }

  if (ch == '[' || ch == 'L' || ch == '/' || ch == '+' || ch == 'm' ||
      ch == '(' || ch == ']' || ch == ')' || ch == '=' || ch == '-' || ch == ':' ){
    ch = getchar();
    // cout << "after eating token, ch is now " << ch << endl;
    return;
  }

  if(ch =='i'){
    ch = getchar();
    //cout << "ch is " << ch << endl;
    ch = getchar();
    return;
  }

  if(ch == 'g'){
    // cout << "eating g nowHIHIHIHIHIHIHIHIHIHIHI" << endl;
    ch = getchar();
    //cout << "at eat ch is " << ch << endl;
    ch = getchar();
    //cout << "ch is " << ch << endl;
    ch = getchar();
    //cout << "ch is " << ch << endl;
    ch = getchar();
    //cout << "ch is " << ch << endl;
    //	        ch = getchar();
    //				 cout << "ch is " << ch << endl;
    return;
  }

  if(ch == 'p'){
    //    cout << "eat token on p" << endl;
    //cout << "ch is " << ch << endl;
    ch = getchar();
    // cout << "ch is " << ch << endl;
    ch = getchar();
    //cout << "ch is " << ch << endl;
    ch = getchar();
    //cout << "ch is " << ch << endl;
    ch = getchar();
    //cout << "ch is " << ch << endl;
    ch = getchar();
    //cout << "ch is now " << ch << endl;
    return;
  }

    ch = getchar();
    long upperlim = pow(2,31) - 1;

  while (ch == '0' || ch == '1' ||
	 ch == '2' || ch == '3' ||
	 ch == '4' || ch == '5' ||
	 ch == '6' || ch == '7' ||
	 ch == '8' || ch == '9' )
    {
      numberstream += ch;
      numbercheck = atof(numberstream.c_str());
      gotonum = atof(numberstream.c_str());
      labelnum = atof(numberstream.c_str());

      //cout << "the numbercheck is" << numbercheck;
      if (/*this negative number check actually never happens, negative numbers are taken care of in next token.*/numbercheck < 0 || (numbercheck > upperlim)){
	range_error(ch);
	//cout << "WHAT IS THIS!!!";
      }


      ch = getchar();
    }
 
  
  numberstream = "";
  
}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
  x = 1;
  ch = ' ';
  character = 'n';
  linenum = 1;
  mismatch_token = T_plus;
  decimalcount = 0;
  numberstream = "";
  numbercheck = 0;

}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
  return linenum;
}

void scanner_t::scan_error (char x)
{
	printf("scan error: unrecognized character '%c' -line %d\n",x, get_line());
	exit(1);

}

void scanner_t::range_error (char x)
{
  printf("range error: number must be between 0 and ((2^31)-1) -line %d\n",get_line());
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
  void Statement();
  void Statements();
  void Jump();
  void Expression();
  void ExpressionD();
  void Term();
  void TermD();
  void Factor();
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
  //cout << "Start" << endl;
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);
	cerr << "#include <stdio.h>\n#include <math.h>\n\nint main(){\nint m[101];\n";
	switch( scanner.next_token() ) 
	{
	        case T_label:
			Statement();
			Statements();
			break;
		case T_goto:
		        Statement();
			Statements();
			break;
	        case T_m:
	               Statement();
		       Statements();
		       break;
	        case T_print:
		       Statement();
		       Statements();
		       break;
		default:
			syntax_error(NT_Start);
			break;
	}
	cerr << "\nreturn 0;\n  }";
	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
}
void parser_t::Statements()
{
  //cout << "Statements" << endl;
  parsetree.push(NT_Statements);
  switch( scanner.next_token() )
    {
    case T_label:
      Statement();
      Statements();
      break;
    case T_goto:
      Statement();
      Statements();
      break;
    case T_m:
      Statement();
      Statements();
      break;
    case T_print:
      Statement();
      Statements();
      break;
   
      //trying out this case:
      /* case T_minus:
      parsetree.drawepsilon();
      break;*/
      /*
    case T_equals:
      parsetree.drawepsilon();
      break;
      */
     case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_Statements);
      break;
    }
  parsetree.pop();
}

void parser_t::Statement()
{
  //cout << "Statement" << endl;
  parsetree.push(NT_Statement);
  switch( scanner.next_token() )
    {
    case T_label:
      eat_token(T_label);
      eat_token(T_num);  
      eat_token(T_colon);
      cerr << "L" << scanner.gotonum << ":\n";
      break;
    case T_goto:
      eat_token(T_goto);
      eat_token(T_num);
      //cerr << "goto " << scanner.numbercheck;
      Jump();
      break;
    case T_m:
      eat_token(T_m);
      eat_token(T_opensquare);
      cerr << "m[";
      Expression();
      eat_token(T_closesquare);
      eat_token(T_equals);
      cerr << "] = ";
      Expression();
      cerr << ";\n";
      break;
    case T_print:
      eat_token(T_print);
      cerr << "printf(\"%d\\n\",";
      Expression();
      cerr << ");\n";
      break;
    default:
      syntax_error(NT_Statement);
      break;
    }
  parsetree.pop();
}

void parser_t::Jump()
{
  //cout << "Jump" << endl;
  parsetree.push(NT_Jump);
  switch( scanner.next_token() )
    {
    case T_if:
      eat_token(T_if);
      cerr << " if( ";
      scanner.go = scanner.gotonum;
      Expression();
      cerr << "){\n     goto L" << scanner.go << ";\n    }\n";
      
      break;    
    case T_eof:
      //cout << "IOOGOT HERE OMAOSDFMLASKDFMASLDFKMASL;KFMA;SLKDFM" << endl;
      parsetree.drawepsilon();
      cerr << "goto " << scanner.gotonum << ";\n";
      break;
      //prolly delete
    case T_goto:
      parsetree.drawepsilon();
      cerr << "goto " << scanner.gotonum << ";\n";
      break;
    case T_print:
      parsetree.drawepsilon();
      cerr << "goto " << scanner.gotonum << ";\n";
      break;
    case T_m:
      parsetree.drawepsilon();
      cerr << "goto " << scanner.gotonum << ";\n";
      break;
    case T_label:
      parsetree.drawepsilon();
      cerr << "goto " << scanner.gotonum << ";\n";
      break;
    default:
      syntax_error(NT_Jump);
      break;
    }
  parsetree.pop();
}

void parser_t::Expression()
{
  //cout << "Expression" << endl;
  parsetree.push(NT_Expression);
  switch( scanner.next_token() )
    {
    case T_num:
      Term();
      ExpressionD();
      break;
    case T_openparen:
      Term();
      ExpressionD();
      break;
    case T_m:
      Term();
      ExpressionD();
      break;
      //probably remove this case T_goto
      /*    case T_minus:
      parsetree.drawepsilon();
      break;*/
    /*  case T_goto:
      parsetree.drawepsilon();
      break;*/
    default:
      syntax_error(NT_Expression);
      break;
    }
  parsetree.pop();
}
void parser_t::ExpressionD()
{
  //cout << "ExpressionD" << endl;
  parsetree.push(NT_ExpressionD);
  switch( scanner.next_token() )
    {
    case T_plus:
      eat_token(T_plus);
      cerr << " + ";
      Term();
      ExpressionD();
      break;
    case T_minus:
      eat_token(T_minus);
      cerr << " - ";
      Term();
      ExpressionD();
      break;
      //thiws case likely removed:
    case T_goto:
      parsetree.drawepsilon();
      break;
    case T_print:
      parsetree.drawepsilon();
      break;
    case T_label:
      parsetree.drawepsilon();
      break;
      //trying out this case:
    case T_closesquare:
      parsetree.drawepsilon();
      break;/*
    case T_num:
      parsetree.drawepsilon();
      break;*/
    case T_m:
      parsetree.drawepsilon();
      break;
    case T_closeparen:
      parsetree.drawepsilon();
      break;/*
    case T_equals:
      parsetree.drawepsilon();
      break;*/
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_ExpressionD);
      break;
    }
  parsetree.pop();
}
void parser_t::Term()
{
  //cout << "Term" << endl;
  parsetree.push(NT_Term);
  switch( scanner.next_token() )
    {
    case T_num:
      Factor();
      TermD();
      break;
    case T_openparen:
      Factor();
      TermD();
      break;
    case T_m:
      Factor();
      TermD();
      break;
    default:
      syntax_error(NT_Term);
      break;
    }
  parsetree.pop();
}
void parser_t::TermD()
{
  //cout << "TermD" << endl;
  parsetree.push(NT_TermD);
  switch( scanner.next_token() )
    {
    case T_times:
      eat_token(T_times);
      cerr << " * ";
      Factor();
      TermD();
      break;
    case T_divide:
      eat_token(T_divide);
      cerr << " / ";
      Factor();
      TermD();
      break;
    case T_power:
      eat_token(T_power);
      //      scanner.pow1num = scanner.numbercheck;
      //cerr << " pow(" << scanner.numbercheck << ",";  
      
      Factor();
      // scanner.pow2num = scanner.numbercheck;
      cerr << ")";
      TermD();
      break;
      //this case may possibly be removed: T_Goto:
    case T_print:
      parsetree.drawepsilon();
      break;
    case T_goto:
      parsetree.drawepsilon();
      break;
    case T_label:
      parsetree.drawepsilon();
      break;
      //trying out this case:
    case T_closesquare:
      parsetree.drawepsilon();
      break;/*
    case T_num:
      parsetree.drawepsilon();
      break;*/
    case T_m:
      parsetree.drawepsilon();
      break;
    case T_plus:
      parsetree.drawepsilon();
      break;
    case T_minus:
      parsetree.drawepsilon();
      break;
    case T_closeparen:
      parsetree.drawepsilon();
      break;/*
    case T_equals:
      parsetree.drawepsilon();
      break;*/
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      syntax_error(NT_TermD);
      break;
    }
  parsetree.pop();
}

void parser_t::Factor()
{
  //cout << "Factor" << endl;
  parsetree.push(NT_Factor);
  switch( scanner.next_token() )
    {
    case T_num:
      eat_token(T_num);
      if (scanner.next_token() == T_power){
	cerr << "(int) pow(" << scanner.numbercheck << ",";
      }
      else{
      cerr << scanner.numbercheck;
      }
      break;
    case T_openparen:
      eat_token(T_openparen);
      cerr << "( ";
      Expression();
      eat_token(T_closeparen);
      cerr << " )";
      break;
    case T_m:
      eat_token(T_m);
      eat_token(T_opensquare);
      cerr << "m[";
      Expression();
      eat_token(T_closesquare);
      cerr << "]";
      break;
    default:
      syntax_error(NT_Factor);
      break;
    }
  parsetree.pop();
}




//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

int main(int argc, char*argv[])
{
  // just scanner
  if (argc > 1) {
    if (strcmp(argv[1], "-s") == 0) {
      scanner_t scanner;
      token_type tok = scanner.next_token();
      while(tok != T_eof){
	scanner.eat_token(tok);
	printf("%s\n", token_to_string(tok));
	tok = scanner.next_token();
      }
      printf("%s\n", token_to_string(tok));
    }
  }
  else{

	parser_t parser;
	parser.parse();
	return 0;
  }
}
