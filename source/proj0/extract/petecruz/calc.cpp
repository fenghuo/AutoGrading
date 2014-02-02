#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map>
#include <memory>
#include <math.h>
#include <vector>
#include <ctype.h>
#include <stack>

using namespace std;

#define get_num(token, i) if (token.type == T_num) { i = token.num } else { printf("Attempting to retrieve number from non-number token."); exit(1); }

/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25

//all of the terminals in the language
typedef enum {
	T_eof = 0,		// 0: end of file
	T_num,	    	// 1: numbers
	T_plus,			// 2: +
	T_minus,		// 3: -
	T_times,		// 4: *
	T_divide,		// 5: /
	T_power, 		// 6: **
	T_equals,		// 7: =
	T_openparen,		// 8: (
	T_closeparen, 	// 9: )
	T_opensquare,	// 10: [
	T_closesquare,	// 11: ]
	T_m,			// 12: m
	T_goto,			// 13: goto
	T_if,			// 14: if
	T_print,		// 15: print
	T_label,		// 16: L
	T_colon,		// 17: :
    T_epsilon
} token_type;

//this function returns a string for the token.  It is used in the parsetree_t
//class to actually dump the parsetree to a dot file (which can then be turned
//into a picture).  Note that the return char* is a reference to a local copy
//and it needs to be duplicated if you are a going require multiple instances
//simultaniously
char* token_to_string(token_type c) {
	static char buffer[MAX_SYMBOL_NAME_SIZE];
	switch( c ) {
		case T_eof:		    strncpy(buffer,"eof",MAX_SYMBOL_NAME_SIZE); break;
		case T_num:		    strncpy(buffer,"num",MAX_SYMBOL_NAME_SIZE); break;
		case T_plus:		strncpy(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
		case T_minus:	    strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
		case T_times:	    strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
		case T_divide:	    strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
		case T_power:	    strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
		case T_equals:	    strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
		case T_openparen:	strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
        case T_closeparen:	strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
        case T_opensquare:	strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
        case T_closesquare:	strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
        case T_m:			strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
        case T_goto:		strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
        case T_if:			strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
        case T_print:	    strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
        case T_label:	    strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
        case T_colon:	    strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
		default: 			strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
	}
	return buffer;
}

vector<string> split(string str)
{
    vector<string> tokens;
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<vector<string> >(tokens));
    
    return tokens;
}

// TODO: This might break when numbers are not single digits
int num_from_char(char *str)
{
    string number;
    while (*str && isdigit(*str)) {
        //cout << str << endl;
        string val(str);
        number += val;
        str++;
        break;
    }

    //@cout << "(num_from_char) number is: " << number << endl;
    return stoi(number);
}

int n_digits(int number)
{ 
    if (number == 0 ) { return 1; }
    else { return floor(log10(abs(number))) + 1; }
}

//all of the non-terminals in the grammar (you need to add these in
//according to your grammar.. these are used for printing the thing out)
//please follow the convention we set up so that we can tell what the heck you
//are doing when we grade
typedef enum {
	epsilon = 100,
	NT_Start,
    NT_Statements,
    NT_StatementP,
    NT_Statement,
    NT_Label,
    NT_Jump,
    NT_Assignment,
    NT_Print,
	NT_Expression,
    NT_AddSub,
    NT_MultDiv,
    NT_Term,
    NT_Factor,
    NT_Parens,
    NT_MExp,
    NT_EXP
	//WRITEME: add symbolic names for you non-terminals here
} nonterm_type;

