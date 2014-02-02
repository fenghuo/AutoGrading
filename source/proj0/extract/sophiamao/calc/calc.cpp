// http://www.cs.ucsb.edu/~cs160/project0.html

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>

using namespace std;


/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25
#define MAX_NO_DIGIT 10  // need to check

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
	NT_Expression
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
        default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
    }
	return buffer;
}

/*** Scanner Class ***********************************************/

class scanner_t {
public:
    
	void eat_token(token_type c);
	token_type next_token();
	int get_line();
	scanner_t();
	char* get_print_data();
	int get_print_data_size();
	char* get_goto_data();
	int get_goto_no_size();
	char* get_label();
	int get_label_size();
    void mismatch_error(token_type c);
    
private:
    
	void scan_error(char x);
	
	
	void add_to_line(int ignore, int* len, int* len_if, char* c_line, char* c_if, char c);
    
	token_type cur_token_type;
	char cur_token[MAX_SYMBOL_NAME_SIZE];
	int cur_token_size;
	int cur_line_no;
	char print_data[256];
	int print_data_size;
	char goto_data[256];
	int goto_no_size;
	char label[256];
	int label_size;
};

char* scanner_t::get_print_data() {
	return print_data;
}

int scanner_t::get_print_data_size() {
	return print_data_size;
}

char* scanner_t::get_goto_data() {
	return goto_data;
}

int scanner_t::get_goto_no_size() {
	return goto_no_size;
}

char* scanner_t::get_label() {
	return label;
}

int scanner_t::get_label_size() {
	return label_size;
}

void scanner_t::add_to_line(int ignore, int* len, int* len_if, char* c_line, char* c_if, char c) {
	if(ignore == 1)
		return;
    
	if(*len_if > 0)
		c_if[*(len_if)++] = c;
	else
		c_line[(*len)++] = c;
}

