#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;
//By Arnold Villatoro
//Coding Buddy: Iris Moridis 

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
            //case T_J1:	strncpy(buffer,"Jump1",MAX_SYMBOL_NAME_SIZE); break;
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
	NT_Statement,
	NT_Label,
	NT_Jump,
	NT_Assignment,
	NT_Print,
	NT_Expression,
	NT_E1,
	NT_T,
	NT_T1,
	NT_F,
	NT_F1,
	NT_Base,
    NT_J1
	
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
        case NT_Label: strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Jump: strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Assignment: strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Print: strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Expression: strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
        case NT_E1: strncpy(buffer,"E1",MAX_SYMBOL_NAME_SIZE); break;
        case NT_T: strncpy(buffer,"T",MAX_SYMBOL_NAME_SIZE); break;
        case NT_T1: strncpy(buffer,"T1",MAX_SYMBOL_NAME_SIZE); break;
        case NT_F: strncpy(buffer,"F",MAX_SYMBOL_NAME_SIZE); break;
        case NT_F1: strncpy(buffer,"F1",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Base: strncpy(buffer,"Base",MAX_SYMBOL_NAME_SIZE); break;
        case NT_J1: strncpy(buffer,"Jump1",MAX_SYMBOL_NAME_SIZE); break;
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
    vector<int> m;
    
private:
    
    vector<char> inputStorage;
    vector<token_type> tokenStorage;
    
    int lineNumber;
    
    
    //WRITEME: Figure out what you will need to write the scanner
	//and to implement the above interface.  It does not have to
	//be a state machine or anything fancy.  Just read in the
	//characters one at a time (using getchar would be a good way)
	//and group them into tokens.
    
	//error message and exit for unrecognized character
    void scan_error(char x);
	//error message and exit for token mismatch
	void mismatch_error(token_type c);
    
    
};

token_type scanner_t::next_token()
{
	//WRITEME: replace this bogus junk with code that will take a peek
	//at the next token and return it to the parser.  It should _not_
	//actually consume a token - you should be able to call next_token()
	//multiple times without actually reading any more tokens
    
    return tokenStorage[0];
    
}

void scanner_t::eat_token(token_type c)
{
	//if we are supposed to eat token c, and it does not match
	//what we are supposed to be reading from file, then it is a
	//mismatch error ( call - mismatch_error(c) )
    
    if(c != tokenStorage[0]) {
        mismatch_error(c);
    }
    else
        tokenStorage.erase (tokenStorage.begin());
    
}

scanner_t::scanner_t()
{
    int c = 0;
    lineNumber = 0;
    char x;
    int index3=0;
    string numString;
    while (c != -1) {
        c = getchar();
        inputStorage.push_back(c);
    }
    for (int i=0; i < inputStorage.size()-1; i++) {
        x = inputStorage[i];
        //cout<<x<<endl;
        if ((x != '+') && (x != '-') && (x!='*') && (x!='/') && (x!= '(') && (x!=')') && (x != '=') && (x != 'm') && (x!='[') && (x != ']') && (!isdigit(x)) && (x!= 'L') && (x!=':') && (x!='g') && (x!='i') && (x!='p') && (x!='\n') && (x!=' ')){
            //else{
                //cout<<"I am here"<<endl;
                lineNumber+=1;
                scan_error(x);}
        //}
        else{
        if (x == '+') {tokenStorage.push_back(T_plus);}
        if (x == '-') {tokenStorage.push_back(T_minus);}
        if (x == '*') {
            if (inputStorage[i+1] == '*') {
                tokenStorage.push_back(T_power);
                i++;
            }
            else {tokenStorage.push_back(T_times);}
        }
        if (x == '[')
            {tokenStorage.push_back(T_opensquare);}
        if (x == ']')
            {tokenStorage.push_back(T_closesquare);}
        if (x == 'm')
            {tokenStorage.push_back(T_m);}
        if (x== 'L')
            {tokenStorage.push_back(T_label);}
        if(x==':')
            {tokenStorage.push_back(T_colon);}
        if (x == '/')
            {tokenStorage.push_back(T_divide);}
        if (x == '=')
            {tokenStorage.push_back(T_equals);}
        if (x == '(')
            {tokenStorage.push_back(T_openparen);}
        if (x == ')')
            {tokenStorage.push_back(T_closeparen);}
        if (isdigit(x)) {
            if(!isdigit(inputStorage[i+1])){
                tokenStorage.push_back(T_num);
                stringstream ss;
                int s;
                ss<<inputStorage[i];
                ss>>s;
                if (s >= 0 && s!=pow(2,31)-1)
                    m.push_back(s);
                else{
                    printf("scan error: number out of range -line %d\n", get_line()+1);
                    exit(1);}
            }
            else{
                stringstream ss;
                string s;
                ss << inputStorage[i];
                ss >> s;
                numString+=s;
                while(isdigit(inputStorage[i+1])) {
                    stringstream ss;
                    string s;
                    ss << inputStorage[i+1];
                    ss >> s;
                    numString+=s;
                    i++;
                }
                tokenStorage.push_back(T_num);
                int number;
                stringstream p;
                p<<numString;
                p>>number;
                if (number >= 0 && number!= pow(2,31)-1){
                    m.push_back(number);
                    numString="";}
                else{
                    printf("scan error: number out of range -line %d\n", get_line()+1);
                    exit(1);
                }
            }
        }
        
        if(x=='p'){
            index3=i+1;
            if(inputStorage[index3]=='r'){
                index3+=1;
                if(inputStorage[index3]=='i'){
                    index3+=1;
                    if(inputStorage[index3]=='n'){
                        index3+=1;
                        if(inputStorage[index3]=='t'){
                            tokenStorage.push_back(T_print);
                            i+=4;}
                        else{
                            lineNumber+=1;
                            scan_error(inputStorage[index3]);}
                    }
                    else{
                        lineNumber+=1;
                        scan_error(inputStorage[index3]);}
                }
                else{
                    lineNumber+=1;
                    scan_error(inputStorage[index3]);}
            }
            else{
                lineNumber+=1;
                scan_error(inputStorage[index3]);}
        }
        
        if(x=='g'){
	  //cout<<"here"<<endl;
            index3=i+1;
            if(inputStorage[index3]=='o'){
                index3+=1;
                if(inputStorage[index3]=='t'){
                    index3+=1;
                    if(inputStorage[index3]=='o'){
                        tokenStorage.push_back(T_goto);
                        i+=3;}
                    else{
                        lineNumber+=1;
                        scan_error(inputStorage[index3]);}
                }
                else{
                    lineNumber+=1;
                    scan_error(inputStorage[index3]);
                    }
            }
            else{
                lineNumber+=1;
                scan_error(inputStorage[index3]);}
        }
        if(x=='i'){
            index3=i+1;
            if(inputStorage[index3]=='f'){
                tokenStorage.push_back(T_if);
                i+=1;}
            else{
                lineNumber+=1;
                scan_error(inputStorage[index3]);}
        }
        
        if (x == '\n') {
            lineNumber++;}
        }
    }
    tokenStorage.push_back(T_eof);
    //}
    //################################## END INPUT -> TOKEN BLOCK   #############################
} //end input -> token for loop