typedef struct {
    int lineNum;
    int colNum;
    int num;
    char *ident;
    token_type type;
} Token;

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
          case NT_StatementP: strncpy(buffer,"StatementP",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Statement: strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
		  case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
          case NT_AddSub: strncpy(buffer,"AddSub",MAX_SYMBOL_NAME_SIZE); break;
          case NT_MultDiv: strncpy(buffer,"MultDiv",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Term: strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Factor: strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
          case NT_Parens: strncpy(buffer,"Parens",MAX_SYMBOL_NAME_SIZE); break;
          case NT_MExp: strncpy(buffer,"MExp",MAX_SYMBOL_NAME_SIZE); break;
          case NT_EXP: strncpy(buffer,"EXP",MAX_SYMBOL_NAME_SIZE); break;
		  //WRITEME: add the other nonterminals you need here		
		  default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
		}
	return buffer;
}

/*** Scanner Class ***********************************************/

class scanner_t {
  public:
    int index = 0;
    int _index = 0;
    vector<Token> token_list;

	//eats the next token and prints an error if it is not of type c
	void eat_token(token_type c);

	//peeks at the lookahead token
	token_type next_token();
    Token _next_token();

    int get_number(Token);

	//return line number for errors
	int get_line();
    int _get_line();

	//constructor 
	scanner_t();
    void print_tokens(vector<Token> list);
    void tokenize_input();

  private:

	//WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.  
    map<int, string> scan_input();

	//error message and exit for unrecognized character
	void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);

	//WRITEME: this is a bogus member for implementing a useful stub, it should
	//be cut out once you get the scanner up and going.
	//token_type bogo_token;

};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens in 
	//if ( bogo_token!=T_plus && bogo_token!=T_eof ) return T_plus;
	//else return bogo_token;
    return token_list[index].type;
}

Token scanner_t::_next_token() { return token_list[index]; }

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a 
	//mismatch error ( call - mismatch_error(c) )

	//WRITEME: cut this bogus stuff out and implement eat_token
    token_type token = next_token();
    if (c == token) { index++; }
    else { mismatch_error(c); }
}

int scanner_t::get_number(Token token)
{
    return token.num;
}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
    if (index == token_list.size()) { return token_list[index - 1].lineNum; }
    else { return token_list[index].lineNum; }
}

int scanner_t::_get_line()
{
    return _index;
}

map<int, string> scanner_t::scan_input()
{
    int i = 1;
    string inputLine;
    map<int, string> lines;

    while(getline(cin, inputLine)) {
        lines.insert(std::pair<int, string>(i, inputLine));
        i += 1;
    }

    return lines;
}
    
