#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>

using namespace std;
#define code_printf(string, args...)		\
  fprintf (stderr, string , ##args)



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
  T_print,				// 15: print_M
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
  //Done: add symbolic names for you non-terminals here
  NT_RExpression,
  NT_Term,
  NT_RTerm,
  NT_Factor,
  NT_Statements,
  NT_Statement,
  NT_Label,
  NT_Jump,
  NT_JumpIf,
  NT_Assignment,
  NT_Print,
  NT_Power
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
  case NT_RExpression: strncpy(buffer,"RExpression",MAX_SYMBOL_NAME_SIZE); break;
    //Done: add the other nonterminals you need here		
  case NT_Term: strncpy(buffer,"Term", MAX_SYMBOL_NAME_SIZE); break;
  case NT_RTerm: strncpy(buffer,"RTerm", MAX_SYMBOL_NAME_SIZE); break;
  case NT_Factor: strncpy(buffer,"Factor", MAX_SYMBOL_NAME_SIZE);break;
  case NT_Statements: strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Power: strncpy(buffer,"Power",MAX_SYMBOL_NAME_SIZE); break;
  case NT_Jump: strncpy(buffer,"Jump", MAX_SYMBOL_NAME_SIZE); break;
  case NT_JumpIf: strncpy(buffer,"JumpIf", MAX_SYMBOL_NAME_SIZE); break;

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

  // returns the last parsed number
  int get_num(){
    return number;
  }

  //constructor 
  scanner_t();

private:

  int lineNo;
  unsigned int number;

  token_type current_token;

  void updateTokens();

  //error message and exit for unrecognized character
  void scan_error(char x);
  //error message and exit for token mismatch
  void mismatch_error(token_type c);
  
};



void scanner_t::updateTokens(){
  int character;
  char c;
  
  while(((character = getc(stdin)) != EOF)){

    c = character;    
    if(c == '\n')
      lineNo++;
    if(c != ' ' &&c != '\t' && c != '\n' && c != '\r')
      break;
  }

  if(character == EOF)
    current_token = T_eof;
  else if(c >= '0' && c <= '9'){
    char buf[11];
    char *endptr; 
    int i = 1;
    //    uint64_t number;

    buf[0] = c;
    c = fgetc(stdin);

    while(c >= '0' && c <= '9'){
      /*32-bit unsigned int can't be more than 10 characters long */
      if(i >= 10){
	printf("Line %d: Scanner error: integer overflow\n",lineNo);
	assert(0);
      }
      buf[i++] = c;
      c = fgetc(stdin);
    }
    ungetc(c,stdin);
    buf[i] = '\0';

    number = strtoul(buf, &endptr, 10);
    assert(*endptr == '\0');
    if(number > 2147483647){
      printf("Line %d: Scanner error: integer overflow\n",lineNo);
      assert(0);
    }
      
    current_token = T_num;
  }
  else if(c == '+')
    current_token = T_plus;
  else if(c == '-')
    current_token = T_minus;
  else if(c == '*'){
    c = fgetc(stdin);
    if(c != '*'){
      ungetc(c,stdin);
      current_token = T_times;
    }
    else
      current_token = T_power;
  }
  else if(c == '/')
    current_token = T_divide;
  else if(c == '=')
    current_token = T_equals;
  else if(c == '(')
    current_token = T_openparen;
  else if(c == ')')
    current_token = T_closeparen;
  else if(c == '[')
    current_token = T_opensquare;
  else if(c == ']')
    current_token = T_closesquare;
  else if(c == 'm')
    current_token = T_m;
  else if(c == 'g'){
    char buf[4];
    fread(buf,3,1,stdin);
    buf[3] = 0;
    if(strcmp(buf,"oto") != 0){
      printf("Line: %d Scanner error Did you mean goto?\n",lineNo);
      assert(0);
    }
    
    current_token = T_goto;
  }
  else if(c == 'i'){
    c = fgetc(stdin);
    if(c != 'f')
      printf("Line: %d Scanner error Did you mean if?\n",lineNo);
    current_token = T_if;
  }
  else if(c == 'p'){
    char buf[5];
    fread(buf,4,1,stdin);
    buf[4] = 0;
    if(strcmp(buf,"rint") != 0){
      printf("Line: %d Scanner error Did you mean print?\n",lineNo);
      assert(0);
    }

    current_token = T_print;
  }
  else if(c == ':')
    current_token = T_colon;
  else if(c == 'L')
    current_token = T_label;
  else{
    scan_error(c);
  }
}



token_type scanner_t::next_token()
{
  return current_token;  
}

void scanner_t::eat_token(token_type c)
{
  if(current_token != c){
    mismatch_error(c);
  }

  updateTokens();

  
}

scanner_t::scanner_t(): lineNo(1)
{
  /*read one token in*/
  updateTokens();
}