int scanner_t::get_line()
{
    return lineNumber;
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
    void Statements();
    void Statement();
    void Expression();
    void E1();
    void T();
    void T1();
    void F();
    void F1();
    void Base();
    void final();
    void Assignment();
    void Label();
    void Jump();
    void Jump1();
    void Print();
    void Goto();
    
    
    
public:
    vector<string> cCode;
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
    cCode.push_back("#include <stdio.h>\n#include <math.h>\n");
    cCode.push_back("\nint main()\n");
    cCode.push_back("{\nint m[101];\n");
    Start();
}


//WRITEME: the Start() function is not quite right.  Right now
//it is made to parse the grammar:
//       Start -> '+' Start | EOF
//which is not a very interesting language.  It has been included
//so you can see the basics of how to structure your recursive
//descent code.

//Here is an example
void parser_t::Start()
{
    //push this non-terminal onto the parse tree.
    //the parsetree class is just for drawing the finished
    //parse tree, and should in should have no effect the actual
    //parsing of the data
    parsetree.push(NT_Start);
    
    Statements();
    
    //now that we are done with List, we can pop it from the data
    //stucture that is tracking it for drawing the parse tree
    parsetree.pop();
    cCode.push_back("return 0;");
    cCode.push_back("\n}");
    //parser.cCode.push_back("return 0\n");                                     
    for(int i=0; i<cCode.size(); i++)
      {
	cerr<<cCode[i];
      }
}

void parser_t::Statements()
{
    parsetree.push(NT_Statements);
    if (scanner.next_token() == T_eof) {
        parsetree.drawepsilon();
    }
    else {
        Statement();
        Statements();
    }
    parsetree.pop();
    // parsetree.pop();
}

void parser_t::Statement() {
    parsetree.push(NT_Statement);
    bool test = false;
    
    switch(scanner.next_token()) {
            
        case T_goto:{
            parsetree.push(NT_Jump);
            eat_token(T_goto);
            eat_token(T_num);
            stringstream g;
            g << scanner.m[0];
            string str = g.str();
            
            scanner.m.erase(scanner.m.begin());
            if(scanner.next_token()==T_if){
                parsetree.push(NT_J1);
                eat_token(T_if);
                cCode.push_back("if (");
                Expression();
                cCode.push_back(")\n\t");
                parsetree.pop();
            }
            cCode.push_back("goto L");
            //Expression();
            cCode.push_back(str);
            parsetree.pop();
            //cCode.push_back("}\n");
            
            break;}
        case T_m:
            eat_token(T_m);
            cCode.push_back("m[");
            eat_token(T_opensquare);
            Expression();
            cCode.push_back("]");
            eat_token(T_closesquare);
            cCode.push_back("=");
            eat_token(T_equals);
            Expression();
            break;
        case T_print:
            eat_token(T_print);
            cCode.push_back("printf( \" %i\\n\", ");
            Expression();
            cCode.push_back(")");
            break;
            
        default:
            syntax_error(NT_Statement);
            break;
        case T_label:
            eat_token(T_label);
            cCode.push_back("L");
            eat_token(T_num);
            stringstream t;
            t << scanner.m[0];
            string str = t.str();
            cCode.push_back(str);
            scanner.m.erase(scanner.m.begin());
            eat_token(T_colon);
            test = true;
            cCode.push_back(":\n");
            break;
    }
    if (test==false)
        cCode.push_back(";\n");
    
    parsetree.pop();
}