token_type scanner_t::next_token()
{
	if(cur_token_size != 0)  // not eaten yet
		return cur_token_type;
    
	int c, tmp, i;
	static char c_line[256];
	static int len = 0;
	static char c_if[256];
	static int len_if = 0;
	static int ignore = 0;
	
	cur_token_size = 0;
	
	while((c = getchar()) == ' '){};
	
	while(c == '\n') {
		cur_line_no++;
		c = getchar();
		for(i = 0; i < len_if; ++i) {
			fprintf(stderr, "%c", c_if[i]);
		}
		if(len_if > 0)
			fprintf(stderr, ") ");
		for(i = 0; i < len; ++i) {
			fprintf(stderr, "%c", c_line[i]);
		}
		if(len > 0)
			fprintf(stderr, ";\n");  // for C code
		len = 0;
		len_if = 0;
		ignore = 0;
	}
	
	while(c == ' ') {
		c = getchar();
	}
    
	switch(c){
		case '=':
			cur_token_type = T_equals;
		case '(':
			cur_token_type = T_openparen;
		case ')':
			cur_token_type = T_closeparen;
		case '[':
			cur_token_type = T_opensquare;
		case ']':
			cur_token_type = T_closesquare;
		case '+':
			cur_token_type = T_plus;
		case '-':
			cur_token_type = T_minus;
		case '/':
			cur_token_type = T_divide;
			cur_token[cur_token_size++] = c;
			add_to_line(ignore, &len, &len_if, c_line, c_if, c);
			break;
		case '*':
			cur_token[cur_token_size++] = c;
			cur_token_type = T_times;
			add_to_line(ignore, &len, &len_if, c_line, c_if, c);
			tmp = getchar();
			if(tmp == '*') {
				cur_token[cur_token_size++] = tmp;
				cur_token_type = T_power;
				add_to_line(ignore, &len, &len_if, c_line, c_if, c);
			} else {
				ungetc(tmp, stdin);  // put it back: peek only
			}
			break;
		case 'p':
			tmp = 0;
			cur_token[cur_token_size++] = c;
			if((c = getchar()) == 'r') {
				cur_token[cur_token_size++] = c;
				if((c = getchar()) == 'i') {
					cur_token[cur_token_size++] = c;
					if((c = getchar()) == 'n') {
						cur_token[cur_token_size++] = c;
						if((c = getchar()) == 't') {
							cur_token[cur_token_size++] = c;
							if((c = getchar()) == ' ') {
								tmp = 1;
								cur_token_type = T_power;
								ignore = 1;
								fprintf(stderr, "//printf(\" \");");
								// print_data_size = 0;
								// while((c = getchar()) != '\n') {  // read print data
                                // print_data[print_data_size++] = c;
                                // cur_line_no++;
								// }
							}
						}
					}
				}
			}
			if(tmp == 0)
				scan_error(c);
			break;
		case 'i':
			cur_token[cur_token_size++] = c;
			len_if = 0;
			c_if[len_if++] = 'i';
			tmp = getchar();
			if(tmp == 'f') {
				cur_token[cur_token_size++] = tmp;
				cur_token_type = T_if;
				add_to_line(ignore, &len, &len_if, c_line, c_if, tmp);
				add_to_line(ignore, &len, &len_if, c_line, c_if, '(');
			} else {
				scan_error(tmp);
			}
			break;
		case 'g':
			tmp = 0;
			cur_token[cur_token_size++] = c;
			add_to_line(ignore, &len, &len_if, c_line, c_if, c);
			if((c = getchar()) == 'o') {
				cur_token[cur_token_size++] = c;
				add_to_line(ignore, &len, &len_if, c_line, c_if, c);
				if((c = getchar()) == 't') {
					cur_token[cur_token_size++] = c;
					add_to_line(ignore, &len, &len_if, c_line, c_if, c);
					if((c = getchar()) == 'o') {
						cur_token[cur_token_size++] = c;
						add_to_line(ignore, &len, &len_if, c_line, c_if, c);
						if((c = getchar()) == ' ') {
							add_to_line(ignore, &len, &len_if, c_line, c_if, ' ');
							tmp = 1;
							cur_token_type = T_goto;
							goto_no_size = 0;
							while((c = getchar()) == ' '){};
							if(c >= '0' && c <= '9') {    // get label num
								goto_data[goto_no_size++] = c;
								add_to_line(ignore, &len, &len_if, c_line, c_if, 'L');
								add_to_line(ignore, &len, &len_if, c_line, c_if, c);
								while(c = getchar()) {
									if(c >= '0' && c <= '9') {
										goto_data[goto_no_size++] = c;
										add_to_line(ignore, &len, &len_if, c_line, c_if, c);
										if(goto_no_size > MAX_NO_DIGIT)
											scan_error(c);
									} else {
										break;
									}
								}
								if(c != ' ') {
									scan_error(c);
								} else {
									ungetc(c, stdin);  // put it back: peek only
								}
							} else {
								scan_error(c);
							}
						}
					}
				}
			}
			if(tmp == 0)
				scan_error(c);
			break;
		case 'L':
			cur_token[cur_token_size++] = c;
			cur_token_type = T_label;
			if(c_line[0] == 'g')
				add_to_line(ignore, &len, &len_if, c_line, c_if, 'L');
			add_to_line(ignore, &len, &len_if, c_line, c_if, c);
			label_size = 0;
			c = getchar();
			if(c >= '0' && c <= '9') {    // get label num
				label[label_size++] = c;
				add_to_line(ignore, &len, &len_if, c_line, c_if, c);
				while(c = getchar()) {
					if(c >= '0' && c <= '9') {
						label[label_size++] = c;
						add_to_line(ignore, &len, &len_if, c_line, c_if, c);
						if(label_size > MAX_NO_DIGIT)
							scan_error(c);
					} else {
						break;
					}
				}
				if(c != ':') {
					scan_error(c);
				} else {
					ungetc(c, stdin);  // put it back: peek only
				}
			} else {
				scan_error(c);
			}
			break;
		case ':':
			cur_token[cur_token_size++] = c;
			cur_token_type = T_colon;
			add_to_line(ignore, &len, &len_if, c_line, c_if, c);
			tmp = getchar();
			if(tmp == ' ' || tmp == '\n') {
				ungetc(tmp, stdin);  // put it back: peek only
			} else {
				scan_error(tmp);
			}
			break;
		case 'm':
			cur_token[cur_token_size++] = c;
			cur_token_type = T_m;
			add_to_line(ignore, &len, &len_if, c_line, c_if, c);
			tmp = getchar();
			if(tmp == '[') {
				ungetc(tmp, stdin);  // put it back: peek only
			} else {
				scan_error(tmp);
			}
			break;
		case EOF:  // end of file
			cur_token_type = T_eof;
			cur_token[cur_token_size++] = c;
			break;
		default:
			if(c >= '0' && c <= '9') {
				cur_token_type = T_num;
				cur_token[cur_token_size++] = c;
				add_to_line(ignore, &len, &len_if, c_line, c_if, c);
				while(c = getchar()) {
					if(c >= '0' && c <= '9') {
						cur_token[cur_token_size++] = c;
						add_to_line(ignore, &len, &len_if, c_line, c_if, c);
						if(cur_token_size > MAX_NO_DIGIT)
							scan_error(c);
					} else {
						break;
					}
				}
				if(c != ' ') {
					scan_error(c);
				} else {
					ungetc(c, stdin);  // put it back: peek only
				}
			} else {
				scan_error(c);
			}
	}
	
	return cur_token_type;
}

