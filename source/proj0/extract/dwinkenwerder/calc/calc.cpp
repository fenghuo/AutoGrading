#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals
**********************/

#define MAX_SYMBOL_NAME_SIZE 25

//all of the terminals in the language
typedef enum {
     T_eof = 0,         // 0: end of file
     T_num,             // 1: numbers
     T_plus,            // 2: +
     T_minus,           // 3: -
     T_times,           // 4: *
     T_divide,          // 5: /
     T_power,           // 6: **
     T_equals,          // 7: =
     T_openparen,       // 8: (
     T_closeparen,      // 9: )
     T_opensquare,      // 10: [
     T_closesquare,     // 11: ]
     T_m,               // 12: m
     T_goto,            // 13: goto
     T_if,              // 14: if
     T_print,           // 15: print
     T_label,           // 16: L
     T_colon,           // 17: :
     T_newln            // 18: \n
} token_type;

//this function returns a string for the token. IT is used in the parsetree_t
//class to actually dump the parsetree to a dot file (which can then be turned
//into a picture). Note that the return char* is a reference to a local copy
//and it needs to be duplicated if you are a going require multiple instances
//simultaniously
char* token_to_string(token_type c) {
     static char buffer[MAX_SYMBOL_NAME_SIZE];
     switch( c ) {
          case T_eof:           strncpy
(buffer,"eof",MAX_SYMBOL_NAME_SIZE); break;
          case T_num:           strncpy
(buffer,"num",MAX_SYMBOL_NAME_SIZE); break;
          case T_plus:           strncpy
(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
          case T_minus:           strncpy
(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
          case T_times:           strncpy
(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
          case T_divide:           strncpy
(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
          case T_power:           strncpy
(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
          case T_equals:           strncpy
(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
          case T_openparen:           strncpy
(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
          case T_closeparen:           strncpy
(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
          case T_opensquare:           strncpy
(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
          case T_closesquare:           strncpy
(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
          case T_m:           strncpy
(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
          case T_goto:           strncpy
(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
          case T_if:           strncpy
(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
          case T_print:           strncpy
(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
          case T_label:           strncpy
(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
          case T_colon:           strncpy
(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
          case T_newln:           strncpy
(buffer,"\n",MAX_SYMBOL_NAME_SIZE);break;
          default:                strncpy
(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
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
     NT_ExpressionPrime,
     NT_Term,
     NT_TermPrime,
     NT_Statements,
     NT_StatementsPrime,
     NT_Statement,
     NT_Assignment,
     NT_Print,
     NT_Jump,
     NT_Label,
     NT_Factor

} nonterm_type;

//this function returns a string for the non-terminals. It is used in the parsetree_t
//class to actually dump the parsetree to a dot file (which can then be turned
//into a picture). Note that the return char* is a reference to a local copy
//and it needs to be duplicated if you are a going require multiple instances
//simultaneously
char* nonterm_to_string(nonterm_type nt)
{
     static char buffer[MAX_SYMBOL_NAME_SIZE];
     switch( nt ) {
            case epsilon: strncpy
(buffer,"e",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Start: strncpy
(buffer,"Start",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Expression: strncpy
(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
	      //WRITEME: add the other nonterminals you need here
            case NT_ExpressionPrime: strncpy
(buffer,"ExpressionPrime",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Term: strncpy
(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
            case NT_TermPrime: strncpy
(buffer,"TermPrime",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Assignment: strncpy
(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Print: strncpy
(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Jump: strncpy
(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Label: strncpy
(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Statements: strncpy
(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
            case NT_StatementsPrime: strncpy
(buffer,"StatementsPrime",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Statement: strncpy
(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
            case NT_Factor: strncpy
(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;


            default: strncpy
(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
           }
     return buffer;
}

/*** Scanner Class
***********************************************/

class scanner_t {
  public:

    //eats the next token and prints an error if it is not of type c
    void eat_token(token_type c);

    //peeks at the lookahead token
    token_type next_token();

    //return line number for erros
    int get_line();

    //constructor
    scanner_t();

private:

  //WRITEME: Figure out what you will need to write the scanner
  //and to implement the above interface. It does not have to
  //be a state machine or anything fancy. Just read in the
  //characters one at a time (using getchar would be a good way)
  //and group them into tokens.

  //error message and exit for unrecognized character
  void scan_error(char x);
  //error message and exit for token mismatch
  void mismatch_error(token_type c);

  void num_error(int x);

  //WRITEME: this is a bogus member for implementing a useful stub, it should
  //be cut out once you get the scanner up and going.
  token_type temp;
  int line;
  bool numchain;
  string currentnum;
};

token_type scanner_t::next_token()
{
     //WRITEME: replace this bogus junk with code that will take a peek
     //at the next token and return it to the parser. It should not
     //actually consume a token - you should be able to call next_token()
     //multiple times without actually reading any more tokens in
  char tokentype,a,b,c,d;
  tokentype = std::cin.get();
  switch(tokentype) {
  case '+': temp = T_plus;std::cin.unget();numchain=false;break;
  case '-': temp = T_minus; std::cin.unget();numchain=false;break;
  case ':': temp = T_colon; std::cin.unget();numchain=false;break;
  case '[': temp = T_opensquare; std::cin.unget();numchain=false;break;
  case ']': temp = T_closesquare; std::cin.unget();numchain=false;break;
  case '(': temp = T_openparen; std::cin.unget();numchain=false;break;
  case ')': temp = T_closeparen; std::cin.unget();numchain=false;break;
  case 'L': temp = T_label; std::cin.unget();numchain=false;break;
  case 'm': temp = T_m; std::cin.unget();numchain=false;break;
  case '/': temp = T_divide; std::cin.unget();numchain=false;break;
  case '=': temp = T_equals; std::cin.unget();numchain=false;break;
  case '0': temp = T_num; std::cin.unget();
    if(numchain==false){currentnum="0";}
    else{numchain=true;int x = currentnum.size();currentnum.insert(x,"0");
if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '1': temp = T_num; std::cin.unget();
if(numchain==false){currentnum="1";}
 else{numchain=true;currentnum.insert(currentnum.size(),"1");
   if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '2': temp = T_num; std::cin.unget();
if(numchain==false){currentnum="2";}
 else{numchain=true;currentnum.insert(currentnum.size(),"2");
   if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '3': temp = T_num; std::cin.unget();
if(numchain==false){currentnum="3";}
 else{numchain=true;currentnum.insert(currentnum.size(),"3");
   if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '4': temp = T_num; std::cin.unget();
if(numchain==false){currentnum="4";}
 else{numchain=true;currentnum.insert(currentnum.size(),"4");
   if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '5': temp = T_num; std::cin.unget();
if(numchain==false){currentnum="5";}
 else{numchain=true;currentnum.insert(currentnum.size(),"5");
   if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '6': temp = T_num; std::cin.unget();
if(numchain==false){currentnum="6";}
 else{numchain=true;currentnum.insert(currentnum.size(),"6");
   if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '7': temp = T_num; std::cin.unget();
if(numchain==false){currentnum="7";}
 else{numchain=true;currentnum.insert(currentnum.size(),"7");
   if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '8': temp = T_num; std::cin.unget();
if(numchain==false){currentnum="8";}
 else{numchain=true;currentnum.insert(currentnum.size(),"8");
   if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;
  case '9': temp = T_num; std::cin.unget();
    if(numchain==false){currentnum="9";}
    else{numchain=true;currentnum.insert(currentnum.size(),"9");
      if(atoi(currentnum.c_str())>2147483647){num_error(atoi(currentnum.c_str()));}
 }break;  
  case '-1': temp = T_eof; std::cin.unget();numchain=false;break;
  case '\n': temp = T_newln;std::cin.unget();numchain=false;break;
  case '*': numchain=false; 
           a = std::cin.get();
            if(a=='*')
	      {temp=T_power;std::cin.unget();}
	    else
	      {temp=T_times;std::cin.unget();}
	    std::cin.putback('*');break;
  case 'i': numchain=false;
    a=std::cin.get();
    if(a=='f')
      {temp=T_if;std::cin.unget();}
    else
      {std::cin.unget();scan_error(tokentype);}
    std::cin.putback('i');break;
  case 'p': numchain=false;
    a=std::cin.get();
    if(a=='r')
      {b=std::cin.get();
	if(b=='i')
	  {c=std::cin.get();
	    if(c=='n')
	      {d=std::cin.get();
		if(d=='t')
		  {temp=T_print;std::cin.putback('t');}
		else{std::cin.unget();scan_error(d);}
	      std::cin.putback('n');}
	    else{std::cin.unget();scan_error(c);}
	  std::cin.putback('i');}
	else{std::cin.unget();scan_error(b);}
      std::cin.putback('r');}
    else{std::cin.unget();scan_error(a);}
    std::cin.putback('p');break;
  case 'g': numchain=false;
    a=std::cin.get();
    if(a=='o')
      {b=std::cin.get();
	if(b=='t')
	  {c=std::cin.get();
	    if(c=='o')
	      {temp=T_goto;std::cin.putback('o');}
	    else{std::cin.unget();;scan_error(c);}
	    std::cin.putback('t');}
	else{std::cin.unget();scan_error(b);}
      std::cin.putback('o');}
    else{std::cin.unget();scan_error(a);}
    std::cin.putback('g');;
    break;
  default:scan_error(tokentype);break;
  }
    return temp;
}
void scanner_t::num_error(int x)
{
  printf("scan error: number %d at line %d entered is higher than 2^31-1",x,get_line());
  exit(1);
}

void scanner_t::eat_token(token_type c)
{
     //if we are supposed to eat token c, and it does not match
     //what we are supposed to be reading from file, then it is a
     //mismatch error ( call - mismatch_error(c) )

     //WRITEME: cut this bogus stuff out and implement eat_token
  char temp;
  temp=std::cin.get();
  switch( c ){
  case T_print:
    if(temp=='p')
      {temp=std::cin.get();
	if(temp=='r')
	  {temp=std::cin.get();
	    if(temp=='i')
	      {temp=std::cin.get();
		if(temp=='n')
		  {temp=std::cin.get();
		    if(temp=='t')
		      {break;}
		    else{mismatch_error(c);break;}}
		else{mismatch_error(c);break;}}
	    else{mismatch_error(c);break;}}
	else{mismatch_error(c);break;}}
    else{mismatch_error(c);break;}
  case T_if:
    if(temp=='i')
      {temp=std::cin.get();
	if(temp=='f')
	  {break;}
	else{mismatch_error(c);break;}}
    else{mismatch_error(c);break;}
  case T_goto:
    if(temp=='g')
      {temp=std::cin.get();
	if(temp=='o')
	  {temp=std::cin.get();
	    if(temp=='t')
	      {temp=std::cin.get();
		if(temp=='o')
		   {break;}
		else{mismatch_error(c);break;}}
	    else{mismatch_error(c);break;}}
	else{mismatch_error(c);break;}}
    else{mismatch_error(c);break;}
  case T_power:
    if(temp=='*')
      {temp=std::cin.get();break;}
    else{mismatch_error(c);break;}
  case T_colon:
    if(temp==':')
      {break;}
    else{mismatch_error(c);break;}
  case T_label:
    if(temp=='L')
      {break;}
    else{mismatch_error(c);break;}
  case T_m:
    if(temp=='m')
      {break;}
    else{mismatch_error(c);break;}
  case T_opensquare:
    if(temp=='[')
      {break;}
    else{mismatch_error(c);break;}
  case T_closesquare:
    if(temp==']')
      {break;}
    else{mismatch_error(c);break;}
  case T_openparen:
    if(temp=='(')
      {break;}
    else{mismatch_error(c);break;}
  case T_closeparen:
    if(temp==')')
      {break;}
    else{mismatch_error(c);break;}
  case T_equals:
    if(temp=='=')
      {break;}
    else{mismatch_error(c);break;}
  case T_plus:
    if(temp=='+')
      {break;}
    else{mismatch_error(c);break;}
  case T_minus:
    if(temp=='-')
      {break;}
    else{mismatch_error(c);break;}
  case T_times:
    if(temp=='*')
      {break;}
    else{mismatch_error(c);break;}
  case T_divide:
    if(temp=='/')
      {break;}
    else{mismatch_error(c);break;}
  case T_num:
    if(temp=='0'||temp=='1'||temp=='2'||temp=='3'||temp=='4'||temp=='5'||temp=='6'||temp=='7'||temp=='8'||temp=='9')
      {break;}
    else{mismatch_error(c);break;}
  case T_eof:
    if(temp=='-1')
      {break;}
    else{mismatch_error(c);break;}
  case T_newln:
    if(temp=='\n')
      {line++;break;}
    else{mismatch_error(c);break;}
  default: mismatch_error(c);break;
  }
}

scanner_t::scanner_t()
{
  //WRITEME: this is the scanner constructor
  line=1;
  numchain=false;
  currentnum="";
}

int scanner_t::get_line()
{
  //WRITEME: this function should return the current line number
  //(used for error reporting)
  return line;
  //NOTE: FOR KEEPING TRACK OF LINES, JUST ADD TO COUNTER FOR EACH
  //NEW LINE TOKEN YOU FIND.
}

void scanner_t::scan_error (char x)
{
     printf("scan error: unrecognized character"" '%c' -line %d\n",x, get_line());
     exit(1);

}

void scanner_t::mismatch_error (token_type x)

{
     printf("syntax error: found %s",token_to_string
(next_token()) );
     printf("expecting %s -line %d\n", token_to_string(x),
get_line());
     exit(1);
}


/*** ParseTree Class
********************************************/

//just dumps out the tree as a dot file. The interface is described below
//on the actual methods. This class is full and complete. You should not
//have to touch a thing if everything goes according to plan. while you don't
//have to modify it, you will have to call it from your recursive decent
//parcer, so read about the interface below.
class parsetree_t {
  public:
     void push(token_type t);
     void push(nonterm_type t);
     void pop();
     void drawepsilon();
     parsetree_t();
  private:
     enum stype_t{
          TERMINAL = 1,
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
  printf("disgraph G {\n");
}

//This push function takes a non terminal and keeps it on the parsetree
//stack. The stack keeps trace of where we are in the parse tree as
//we walk it in a depth first way. You should call push when you start
//expanding a symbol, and call pop when you are done. The parsetree_t
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

//when you are done parsing a symbol, pop it. That way the parsetree_t will
//know that you are now working on a higher part of the tree.
void parsetree_t::pop()
{
     if( !stuple_stack.empty() ) {
         stuple_stack.pop();
     }

     if ( stuple_stack.empty() ) {
       printf( "};\n" );
     }
}

//draw an epsilon on the parse tree hanging off of the top of the stack
void parsetree_t::drawepsilon()
{
     push(epsilon);
     pop();
}

//this private print function is called from push. Basically it
//just prints out the command to draw an edge form the top of the stack (TOS)
//to the new symbol that was just pushed. If it happens to be a terminal
//then it makes it a snazzy blue color so you can see your program on the leaves
void parsetree_t::printedge(stuple temp)
{
     if( temp.stype == TERMINAL ) {
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
       printf( "\t\"%s%d\" ", stuple_to_string
(stuple_stack.top()), stuple_stack.top().uniq );
       printf( "-> \"%s%d\"\n", stuple_to_string(temp),temp.uniq);
     }
}

//just a private utility for helping with the printing of the dot stuff
char* parsetree_t::stuple_to_string(const stuple& s)
{
     static char buffer[MAX_SYMBOL_NAME_SIZE];
     if ( s.stype == TERMINAL ) {
       snprintf( buffer, MAX_SYMBOL_NAME_SIZE, "%s",token_to_string(s.t) );
     } else if ( s.stype == NONTERMINAL ) {
       snprintf( buffer, MAX_SYMBOL_NAME_SIZE, "s", nonterm_to_string(s.nt));
     } else {
          assert(0);
     }
     
     return buffer;
}


/*** Parser Class
***********************************************/

//the parser_t class handles everything. It has an instance of scanner_t
//so it can peek at tokens and eat them up. It also has access to the
//parsetree_t class so it can print out the parse tree as it figures it out.
//To make the recursive decent parse work, you will need to add some
//methods to this class. The helper functions are described below

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
  void StatementsPrime();
  void Statement();
  void Label();
  void Jump();
  void Assignment();
  void Print();
  void Expression();
  void ExpressionPrime();
  void Term();
  void TermPrime();
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
//to make progress. You should call this as soon as you can know
//there is a syntax_error
void parser_t::syntax_error(nonterm_type nt)
{
     printf("syntax error: found %s in the parsing %s - line %d\n",
	  token_to_string( scanner.next_token()),
       nonterm_to_string(nt),
       scanner.get_line() );
     exit(1);
}


//Once the recursive decent parser is set up, you simply call parse()
//to parse the entire input, all of which can be derived from the start
//symbol
void parser_t::parse()
{
     Start();
}


//WRITEME: the Start() function is not quite right. Right now
//it is made to parse the grammer:
// Start-> '+' Start | EOF
//which is not a very interesting language. It has been included
//so you can see the basics of how to structure your recursive
//decent code.

//Here is an example
void parser_t::Start()
{
  //push this non-terminal onto the parse tree.
  //the parsetree class is just for drawing the finished
  //parse tree, and sould in should have no effect the actual
  //parsing of the data
  parsetree.push(NT_Start);
  
  Statements();

  //now that we are done with List, we can pop it from the data
  //structure that is tracking it for drawing the parse tree
  parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here

void parser_t::Statements()
{
  parsetree.push(NT_Statements);
  switch(scanner.next_token())
    {
    case T_eof:
      parsetree.drawepsilon();
      break;

     default:
      Statement();
      StatementsPrime();
    }
  parsetree.pop();
}

void parser_t::Statement()
{
  parsetree.push(NT_Statement);

  switch( scanner.next_token())
    {
       case T_label:
	 Label();
	 break;
       case T_print:
	 Print();
	 break;
       case T_goto:
	 Jump();
	 break;
       case T_m:
	 Assignment();
	 break;
    default:
      syntax_error(NT_Statement);

    }
  parsetree.pop();
}

void parser_t::StatementsPrime()
{
  parsetree.push(NT_StatementsPrime);
  switch(scanner.next_token())
    {
    case T_eof:
      parsetree.drawepsilon();
      break;
    default:
      Statement();
      StatementsPrime();
    }
  parsetree.pop();
}

void parser_t::Label()
{
  parsetree.push(NT_Label);
  if(scanner.next_token() == T_label)
    {
      eat_token(T_label);
    }
  else{syntax_error(NT_Label);}
  if(scanner.next_token()==T_num)
    {
      eat_token(T_num);
    }
  else{syntax_error(NT_Label);}
if(scanner.next_token() == T_colon)
  {
    eat_token(T_colon);
  }
 else{syntax_error(NT_Label);}
  parsetree.pop();
}

void parser_t::Jump()
{


}

void parser_t::Print()
{
  parsetree.push(NT_Print);
  if(scanner.next_token()==T_print)
    {eat_token(T_print);}
  else{syntax_error(NT_Print);}
  Expression();

  parsetree.pop();
}

void parser_t::Expression()
{
  parsetree.push(NT_Expression);
  switch(scanner.next_token())
    {
      case T_m:
	eat_token(T_m);
	if(scanner.next_token()==T_opensquare)
	  {eat_token(T_opensquare);}
	else{syntax_error(NT_Expression);}
	Expression();
	if(scanner.next_token()==T_closesquare)
	  {eat_token(T_closesquare);}
	else{syntax_error(NT_Expression);}
	break;
	
      default:
	Term();
	ExpressionPrime();
    }
  parsetree.pop();
}

void parser_t::ExpressionPrime()
{
  parsetree.push(NT_ExpressionPrime);
  switch(scanner.next_token())
    {
      case T_plus:
	eat_token(T_plus);
	Term();
	ExpressionPrime();
	break;
      case T_minus:
	eat_token(T_minus);
	Term();
	ExpressionPrime();
	break;
      case T_eof:
	parsetree.drawepsilon();
	break;
      default:
	syntax_error(NT_ExpressionPrime);
	break;
    }
  parsetree.pop();
}

void parser_t::Term()
{
  parsetree.push(NT_Term);
  Factor();
  TermPrime();
  parsetree.pop();
}

void parser_t::TermPrime()
{
  parsetree.push(NT_TermPrime);
  switch(scanner.next_token())
    {
      case T_times:
	eat_token(T_times);
	Factor();
	TermPrime();
	break;
      case T_divide:
	eat_token(T_divide);
	Factor();
	TermPrime();
	break;
      case T_eof:
	parsetree.drawepsilon();
	break;
      default:
	syntax_error(NT_TermPrime);
    }
  parsetree.pop();
}

void parser_t::Factor()
{
  switch(scanner.next_token())
    {
      case T_num:
	eat_token(T_num);
	break;
    default:
      syntax_error(NT_Factor);
    }
}

void parser_t::Assignment()
{
  parsetree.push(NT_Assignment);
  if(scanner.next_token() == T_m)
    {
      eat_token(T_m);
    }
  else{syntax_error(NT_Assignment);}
  if(scanner.next_token()==T_opensquare)
    {
      eat_token(T_opensquare);
    }
  else{syntax_error(NT_Assignment);}
  Expression();
if(scanner.next_token() == T_closesquare)
  {
    eat_token(T_closesquare);
  }
 else{syntax_error(NT_Assignment);}
if(scanner.next_token() == T_equals)
  {
    eat_token(T_equals);
  }
 else{syntax_error(NT_Assignment);}
 Expression();
 parsetree.pop();

}

/*** Main ***********************************************/

int main()
{
  
  scanner_t scann;
  //token_type token = scann.next_token();
  //printf(token_to_string(token));
  //token = scann.next_token();
  //printf(token_to_string(token));
  //token = scann.next_token();
  //printf(token_to_string(token));
  //while(!T_eof)
  //{token_type token = scann.next_token();
  //printf(token_to_string(token));
  //scann.eat_token(token);
  //}

  parser_t parser;
  parser.parse();
  return 0;
}