void parser_t::Expression() {
    parsetree.push(NT_Expression);
    
    T();
    E1();
    
    parsetree.pop();
}

void parser_t::E1() {
    parsetree.push(NT_E1);
    
    switch(scanner.next_token()) {
        case T_plus:
            eat_token(T_plus);
            cCode.push_back(" + ");
            T();
            E1();
            break;
        case T_minus:
            eat_token(T_minus);
            cCode.push_back(" - ");
            T();
            E1();
            break;
            // default:
            //   syntax_error(NT_E1);
            //  break;
    }
    parsetree.drawepsilon();
    parsetree.pop();
}

void parser_t::T() {
    parsetree.push(NT_T);
    F();
    T1();
    parsetree.pop();
}

void parser_t::T1() {
    parsetree.push(NT_T1);
    
    switch(scanner.next_token()) {
        case T_times:
            eat_token(T_times);
            cCode.push_back(" * ");
            F();
            T1();
            break;
        case T_divide:
            eat_token(T_divide);
            cCode.push_back(" / ");
            F();
            T1();
            break;
            // default:
            //   syntax_error(NT_T1);
            
            //break;
    }
    parsetree.drawepsilon();
    parsetree.pop();
}

void parser_t::F() {
    parsetree.push(NT_F);
    
    Base();
    F1();
    
    parsetree.pop();
}

void parser_t::F1() {
    parsetree.push(NT_F1);
    
    if (scanner.next_token() == T_power) {
        eat_token(T_power);
        //     cCode.push_back("pow(");
        Base();
        // cCode.push_back(",");
        F1();
        cCode.push_back(")");
    }
    parsetree.drawepsilon();
    parsetree.pop();
}

void parser_t::Base() {
    parsetree.push(NT_Base);
    switch(scanner.next_token()) {
        case T_openparen:
            eat_token(T_openparen);
            cCode.push_back("(");
            Expression();
            eat_token(T_closeparen);
            cCode.push_back(")");
            if(scanner.next_token()==T_power)
            {
                long length = cCode.size();
                int test=0;
                for(int i=0; i<cCode.size();i++){
                    if(cCode[(cCode.size()-i)-1]!="("){
                             test+=1;
                            continue;
                    }
                    else if(cCode[(cCode.size()-i)-1]=="("){
                       if(cCode[(cCode.size()-i)-2]=="(")
                        {
                          test+=1;
                            continue;
                        }
                        break;}
                }
                string temp = cCode[cCode.size()-(test)-1];
                //cout<<temp<<endl;
                cCode[cCode.size()-(test+1)]="pow(";
                for(int i=1; i<test;i++){
                    cCode[cCode.size()-i]=cCode[cCode.size()-i-1];
                }
                cCode.push_back(")");
                cCode[cCode.size()-test-1]= "(";
                cCode.push_back(",");
            }
            break;
        case T_m:
            cCode.push_back("m");
            eat_token(T_m);
            eat_token(T_opensquare);
            cCode.push_back("[");
            Expression();
            eat_token(T_closesquare);
            cCode.push_back("]");
            if(scanner.next_token()==T_power)
            {
                long length = cCode.size();
                int test=0;
                for(int i=0; i<cCode.size();i++){
                    if(cCode[(cCode.size()-i)-1]!="m"){
                        test+=1;
                        }
                    else{
		      //cout<<cCode[(cCode.size()-i)-2]<<endl;
                        if(cCode[(cCode.size()-i)-2]=="[")
                        {
                            test+=1;
                            continue;
                        }
                        break;}
                }
                string temp = cCode[cCode.size()-(test+1)];
                //cout<<temp<<endl;
                cCode[cCode.size()-(test+1)]="pow(";
                for(int i=1; i<test;i++){
                    cCode[cCode.size()-i]=cCode[cCode.size()-i-1];
                }
                cCode.push_back("]");
                cCode[cCode.size()-test-1]= "m";
                cCode.push_back(",");
            }
            break;
        case T_num:{
            stringstream ss;
            ss << scanner.m[0];
            string str = ss.str();
            eat_token(T_num);
            if (scanner.next_token()==T_power)
            {  cCode.push_back("pow(");
                cCode.push_back(str);
                cCode.push_back(",");
            }
            //            stringstream ss;
            // ss << scanner.m[0];
            // string str = ss.str();
            else
                cCode.push_back(str);
            scanner.m.erase (scanner.m.begin());
            //  eat_token(T_num);
            break;}
        default:
            syntax_error(NT_Base);
            break;
    }
    
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