void scanner_t::tokenize_input()
{
    vector<Token> tokenList;
    map<int, string> lines = scan_input();
    map<int, string>::iterator it;

    for (it = lines.begin(); it != lines.end(); it++) {
        //cout << "printing out the string: " << it->second << endl;
        char *char_str = new char[it->second.size() + 1];
        char_str[it->second.size()] = 0;
        memcpy(char_str, it->second.c_str(), it->second.size());
        _index++;

        for (int j = 0; j < strlen(char_str); j++) {
            //cout << "top level value of j: " << j << endl;
            //cout << "the size of the char_str is: " << strlen(char_str) << endl;
            Token token = { token.lineNum = it->first, token.colNum = j + 1 };

            //cout << char_str[j] << endl;
            switch (char_str[j]) {
                case 'm': token.type = T_m; break;
                case 'p':
                    if (strncmp(char_str + j, "print ", 6) == 0) {
                        j += 5; 
                        token.type = T_print;
                    }
                    break;
                case 'g':
                    if (strncmp(char_str + j, "goto ", 5) == 0) {
                        j += 4; // TODO: this was 5
                        token.type = T_goto;
                    }
                    break;
                case 'i':
                    if (strncmp(char_str + j, "if ", 3) == 0) {
                        j += 2; // TODO: this was 3
                        token.type = T_if;
                    }
                    break;
                case 'L': token.type = T_label; break;
                case '(': token.type = T_openparen; break;
                case ')': token.type = T_closeparen; break;
                case '[': token.type = T_opensquare; break;
                case ']': token.type = T_closesquare; break;
                case ':': token.type = T_colon; break;
                case '+': token.type = T_plus; break;
                case '-': token.type = T_minus; break;
                case '*':
                    //@cout << "inside the * case " << endl;
                    //@cout << char_str[j] << "&" << char_str[j+1] << "&" << char_str[j+2] << endl;
                    if (strncmp(char_str + j, "**", 2) == 0) {
                        //@cout << "inside the if clause of * " << endl;
                        j += 2;
                        token.type = T_power;
                    }
                    else { //@cout << "&&&&& in the * else clause " << endl;
                        token.type = T_times;
                    }
                    break;
                case '/': token.type = T_divide; break;
                case '=': token.type = T_equals; break;
                case ' ': break;
                default:
                    //cout << "default case" << endl;
                    //cout << "printing value: " << char_str[j] << endl;
                    if (isdigit(char_str[j])) {
                        //@cout << "inside clause " << char_str[j] << endl;
                        //@cout << "current position " << j << endl;
                        int number = num_from_char(char_str + j);
                        //@cout << "number is: " << number << endl;
                        if (n_digits(number) != 1) j += (n_digits(number) - 1);
                        token.type = T_num;
                        token.num = number;
                        //@cout << n_digits(number) << endl;
                        //@cout << "new position " << j << endl;
                        //cout << "inside num clause, value is: " << number << endl;
                    }
                    else { scan_error(char_str[j]); }
                    break;
            }

            if (token.type) {tokenList.push_back(token);
            //@cout << "Just pushed a token" << endl;
            //@cout << "the value of j is: " << j << endl;
            //cout << "token number is " << token.type << endl;
            //@cout << "***** the value in the char_str: " << char_str[j] << endl;
            }
        }
    }

    //cout << "THE LIST SIZE IS: " << tokenList.size() << endl;
    Token e;
    e.type = T_eof;
    e.lineNum = tokenList[tokenList.size() - 1].lineNum;
    tokenList.push_back(e);
    this->token_list = tokenList;
}

void scanner_t::print_tokens(vector<Token> list)
{
    vector<Token>::iterator it;
    for (it = list.begin(); it != list.end(); it++) {
        cout << it->type << endl;
    }
    
    cout << "list size is: " << list.size() << endl;
}