void scanner_t::eat_token(token_type c)
{
	cur_token_size = 0;   // 0 means token is "eaten"
}

scanner_t::scanner_t()
{
}

int scanner_t::get_line()
{
	return cur_line_no;
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
    
	fprintf(stderr, "%s\n", "#include \"stdio.h\"");
	fprintf(stderr, "%s\n", "#include \"math.h\"");
	fprintf(stderr, "%s\n", "int main(void) {");
	switch( scanner.next_token() )
	{
		case T_plus:
		case T_minus:
		case T_times:
		case T_divide:
		case T_power:
		case T_openparen:
		case T_closeparen:
		case T_opensquare:
		case T_closesquare:
		case T_m:
			parsetree.push(scanner.next_token());
			eat_token(scanner.next_token());
			Start();
			parsetree.pop();
			break;
		case T_goto:
			parsetree.push(scanner.next_token());
			eat_token(scanner.next_token());
			if(scanner.next_token() == T_label) {
				parsetree.push(scanner.next_token());
				eat_token(scanner.next_token());
				Start();
				parsetree.pop();
			} else {
				scanner.mismatch_error(scanner.next_token());  // exit()
			}
			break;
		case T_label:
			parsetree.push(scanner.next_token());
			eat_token(scanner.next_token());
			if(scanner.next_token() == T_colon) {
				parsetree.push(scanner.next_token());
				eat_token(scanner.next_token());
				Start();
				parsetree.pop();
			} else {
				scanner.mismatch_error(scanner.next_token());  // exit()
			}
			break;
		case T_num:
			// not complete
			parsetree.push(scanner.next_token());
			eat_token(scanner.next_token());
			Start();
			parsetree.pop();
			break;
		case T_equals:
			// not complete
			parsetree.push(scanner.next_token());
			eat_token(scanner.next_token());
			Start();
			parsetree.pop();
			break;
		case T_if:
			// not complete
			parsetree.push(scanner.next_token());
			eat_token(scanner.next_token());
			Start();
			parsetree.pop();
			break;
		case T_print:
			// not complete
			parsetree.push(scanner.next_token());
			eat_token(scanner.next_token());
			Start();
			parsetree.pop();
			break;
		case T_eof:
			parsetree.drawepsilon();
			break;
		default:
			syntax_error(NT_Start);
			break;
	}
	fprintf(stderr, "%s\n", "}");
	//now that we are done with List, we can pop it from the data
	//structure that is tracking it for drawing the parse tree
	parsetree.pop();
}

//WRITEME: you will need to put the rest of the procedures here


/*** Main ***********************************************/

int main()
{
	parser_t parser;
	parser.parse();
	return 0;
}