int scanner_t::get_line()
{
  return lineNo;
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
  //Done: fill this out with the rest of the 
  //recursive decent stuff (more methods)
  void Statements();
  void Statement();
  void Label();
  void Jump();
  void JumpIf();
  void Assignment();
  void Print();
  void Expression();
  void Term();
  void Factor();
  void Power();
  void RExpression();
  void RTerm();

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


//DONE: the Start() function is not quite right.  Right now
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
  //parsing of the data.
  parsetree.push(NT_Start);
  code_printf("#include <stdio.h>\n");
  
  code_printf("inline int intPow(int num, int e) {\n");
  code_printf("\tint res = 1;\n");
  code_printf("\tif(e>=0){\n");
  code_printf("\t\tfor(;e>0;e--)res = res*num;\n");
  code_printf("\t\treturn res;\n");
  code_printf("\t}\n");
  code_printf("\tif(num == 1) return 1;\n"); /* if e is negative 1^-a is 1*/
  code_printf("\treturn 0;\n");  /* if e is negative floor(1 ^-a) is 0*/ 
  code_printf("}\n");

  code_printf("int main(int argc, char *argv[]){\n");
  code_printf("\tint m[101];\n");
  // code_printf("\tint m[101];\n");

  Statements();

  code_printf("\treturn 0;\n");
  code_printf("}\n");

  parsetree.pop();
  

  
    
}
void parser_t::Statements()
{
  parsetree.push(NT_Statements);
  if(scanner.next_token() != T_eof){
    Statement();
    Statements();
  }
  else
    parsetree.drawepsilon();
  parsetree.pop();
}

void parser_t::Statement()
{
  parsetree.push(NT_Statement);
  code_printf("\t");
  switch(scanner.next_token()){
  case T_label:
    Label();
    break;
  case T_goto:
    Jump();
    code_printf(";\n");
    break;
  case T_m:
    Assignment();
    code_printf(";\n");
    break;
  case T_print:
    Print();
    code_printf(";\n");
    break;
  default:
    syntax_error(NT_Start);
    break;
  }

  parsetree.pop();
}

void parser_t::Label(){
  parsetree.push(NT_Label);
  eat_token(T_label);
  eat_token(T_num);
  eat_token(T_colon);

  code_printf("my_label%d:\n", scanner.get_num());

  parsetree.pop();
}

void parser_t::Jump(){
  int number;
  parsetree.push(NT_Jump);
  eat_token(T_goto);
  eat_token(T_num);
  number = scanner.get_num();
  JumpIf();
  code_printf("goto my_label%d",number);
  parsetree.pop();
}

void parser_t::JumpIf(){
  parsetree.push(NT_JumpIf);
  if(scanner.next_token() == T_if){
    code_printf("if(");
    eat_token(T_if);
    Expression();
    code_printf(")\n\t\t");
  }
  else
    parsetree.drawepsilon();
  

  parsetree.pop();

}

void parser_t::Assignment(){
  parsetree.push(NT_Assignment);

  code_printf("m[");
  eat_token(T_m);
  eat_token(T_opensquare);
  Expression();
  eat_token(T_closesquare);
  eat_token(T_equals);
  code_printf("] = ");

  Expression();
  parsetree.pop();
}
void parser_t::Print(){
  parsetree.push(NT_Print);
  eat_token(T_print);
  code_printf("printf(\"%%d\\n\", ");
  Expression();
  code_printf(")"); 
  parsetree.pop();
}

void parser_t::Expression(){
  parsetree.push(NT_Expression);
  Term();
  RExpression();
  parsetree.pop();    
}
void parser_t::RExpression(){
  parsetree.push(NT_RExpression);
  switch(scanner.next_token()){
  case T_plus:
    code_printf(" + ");
    eat_token(T_plus);
    Expression();
    break;
  case T_minus:
    code_printf(" - ");
    eat_token(T_minus);
    Expression();
    break;
  default:
    parsetree.drawepsilon();
  }
  parsetree.pop();    
}

void parser_t::Term(){
  parsetree.push(NT_Term);
  Factor();
  RTerm();
  parsetree.pop();
}

void parser_t::RTerm(){
  parsetree.push(NT_RTerm);
  switch(scanner.next_token()){
  case T_times:
    code_printf(" * ");
    eat_token(T_times);
    Term();
    break;
  case T_divide:
    code_printf(" / ");
    eat_token(T_divide);
    Term();
    break;
  default:
    parsetree.drawepsilon();  
  }
  parsetree.pop();
}


void parser_t::Factor(){
  token_type token;
  parsetree.push(NT_Factor);
  code_printf("intPow(");
  switch(token = scanner.next_token()){
  case T_openparen:
    eat_token(T_openparen);
    code_printf("(");
    Expression();
    code_printf(")");
    eat_token(T_closeparen);
    break;
  case T_m:
    code_printf("m[");
    eat_token(T_m);
    eat_token(T_opensquare);
    Expression();
    eat_token(T_closesquare);
    code_printf("]");
    break;
  case T_num:
    code_printf("%d",scanner.get_num());
    eat_token(T_num);
    break;
  default:
    syntax_error(NT_Factor);
    /*
    fprintf(stderr, "%d Invalid factor: %s\n", scanner.get_line(), token_to_string(token));
    assert(0);
    */
  }
  Power();
  parsetree.pop();
}
 
void parser_t::Power(){
  parsetree.push(NT_Power);
  switch(scanner.next_token()){
  case T_power:
    code_printf(", ");
    eat_token(T_power);
    Factor();
    code_printf(")");

    break;
  default:
    code_printf(", 1)");
    parsetree.drawepsilon();
  }
  parsetree.pop();

}




/*** Main ***********************************************/

int main()
{
  parser_t parser;
  parser.parse();
  return 0;
}