void scanner_t::scan_error (char x)
{
	printf("scan error: unrecognized character '%c' -line %d\n",x, _get_line());
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

// TODO: Create the shared pointers and finish the definitions in the parser
class AST {
    public:
        token_type token;
        nonterm_type type;
        int number;

        virtual string emit() = 0;
        void set_number(int value) { number = value; }
        int get_number() { return number; }
        void set_token(token_type t) { token = t; }
        token_type get_token() { return token; }
};

class EXP;
class MExp;
class Parens;
class Factor;
class Term;
class MultDiv;
class AddSub;
class Expression;
class Print;
class Assignment;
class Jump;
class Label;
class Statement;
class StatementP;
class Statements;
class Start;

class EXP : public AST {
    public: 
        shared_ptr<Parens> par;
        shared_ptr<EXP> exp;
        EXP();
        string emit();
};

class MExp : public AST {
    public:
        shared_ptr<Expression> node;
        MExp();
        string emit();
};

class Parens : public AST {
    public:
        shared_ptr<Expression> node;
        int flag;
        Parens();
        Parens(int value);
        string emit();
};

class Factor : public AST {
    public:
        shared_ptr<Parens> par;
        shared_ptr<EXP> exp;
        Factor();
        string emit();
};

class Term : public AST {
    public:
        shared_ptr<Factor> f;
        shared_ptr<MultDiv> md;
        shared_ptr<MExp> me;
        Term();
        string emit();
};

class MultDiv : public AST {
    public:
        shared_ptr<Factor> f;
        shared_ptr<MultDiv> md;
        MultDiv();
        string emit();
};

class AddSub : public AST {
    public:
        shared_ptr<Term> t;
        shared_ptr<AddSub> as;
        AddSub();
        string emit();
};

class Expression : public AST {
    public:
        shared_ptr<Term> t;
        shared_ptr<AddSub> as;
        Expression();
        string emit();
};

class Print : public AST {
    public:
        shared_ptr<MExp> node;
        Print();
        string emit();
};

class Assignment : public AST {
    public:
        shared_ptr<MExp> me;
        shared_ptr<Expression> e;
        Assignment();
        string emit();
};

class Jump : public AST {
    public:
        shared_ptr<Expression> node;
        Jump();
        Jump(int value);
        string emit();
};

class Label : public AST {
    public:
        Label();
        Label(int value);
        string emit();
};

class Statement : public AST {
    public:
        shared_ptr<Label> l;
        shared_ptr<Jump> j;
        shared_ptr<Assignment> a;
        shared_ptr<Print> p;
        int flag;
        Statement();
        string emit();
};

class StatementP : public AST {
    public:
        shared_ptr<Statement> s;
        shared_ptr<StatementP> sp;
        StatementP();
        string emit();
};

class Statements : public AST {
    public:
        shared_ptr<Statement> s;
        shared_ptr<StatementP> sp;
        Statements();
        string emit();
};

class Start : public AST {
    public:
        shared_ptr<Statements> node;
        Start();
        string emit();
};

EXP::EXP() { type = NT_EXP; }
string EXP::emit() {
    if (par != NULL) { return "pow(" + par->emit() + ", " + exp->emit() + ")"; }
    else { return ""; }
}

MExp::MExp() { type = NT_MExp; }
string MExp::emit() { return "m [ " + node->emit() + " ]"; }

Parens::Parens() { type = NT_Parens; }
Parens::Parens(int value) { type = NT_Parens; number = value; }
string Parens::emit() {
    if (flag == 1) {
        stringstream ss;
        ss << number;

        return ss.str();
    }
    else { return "(" + node->emit() + ")"; }
}

Factor::Factor() { type = NT_Factor; }
string Factor::emit() { return par->emit() + exp->emit(); }

Term::Term() { type = NT_Term; }
string Term::emit() {
    if (f != NULL) { return f->emit() + md->emit(); }
    else { return me->emit(); }
}

MultDiv::MultDiv() { type = NT_MultDiv; }
string MultDiv::emit() {
    if (token == T_times) {
        return "*" + f->emit() + md->emit();
    }
    else if (token == T_divide) {
        return "/" + f->emit() + md->emit();
    }
    else { return ""; }
}

AddSub::AddSub() { type = NT_AddSub; }
string AddSub::emit() {
    if (token == T_plus) {
        return "+" + t->emit() + as->emit();
    }
    else if (token == T_minus) {
        return "-" + t->emit() + as->emit();
    }
    else { return ""; }
}

Expression::Expression() { type = NT_Expression; }
string Expression::emit() { return t->emit() + as->emit(); }

Print::Print() {type = NT_Print; }
string Print::emit() { return "printf(\"%d\", " + node->emit() + ");"; }

Assignment::Assignment() { type = NT_Assignment; }
string Assignment::emit() { return me->emit() + " = " + e->emit() + ";\n"; }

Jump::Jump() { type = NT_Jump; }
Jump::Jump(int value) { type = NT_Jump; number = value; }
string Jump::emit() {
    stringstream ss;
    ss << number;
    if (node == NULL) { return "goto " + ss.str() + ";\n"; }
    else {
        return "if (" + node->emit() + ")" + "\n" + "goto " + ss.str() + ";\n";
    }
}

Label::Label() { type = NT_Label; }
Label::Label(int value) { type = NT_Label; number = value; }
string Label::emit() {
    stringstream ss;
    ss << number;
    return "L" + ss.str() + ":\n";
}

Statement::Statement() { type = NT_Statement; }
string Statement::emit() { 
    if (flag == 1) { return l->emit(); }
    else if (flag == 2) { return j->emit(); }
    else if (flag == 3) { return a->emit(); }
    else { return p->emit(); }
    /*
    if (l != NULL) { return l->emit(); }
    if (j != NULL) { return j->emit(); }
    if (a != NULL) { return a->emit(); }
    if (p != NULL) { return p->emit(); }
    */
}

StatementP::StatementP() { type = NT_StatementP; }
string StatementP::emit() {
    if (s != NULL) { return s->emit() + sp->emit(); }
    else { return ""; }
}

Statements::Statements() { type = NT_Statement; }
string Statements::emit() { return s->emit() + sp->emit(); }

Start::Start() { type = NT_Start; }
string Start::emit() { return node->emit(); }

//if (expression)
//    goto n
/*** Parser Class ***********************************************/

//the parser_t class handles everything.  It has an instance of scanner_t
//so it can peek at tokens and eat them up.  It also has access to the
//parsetree_t class so it can print out the parse tree as it figures it out.
//To make the recursive decent parser work, you will have to add some
//methods to this class.  The helper functions are described below

class parser_t {
  public:
	scanner_t scanner;
	parsetree_t parsetree;
	void eat_token(token_type t);
	void syntax_error(nonterm_type);

	//void Start();
    void parse();
	//WRITEME: fill this out with the rest of the 
	//recursive decent stuff (more methods)
    //shared_ptr<Start> _Start();
    void _Start();
    shared_ptr<Statements> _Statements();
    shared_ptr<StatementP> _StatementP();
    shared_ptr<Statement> _Statement();
    shared_ptr<Label> _Label();
    shared_ptr<Jump> _Jump();
    shared_ptr<Assignment> _Assignment();
    shared_ptr<Print> _Print();
    shared_ptr<Expression> _Expression();
    shared_ptr<AddSub> _AddSub();
    shared_ptr<MultDiv> _MultDiv();
    shared_ptr<Term> _Term();
    shared_ptr<Factor> _Factor();
    shared_ptr<Parens> _Parens();
    shared_ptr<MExp> _MExp();
    shared_ptr<EXP> _EXP();

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
	_Start();
}


//WRITEME: the Start() function is not quite right.  Right now
//it is made to parse the grammar:
//       Start -> '+' Start | EOF
//which is not a very interesting language.  It has been included
//so you can see the basics of how to structure your recursive 
//decent code.

//Here is an example
/*
void parser_t::Start()
{
	//push this non-terminal onto the parse tree.
	//the parsetree class is just for drawing the finished
	//parse tree, and should in should have no effect the actual
	//parsing of the data
	parsetree.push(NT_Start);

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

	//now that we are done with List, we can pop it from the data
	//stucture that is tracking it for drawing the parse tree
	parsetree.pop();
    
}
*/

void parser_t::_Start()
{
    parsetree.push(NT_Start);
    auto start = shared_ptr<Start>(new Start());
    start->node = _Statements();

    parsetree.pop();
    cerr << "int main() {\n";
    cerr << start->emit() << endl;
    cerr << "return 0;\n}" << endl;

    //cout << ss.emit() << endl;
}

//WRITEME: you will need to put the rest of the procedures here
shared_ptr<Statements> parser_t::_Statements()
{
    parsetree.push(NT_Statements);
    auto ss = shared_ptr<Statements>(new Statements());
    ss->s = _Statement();
    ss->sp = _StatementP();

    parsetree.pop();
    return ss;
}

shared_ptr<StatementP> parser_t::_StatementP()
{
    parsetree.push(NT_StatementP);
    auto sp = shared_ptr<StatementP>(new StatementP());
    if (scanner.next_token() == T_eof) {
        parsetree.push(T_eof);
        eat_token(scanner.next_token());
        parsetree.pop();
        parsetree.drawepsilon();
    }
    else {
        sp->s = _Statement(); // TODO: This might be an issue
        //cout << "finshed statement" << endl;
        sp->sp = _StatementP();
        //cout << "finished statementp" << endl;
    }

    //cout << "parsing statementp" << endl;
    parsetree.pop();
    return sp;
}

// TODO: Currently only handling the Assignment production
shared_ptr<Statement> parser_t::_Statement()
{
    parsetree.push(NT_Statement);
    auto s = shared_ptr<Statement>(new Statement());
    token_type token = scanner.next_token();

    //cout << token << endl;
    if (token == T_label) {
        s->flag = 1;
        s->l = _Label();
    }
    else if (token == T_goto) {
        s->flag = 2;
        s->j = _Jump();
    }
    else if (token == T_print) {
        s->flag = 4;
        s->p = _Print();
    }
    else {
        s->flag = 3;
        s->a = _Assignment();
    }

    //cout << "popping statement" << endl;
    parsetree.pop();
    //cout << "pop just finished" << endl;
    return s;
}

shared_ptr<Label> parser_t::_Label()
{
    parsetree.push(NT_Label);
    //cout << "Label going to eat: " << scanner.next_token() << endl;
    parsetree.push(T_label);
    eat_token(scanner.next_token());
    parsetree.pop(); // blah
    //cout << "Label going to eat: " << scanner.next_token() << endl;
    parsetree.push(T_num);
    eat_token(scanner.next_token());
    parsetree.pop(); // blah
    //cout << "Label going to eat: " << scanner.next_token() << endl;
    parsetree.push(T_colon);
    eat_token(scanner.next_token());
    parsetree.pop(); // blah
    auto l = shared_ptr<Label>(new Label());

    parsetree.pop();
    return l;

    /*
    if (scanner.next_token().type == T_label) {
        eat_token(scanner.next_token().type);
        // TODO: Do something with goto
        if (scanner.next_token().type == T_num) {
            int number = scanner.next_token().num;
            eat_token(scanner.next_token().type);
            // TODO: Do something with number
            if (scanner.next_token.type == T_colon) {
                // TODO: Do something with colon
                eat_token(scanner.next_token().type);
                // TODO: Do something with \n
                // TODO: Return a LABEL Node?
                return Label(number);
            }
            else { syntax_error(scanner.next_token().type); }
        }
        else { syntax_error(scanner.next_token().type); }
    }
    else { syntax_error(scanner.next_token().type); } // Else condition is probably an error
    parsetree.pop();
    */
}

shared_ptr<Jump> parser_t::_Jump()
{
    parsetree.push(NT_Jump);
    //@cout << "Jump going to eat: " << scanner.next_token() << endl;
    parsetree.push(T_goto);
    eat_token(T_goto);
    parsetree.pop(); // blah
    int number = scanner._next_token().num;
    //@cout << "Jump going to eat: " << scanner.next_token() << endl;
    parsetree.push(T_num);
    eat_token(T_num);
    parsetree.pop(); // blah
    auto j = shared_ptr<Jump>(new Jump(number));

    if (scanner.next_token() == T_if) { 
        //@cout << "Jump going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_if);
        eat_token(scanner.next_token());
        j->node = _Expression(); 
        parsetree.pop(); // blah
    }

    parsetree.pop();
    return j;
    /*
    if (scanner.next_token().type == T_goto) {
        eat_token(scanner.next_token().type);
        // TODO: Do something with goto
        if (scanner.next_token().type == T_num) {
            eat_token(scanner.next_token().type);
            // TODO: Do something with number
            if (scanner.next_token().type == T_if) {
                eat_token(scanner.next_token().type);
                // TODO: Do something with if
                auto e = Expression();
                // TODO: Do something with \n
            }
            else {
                // TODO: Do something with \n
            }
        }
    }
    parsetree.pop();
    */
}

shared_ptr<Assignment> parser_t::_Assignment()
{
    //@cout << "I was called in assignment" << endl;
    parsetree.push(NT_Assignment);
    //cout << "parsing assignment" << endl;
    auto a = shared_ptr<Assignment>(new Assignment());
    a->me = _MExp();
    //@cout << "Assignment going to eat: " << scanner.next_token() << endl;
    parsetree.push(T_equals);
    eat_token(T_equals);
    parsetree.pop(); // blah
    a->e = _Expression();

    // TODO: Do something with =
    // TODO: Do something with \n
    parsetree.pop();
    return a;
}

shared_ptr<Print> parser_t::_Print()
{
    //cout << "parsing print" << endl;
    //cout << scanner.next_token() << endl;
    parsetree.push(NT_Print);
    parsetree.push(T_print);
    eat_token(T_print);
    parsetree.pop();
    //cout << scanner.next_token() << endl;
    auto p = shared_ptr<Print>(new Print());
    p->node = _MExp();

    parsetree.pop();
    return p;
}

shared_ptr<Expression> parser_t::_Expression()
{
    parsetree.push(NT_Expression);
    //cout << "parsing expression" << endl;
    auto e = shared_ptr<Expression>(new Expression());
    e->t = _Term();
    e->as = _AddSub();

    parsetree.pop();
    return e;
}

shared_ptr<AddSub> parser_t::_AddSub()
{
    parsetree.push(NT_AddSub);
    token_type token = scanner.next_token();
    auto as = shared_ptr<AddSub>(new AddSub());

    // TODO: it may be NT_plus
    if (token == T_plus) {
        as->token = token;
        //@cout << "AddSub going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_plus);
        eat_token(T_plus);
        as->t = _Term();
        as->as = _AddSub();
        parsetree.pop(); // blah
    }
    else if (token == T_minus) {
        as->token = token;
        //@cout << "AddSub going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_minus);
        eat_token(T_minus);
        as->t = _Term();
        as->as = _AddSub();
        parsetree.pop(); // blah
    }
    else {
        // TODO: Do something with ""
        parsetree.drawepsilon();
    }

    parsetree.pop();
    return as;
}

shared_ptr<MultDiv> parser_t::_MultDiv()
{
    parsetree.push(NT_MultDiv);
    token_type token = scanner.next_token();
    auto md = shared_ptr<MultDiv>(new MultDiv());

    if (token == T_times) {
        md->token = token;
        //@cout << "MultDiv going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_times);
        eat_token(token);
        md->f = _Factor();
        md->md = _MultDiv();
        parsetree.pop(); // blah
    }
    else if (token == T_divide) {
        md->token = token;
        //@cout << "MultDiv going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_divide);
        eat_token(token);
        md->f = _Factor();
        md->md = _MultDiv();
        parsetree.pop(); // blah
    }
    else {
        // TODO: Do something with ""
        parsetree.drawepsilon();
    }
    
    parsetree.pop();
    return md;
}

// TODO: This is not complete
shared_ptr<Term> parser_t::_Term()
{
    parsetree.push(NT_Term);
    auto t = shared_ptr<Term>(new Term());

    token_type token  = scanner.next_token();
    if (token == T_openparen || token  == T_num) {
        t->f = _Factor();
        t->md = _MultDiv();
    }
    else if (token == T_m) {
        t->me = _MExp();
    }
    
    else { syntax_error(NT_Term); }
    parsetree.pop();
    return t;
}

// TODO: This is not complete
shared_ptr<Factor> parser_t::_Factor()
{
    parsetree.push(NT_Factor);
    auto f = shared_ptr<Factor>(new Factor());
    f->par = _Parens();
    //@cout << "I got called in Factor" << endl; // TODO: This is broken
    f->exp = _EXP();
    parsetree.pop();
    return f;
}

shared_ptr<Parens> parser_t::_Parens()
{
    parsetree.push(NT_Parens);
    token_type token = scanner.next_token();
    auto par = shared_ptr<Parens>(new Parens());

    if (token == T_openparen) {
        //@cout << "Parens going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_openparen);
        eat_token(token);
        par->node = _Expression();
        parsetree.pop(); // blah
        //@cout << "Parens going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_closeparen);
        eat_token(scanner.next_token());
        parsetree.pop(); //blah
    }
    else if (token == T_num) {
        par->number = scanner._next_token().num;
        par->flag = 1;
        //@cout << "Parens going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_num);
        eat_token(scanner.next_token());
        parsetree.pop(); // blah
    }

    else {
        //@cout << scanner.index << endl;
        //@cout << "the token type is: "<< token << endl; syntax_error(NT_Parens);
    }

    parsetree.pop();
    return par;
    /*
    if (token == T_openparen) {
        eat_token(token);
        // TODO: Do something with (
        auto par = shared_ptr<Parens> Parens();
        if (scanner.next_token().type == T_closeparen) {
            eat_token(scanner.next_token().type);
            // TODO: Do something with )
        }
    }
    else if (token == T_num) {
        eat_token(token);
        auto par = shared_ptr<Parens> Parens();
        par->node = Expression();
        // TODO: Do something with number
        // TODO: Do something with \n
    }
    else { syntax_error(token); }

    parsetree.pop();
    */
}

shared_ptr<MExp> parser_t::_MExp()
{
    parsetree.push(NT_MExp);
    //cout << "MExp going to eat: " << scanner.next_token() << endl;
    //cout << "parsing mexp" << endl;
    parsetree.push(T_m);
    eat_token(T_m);
    //@cout << "MExp going to eat: " << scanner.next_token() << endl;
    parsetree.push(T_opensquare);
    eat_token(T_opensquare);
    auto me = shared_ptr<MExp>(new MExp());
    me->node = _Expression();
    parsetree.pop(); // blah
    parsetree.pop(); // blah
    //@cout << "MExp going to eat: " << scanner.next_token() << endl;
    parsetree.push(T_closesquare);
    eat_token(T_closesquare);
    parsetree.pop(); // blah
    parsetree.pop();
    return me;
    /*
    if (scanner.next_token().type == T_m) {
        eat_token(scanner.next_token().type);
        // TODO: Do something with m
        if (scanner.next_token().type == T_opensquare) {
            eat_token(scanner.next_token().type);
            // TODO: Do something with [
            auto me = shared_ptr<MExp> MExp();
            me->node = Expression();
            return me;
            if (scanner.next_token().type == T_closesquare) {
                // TODO: Do something with ]
            }
        }
    }
    */
}

// TODO: You can't determine whether shared_ptr is NULL
shared_ptr<EXP> parser_t::_EXP()
{
    parsetree.push(NT_EXP);
    auto exp = shared_ptr<EXP>(new EXP());

    if (scanner.next_token() == T_power) {
        //@cout << "EXP going to eat: " << scanner.next_token() << endl;
        parsetree.push(T_power);
        eat_token(scanner.next_token());
        exp->par = _Parens();
        exp->exp = _EXP();
        parsetree.pop();
    }
    else { parsetree.drawepsilon(); }

    parsetree.pop(); // before it was after the call
    return exp;
    /*
    token_type token = scanner.next_token();
    if (token == N_power) {
        eat_token(token);
        // TODO: Do something with pow()
        auto par = Parens();
        auto exp = EXP();
    }
    else {
        // TODO: Do something with ""
    }

    */
}


/*** Main ***********************************************/

int main()
{
    //string str = "254 + 345";
    //split(str);
    //string output = "";
    //scanner_t scanner;
    //scanner.tokenize_input();
    //cout << endl;
    //scanner.print_tokens(scanner.token_list);
    //output = scanner.scan_input();
	parser_t parser;
    parser.scanner.tokenize_input();
    //parser.scanner.print_tokens(parser.scanner.token_list);
    //@cout << "in the main" << endl;
    //@cout << parser.scanner.token_list[parser.scanner.token_list.size() - 1].lineNum << endl;
	parser.parse();
    //@cout << "i reached the end" << endl;
	return 0;
}
