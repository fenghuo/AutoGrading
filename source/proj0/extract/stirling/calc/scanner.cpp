#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>
#include <iostream>
#include <queue>
#include <string>

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

	// queue of line numbers of each token
	queue<int> line_numbers;

	int current_line_number=0;

	// queue of tokens
	queue<token_type> tokens;

};

token_type scanner_t::next_token()
{
	return this->tokens.front();
}

void scanner_t::eat_token(token_type c)
{
	token_type next_token = this->next_token();
	if (c != next_token) {
		// token mismatch
		this->mismatch_error(c);
	} else {
		this->tokens.pop();
		this->line_numbers.pop();
	}
}

scanner_t::scanner_t()
{
	//WRITEME: this is the scanner constructor
	string line;
	while (!cin.eof()) {
		getline(cin, line);
		cout << "Read line " << line << endl;
		this->current_line_number++;
		int i = 0, length = line.length();
		while (i < length) {
			token_type token;
			if ('0' <= line[i] && line[i] <= '9') {
				// number
				// Read in all digits after line[i]
				while (i < length && '0' <= line[i] && line[i] <= '9') ++i;
				--i;
				token = T_num;
			}
			else if (line[i] == '+') { token = T_plus; }
			else if (line[i] == '-') { token = T_minus; }
			else if (line[i] == '*') {
				++i;
				if (i < length && line[i] == '*') token = T_power;
				else { token = T_times; --i; }
			}
			else if (line[i] == '/') { token = T_divide; }
			else if (line[i] == '=') { token = T_equals; }
			else if (line[i] == '(') { token = T_openparen; }
			else if (line[i] == ')') { token = T_closeparen; }
			else if (line[i] == '[') { token = T_opensquare; }
			else if (line[i] == ']') { token = T_closesquare; }
			else if (line[i] == 'm') { token = T_m; }
			else if (line.substr(i, 5) == "goto ") { token = T_goto; i+=4; }
			else if (line.substr(i, 3) == "if ") { token = T_if; i+=2; }
			else if (line.substr(i, 6) == "print ") { token = T_print; i+=5; }
			else if (line[i] == 'L') { token = T_label; }
			else if (line[i] == ':') { token = T_colon; }
			else if (line[i] != ' ' && line[i] != '\t') {
				// unknown token, report token
				// cout << "Character " << line[i] << " failed to parse\n";
				this->scan_error(line[i]);
			}
			if (line[i] != ' ' && line[i] != '\t')
				this->tokens.push(token);
				this->line_numbers.push(this->current_line_number);
				// cout << "Parsed token " << token_to_string(token) << " at line " << this->current_line_number << ", i=" << i << endl;
			}
			++i;
		}
	}
	this->tokens.push(T_eof);
	this->line_numbers.push(this->current_line_number);
}

int scanner_t::get_line()
{
	//WRITEME: this function should return the current line number (used
	//for error reporting
	return this->line_numbers.empty() ? this->current_line_number : this->line_numbers.front();
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

int main() {
	scanner_t scanner;
}